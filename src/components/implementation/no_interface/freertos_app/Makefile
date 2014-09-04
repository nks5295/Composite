C_OBJS=freertos_app.o
ASM_OBJS=
COMPONENT=rtos_app.o
INTERFACES=
DEPENDENCIES=printc frt mem_mgr
FN_PREPEND=

include ../../Makefile.subsubdir
ifeq (${ENABLE_STACK_MANAGER},1)
 MANDITORY_LIB=simple_stklib.o
endif

