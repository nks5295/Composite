#include <cos_component.h>
#include <print.h>
#include <cos_alloc.h>

#include "freertos_lib.h"

void
cos_init(void)
{
        int a = 0, b = 0;
        SemaphoreHandle_t sem = 20;
        printc("!!!!!!!!!!!! I LIVE !!!!!!!!!!!!%d\n", sem);
        rdtscll(a);
        vSemaphoreBinaryCreate(sem);
        rdtscll(b);
        printc("%d cycles c:\n", (b - a));
        while(1);
}
