#include <iostream>
#include <string.h>
 
#include <cstdio>
#include <petsc.h>

#define real8 double 
extern "C"{
    void petsc_initialize_f();
	void esh3d_initialize_f(char *, int *, bool *, bool *, bool *, bool *);
    void inhomogeneity_induced_linear_system_f();
    void inhomogeneity_induced_eigenstrain_full_space_f(real8 *);
    void eshelby_inclusion_solver_f();
    void half_fini_solver_f(real8 *);
    void esh3d_destroy_f();
    void esh3d_output_f(int *);
    void write_esh3d_parameters_f(void);
}
struct Param_t
{
    bool fullSpace;
    bool inhomogeneous;
    bool halfSpace;
    bool finite;
    double appliedStress[6];
    int tecmeshcounter;
};
PetscErrorCode Esh3dSolver(Param_t *param);

int main(int argc, char *argv[]) {
    int         i, length, rank, size;
    char        inpfile[256]="esh3d_top.inp";
    PetscBool   petscTest1 = PETSC_FALSE;
    int         counterMax=10;

    Param_t param;

    PetscFunctionBeginUser;

    MPI_Init(&argc, &argv); 

    if(argc >= 3){
        if (!strcmp(argv[1], "-f")){
            snprintf(inpfile,sizeof(inpfile),"%s",argv[2]);
        }
    }

    length = strlen(inpfile);
    for(i=strlen(inpfile);i<(int)sizeof(inpfile);i++) inpfile[i]=' ';

    param.tecmeshcounter = 0;
//  PetscInitializeNoArguments();

    PetscInitializeFortran();
    petsc_initialize_f();
    PetscOptionsBegin(PETSC_COMM_WORLD, "test_","options for tests",__FILE__); 
    PetscCall(PetscOptionsBool("-1", "1. small demo for building a system of "
                "partial differential equations (PDEs) ",__func__,petscTest1,&petscTest1,NULL));
    PetscOptionsEnd();
    
    MPI_Comm_size(PETSC_COMM_WORLD, &size);
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

    esh3d_initialize_f(inpfile, &length, &param.fullSpace, 
            &param.inhomogeneous, &param.halfSpace, &param.finite);
    if(param.inhomogeneous){
/**
 *      @brief Construct a new inhomogeneity induced linear system object
 *      @details create:
 *          -Mat MatKeig
 *          -Ksp KryInc
 *          -Vec Vec_Feig, Vec_Eig
 *              if(nfluid>0) create:
 *                  - Mat Mat_kfld, Mat_Kvol
 *                  - Ksp KryFld, KryVol
 *                  - Vec Vec_Evol Vec_Fvol
 *                  - if(nsolid>0) create:
 *                      - Vec Vec_dEig
 *      
 */
        inhomogeneity_induced_linear_system_f();
    }

    while (param.tecmeshcounter<counterMax)
    {
        param.appliedStress[0] = 1.0E0;
        param.appliedStress[1] = 0.0E0;
        param.appliedStress[2] = 0.0E0;
        param.appliedStress[3] = 4.0E0;
        param.appliedStress[4] = 1.0E8*double(param.tecmeshcounter);
        param.appliedStress[5] = 0.0E0;
        PetscCall(Esh3dSolver(&param));
        esh3d_output_f(&param.tecmeshcounter);
        param.tecmeshcounter++;
    }
    
    esh3d_destroy_f();
    PetscCall(PetscFinalize());
    MPI_Finalize();
    return 0;
 }

PetscErrorCode Esh3dSolver(Param_t *param)
{

    PetscFunctionBegin;

    if(param->fullSpace){
        if(param->inhomogeneous){
/**
 *          @details
 *              - update remote applied stress(if inhomogeneous)
 *              - Set Vec_Feig
 *              - Solve KryInc \cdot Vec_Eig = Vec_Feig
 *              - if(nfluid>0):
 *                  - Set Vec_Fvol
 *                  - Solve KryVol  Vec_Eig = Vec_Fvol
 *                  - Solve KryFld  Vec_Eig = Vec_Feig
 *                  - if(nsolid>0):
 *                      - Vec_dEig = Vec_Eig
 *                      - iteration: KryVol Vec_Evol = Vec_Fvol, KryFld Vec_dEig = Vec_Feig
 *              - Update interactive Eigen strain: ellipeff
 */
            inhomogeneity_induced_eigenstrain_full_space_f(param->appliedStress);
        }
        eshelby_inclusion_solver_f();
    }else if(param->halfSpace || param->finite){
/**
 *      @details
 *  
 * 
 */
        half_fini_solver_f(param->appliedStress);
    }
    PetscFunctionReturn(0);
}