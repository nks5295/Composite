//FreeRTOS API includes
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOSConfig.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/FreeRTOS.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/queue.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/semphr.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/task.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/croutine.h"

#define MAX_FRT_OBJS 255

//Freertos API Types
typedef enum {
        FRT_OBJ_SEMA,
        FRT_OBJ_MUTEX,
        FRT_OBJ_TASK,
        FRT_OBJ_QUEUE,
} frt_object_t;

struct frt_obj_mapping {
        frt_object_t type;
        void *obj;
        int used;
} frt_obj_array[MAX_FRT_OBJS];

/**********
 * FreeRTOS Semaphore API
**********/
int
freertos_vSemaphoreBinaryCreate (void)
{
//        printc("In freertos_vSemaphoreBinaryCreate\n %d \n", frt_obj_array[0].used);
	int ret = 0;
//        xSemaphoreHandle fak;

        while (frt_obj_array[ret].used != 0 && ret != MAX_FRT_OBJS) {
                ret++;
 //               printc("fak\n");
        }
	if (ret == MAX_FRT_OBJS) return -1;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;
        frt_obj_array[ret].used = 1;
//        freertos_print("Just before vSemaphoreCreateBinary\n");
        vSemaphoreCreateBinary(frt_obj_array[ret].obj);
//        printc("wat made fak %d\n%d\n", frt_obj_array[ret].obj,ret);
	return ret;
}

int
freertos_xSemaphoreBinaryCreateCounting (portBASE_TYPE uxMaxCount, int uxInitialCount)
{
        int ret = 0;
        while (frt_obj_array[ret].used != 0) ret++;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;
        frt_obj_array[ret].used = 1;

        frt_obj_array[ret].obj = xSemaphoreCreateCounting(uxMaxCount, uxInitialCount);

        return ret;
}

int
freertos_xSemaphoreCreateMutex (void)
{
        int ret = 0;
        while (frt_obj_array[ret].used != 0) ret++;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;
        frt_obj_array[ret].used = 1;

        frt_obj_array[ret].obj = xSemaphoreCreateMutex();

        return ret;
}

int
freertos_xSemaphoreCreateRecursiveMutex (void)
{
        int ret = 0;
        while (frt_obj_array[ret].used != 1) ret++;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;
        frt_obj_array[ret].used = 1;

        frt_obj_array[ret].obj = xSemaphoreCreateRecursiveMutex();

        return ret;
}

int
freertos_xSemaphoreTake (int xSemaphore, int xTicksToWait)
{
        assert(frt_obj_array[xSemaphore].type == FRT_OBJ_SEMA);
        return xSemaphoreTake(frt_obj_array[xSemaphore].obj, (portTickType) xTicksToWait);
}

int
freertos_xSemaphoreTakeRecursive (int xMutex, int xTicksToWait)
{
        assert(frt_obj_array[xMutex].type == FRT_OBJ_SEMA);
        return xSemaphoreTakeRecursive(frt_obj_array[xMutex].obj, (portTickType) xTicksToWait);
}

int
freertos_xSemaphoreGive (int xSemaphore)
{
        assert(frt_obj_array[xSemaphore].type == FRT_OBJ_SEMA);
        return xSemaphoreGive(frt_obj_array[xSemaphore].obj);
}

int
freertos_xSemaphoreGiveRecursive (int xMutex)
{
        assert(frt_obj_array[xMutex].type == FRT_OBJ_SEMA);
        return xSemaphoreGiveRecursive(frt_obj_array[xMutex].obj);
}

/********
 * FreeRTOS Task API
 ********/

int
freertos_xTaskCreate (void *pvTaskCode, const char *const pcName, unsigned short usStackDepth, void *pvParameters, int uxPriority, void *pvCreatedTask)
{
        return 0;
}

void
freertos_vTaskDelete (int xTask)
{
        if (xTask == NULL) vTaskDelete((xTaskHandle) xTask);

        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);

        vTaskDelete(frt_obj_array[xTask].obj);
}

int
freertos_uxTaskPriorityGet(int xTask)
{
        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);

        return (int) uxTaskPriorityGet(frt_obj_array[xTask].obj);
}

void
freertos_vTaskPrioritySet(int xTask, int uxNewPriority)
{
        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);

        vTaskPrioritySet(frt_obj_array[xTask].obj, (portBASE_TYPE) uxNewPriority);
}

