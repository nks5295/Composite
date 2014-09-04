#!/bin/sh

./cos_loader \
"c0.o, ;llboot.o, ;*fprr.o, ;mm.o, ;print.o, ;boot.o, ;\
!va.o, a2;!freeRTOS.o, a8;!l.o, a1;!mpool.o, a3;!sm.o, a4;!vm.o, a1;!rtos_app.o, a8:\
c0.o-llboot.o;\
fprr.o-print.o|[parent_]mm.o|[faulthndlr_]llboot.o;\
mm.o-[parent_]llboot.o|print.o;\
boot.o-print.o|fprr.o|mm.o|llboot.o;\
sm.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o|mpool.o;\
mpool.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o;\
vm.o-fprr.o|print.o|mm.o|l.o|boot.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o|vm.o;\
l.o-fprr.o|mm.o|print.o;\
\
rtos_app.o-fprr.o|freeRTOS.o|mm.o|boot.o|print.o;\
freeRTOS.o-[parent_]fprr.o|print.o\
" ./gen_client_stub
