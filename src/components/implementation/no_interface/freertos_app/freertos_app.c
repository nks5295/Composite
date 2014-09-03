#include <cos_component.h>
#include <print.h>

#include "freertos_lib.h"

void
cos_init(void)
{
        SemaphoreHandle_t sem;
        printc("!!!!!!!!!!!! I LIVE !!!!!!!!!!!!\n");
        vSemaphoreCreateBinary(sem);
        while(1);
}
