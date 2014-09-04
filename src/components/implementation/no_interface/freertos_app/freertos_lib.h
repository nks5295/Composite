#ifndef FREERTOS_LIB_H
#define FREERTOS_LIB_H

//#include <freertos_klib.h>
#include <frt.h>

typedef void *SemaphoreHandle_t;

void
vSemaphoreBinaryCreate (SemaphoreHandle_t xSemaphore)
{
	int handle;
	//int *sem_ptr = (int*)xSemaphore;

        //printc("Semaphore creating\n");
        handle = freertos_vSemaphoreBinaryCreate();
        //printc("Just before sem_ptr, handle: %d\n", handle);
	xSemaphore = handle;
        //printc("Semaphore Created!\n");
}

#endif
