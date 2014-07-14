C_OBJS=timer_test.o
ASM_OBJS=
COMPONENT=timeTEST.o
INTERFACES=
FN_PREPEND=parent_
DEPENDENCIES=printc sched sched_hier
IF_LIB=

include ../../Makefile.subsubdir

ifeq (${ENABLE_STACK_MANAGER},1)
  MANDITORY_LIB=simple_stklib.o
endif

