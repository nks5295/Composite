#ifndef FREERTOS_LIB_H
#define FREERTOS_LIB_H

#include "../freertos/freertos_klib.h"

typedef int* SemaphoreHandle_t;

void
vSemaphoreBinaryCreate (SemaphoreHandle_t xSemaphore)
{
        freertos_print("Semaphore creating\n");
        freertos_vSemaphoreBinaryCreate(xSemaphore);
        freertos_print("Semaphore Created!\n");
}

#endif
