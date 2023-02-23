# Makefile for Esh3D 

FFLAGS =  
FPPFLAGS =
LDLIBS =
    
HDF_F_LIB = $(shell if [[ `which h5fc`  ]]; then h5fc -show | cut -d " " -f 2-20; elif [[ `which h5pfc`  ]]; then h5pfc -show | cut -d " " -f 2-20; fi)
HDF_CXX_LIB = $(shell if [[ `which h5c++`  ]]; then h5c++ -show | cut -d " " -f 2-20; elif [[ `which h5pc++`  ]]; then h5pc++ -show | cut -d " " -f 2-20; fi)

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

ifeq ($(debug),true)
	FFLAGS += -O0 -g
else
	ifeq ($(CC),icc)
		FFLAGS += -ipo -diag-disable ipo -static_intel 
	else
		FFLAGS += -Wno-maybe-uninitialized -Wno-tabs -Wno-unused-function -O3
	endif
endif

OBJS = main.o m_esh3d.o m_global.o m_local.o m_elems.o m_utils.o
OBJSLIB = cpp_main.o m_interface.o m_esh3d.o m_global.o m_local.o m_elems.o m_utils.o

m_utils.o  : m_utils.F90
m_esh3d.o  : m_esh3d.F90 m_utils.o
m_elems.o  : m_elems.F90 m_utils.o
m_local.o  : m_local.F90 m_elems.o
m_global.o : m_global.F90 m_local.o
main.o     : main.F90 m_global.o m_esh3d.o
m_interface.o : m_interface.F90 m_global.o m_esh3d.o
cpp_main.o : cpp_main.cpp m_interface.o m_global.o m_esh3d.o

# debug info

info:
	-$(info FLINKER = ${FLINKER})
	-$(info HDF_F_LIB = $(HDF_F_LIB))
	-$(info HDF_CXX_LIB = $(HDF_CXX_LIB))
	-$(info OBJS = $(OBJS))
	-$(info PETSC_LIB = $(PETSC_LIB))
	-$(info LDLIBS = $(LDLIBS))

all: ${OBJS}
	-${FLINKER} ${OBJS} -o ../bin/esh3d ${HDF_F_LIB} ${PETSC_LIB} ${LDLIBS}

cpp: ${OBJSLIB}
	- ${CXXLINKER} ${OBJSLIB} -o ../bin/esh3dwraper ${HDF_F_LIB} ${HDF_CXX_LIB} ${PETSC_LIB} ${LDLIBS}

