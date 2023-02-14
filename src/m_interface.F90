
module esh3d_wraper

#include <petscversion.h>

  use esh3d
  use global
#if (PETSC_VERSION_MAJOR==3 && PETSC_VERSION_MINOR<=7 && PETSC_VERSION_SUBMINOR<5)
  implicit none
#include "petsc.h"
#else
#include <petsc/finclude/petscksp.h>
  use petscksp
  implicit none
#endif

character(256) :: input_file

contains

subroutine read_esh3d_parameters(inpfile, length) bind(c)
    use iso_c_binding
    implicit none
    integer(c_int) :: length
    character(c_char) :: inpfile

!   local variables
    integer :: nodal_bw

    input_file = inpfile(1:length)
    open(10,file=input_file,status='old')
    call ReadParameters(nodal_bw)
    close(10)
end subroutine read_esh3d_parameters

subroutine write_esh3d_parameters() bind(c)
    write(*,*)"nobsArray=",nobsArray(1:3)
end subroutine write_esh3d_parameters

end module esh3d_wraper
