//FreeRTOS API includes
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOSConfig.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/FreeRTOS.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/queue.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/semphr.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/task.h"
#include "../../../lib/freeRTOS/FreeRTOS_Posix/FreeRTOS_Kernel/include/croutine.h"

#include <cos_alloc.h>
#include <mem_mgr.h>
#include "frt_rb.h"
#define MAX_FRT_OBJS 255

extern volatile int thread_extern_spd;

//Freertos API Types
typedef enum {
        FRT_OBJ_SEMA,
        FRT_OBJ_MUTEX,
        FRT_OBJ_TASK,
        FRT_OBJ_QUEUE,
        FRT_OBJ_EMPTY,
} frt_object_t;

struct frt_obj_mapping {
        frt_object_t type;
        int tid;
        void *obj;
} frt_obj_array[MAX_FRT_OBJS] = {FRT_OBJ_EMPTY, NULL, NULL};

struct rb_head *app_rb;
struct rb_head *k_rb;

static volatile int memory_created = 0;

/**********
 * FreeRTOS Semaphore API
**********/
int
frt_vSemaphoreBinaryCreate (void)
{
	int ret = 0;

        while (frt_obj_array[ret].type != FRT_OBJ_EMPTY && ret != MAX_FRT_OBJS) ret++;
	if (ret == MAX_FRT_OBJS) return -1;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;
        vSemaphoreCreateBinary(frt_obj_array[ret].obj);
	return ret;
}

int
frt_xSemaphoreBinaryCreateCounting (portBASE_TYPE uxMaxCount, int uxInitialCount)
{
        int ret = 0;
        while (frt_obj_array[ret].type != FRT_OBJ_EMPTY && ret != MAX_FRT_OBJS) ret++;
        if (ret == MAX_FRT_OBJS) return -1;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;

        frt_obj_array[ret].obj = xSemaphoreCreateCounting(uxMaxCount, uxInitialCount);

        return ret;
}

int
frt_xSemaphoreCreateMutex (void)
{
        int ret = 0;
        while (frt_obj_array[ret].type != FRT_OBJ_EMPTY && ret != MAX_FRT_OBJS) ret++;
        if (ret == MAX_FRT_OBJS) return -1;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;

        frt_obj_array[ret].obj = xSemaphoreCreateMutex();

        return ret;
}

int
frt_xSemaphoreCreateRecursiveMutex (void)
{
        int ret = 0;
        while (frt_obj_array[ret].type != FRT_OBJ_EMPTY && ret != MAX_FRT_OBJS) ret++;
        if (ret == MAX_FRT_OBJS) return -1;
        frt_obj_array[ret].type = FRT_OBJ_SEMA;
        frt_obj_array[ret].obj = xSemaphoreCreateRecursiveMutex();

        return ret;
}

int
frt_xSemaphoreTake (int xSemaphore, int xTicksToWait)
{
        assert(frt_obj_array[xSemaphore].type == FRT_OBJ_SEMA);
        return xSemaphoreTake(frt_obj_array[xSemaphore].obj, (portTickType) xTicksToWait);
}

int
frt_xSemaphoreTakeRecursive (int xMutex, int xTicksToWait)
{
        assert(frt_obj_array[xMutex].type == FRT_OBJ_SEMA);
        return xSemaphoreTakeRecursive(frt_obj_array[xMutex].obj, (portTickType) xTicksToWait);
}

int
frt_xSemaphoreGive (int xSemaphore)
{
        assert(frt_obj_array[xSemaphore].type == FRT_OBJ_SEMA);
        return xSemaphoreGive(frt_obj_array[xSemaphore].obj);
}

int
frt_xSemaphoreGiveRecursive (int xMutex)
{
        assert(frt_obj_array[xMutex].type == FRT_OBJ_SEMA);
        return xSemaphoreGiveRecursive(frt_obj_array[xMutex].obj);
}

/********
 * FreeRTOS Task API
 ********/

unsigned int
frt_xTaskCreate (const char *const pcName, unsigned short usStackDepth, void *pvParameters, int uxPriority)
{
        printc("prio is %d\n", uxPriority);
        unsigned short tid;
        unsigned short ref;
        xTaskHandle th;

        unsigned int ret = 0;
        while (frt_obj_array[ret].type != FRT_OBJ_EMPTY) ret++;
        if (ret == MAX_FRT_OBJS) return -1;
        frt_obj_array[ret].type = FRT_OBJ_TASK;

        thread_extern_spd = 1;
        printc("Before xtask\n");
        xTaskCreate(0, pcName, usStackDepth, pvParameters, uxPriority, &th);
        frt_obj_array[ret].obj = th;
        tid = (unsigned short) prvGetThreadHandle((xTaskHandle) frt_obj_array[ret].obj);
        ref = (unsigned short) ret;
        ret = (tid << 16) | ref;

        return ret;
}

void
frt_vTaskDelete (int xTask)
{
        printc("In vTaskDelete\n");
        if (xTask == NULL) vTaskDelete((xTaskHandle) xTask);
        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);

        vTaskDelete(frt_obj_array[xTask].obj);
        printc("Task ID: %d deleted\n", (int) prvGetThreadHandle((xTaskHandle) frt_obj_array[xTask].obj));
}

int
frt_uxTaskPriorityGet(int xTask)
{
        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);

        return (int) uxTaskPriorityGet(frt_obj_array[xTask].obj);
}

void
frt_vTaskPrioritySet(int xTask, int uxNewPriority)
{
        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);

        vTaskPrioritySet(frt_obj_array[xTask].obj, (portBASE_TYPE) uxNewPriority);
}

void
frt_vTaskSuspend(int xTaskToSuspend)
{
        if (xTaskToSuspend == NULL) vTaskSuspend((xTaskHandle) xTaskToSuspend);

        assert(frt_obj_array[xTaskToSuspend].type == FRT_OBJ_TASK);
        vTaskSuspend((xTaskHandle) frt_obj_array[xTaskToSuspend].obj);
}

void
frt_vTaskDelay(int xTicksToDelay)
{
        assert(xTicksToDelay >= 0);

        vTaskDelay((TickType) frt_obj_array[xTicksToDelay].obj);
}

// TODO: how do i do pionters here
/*
void
frt_vTaskDelayUntil(int *pxPrevoiusWakeTime, int xTimeIncrement)
{
}
*/

void
frt_vTaskResume(int xTaskToResume)
{
        assert(frt_obj_array[xTaskToResume].type == FRT_OBJ_TASK);

        vTaskResume((xTaskHandle) frt_obj_array[xTaskToResume].obj);
}

int
frt_xTaskResumeFromISR(int xTaskToResume)
{
        assert(frt_obj_array[xTaskToResume].type == FRT_OBJ_TASK);

        return (int) xTaskResumeFromISR((xTaskHandle) frt_obj_array[xTaskToResume].obj);
}

// TODO: Pointers issue here again
/*
unsigned int
uxTaskGetSystemState(...)
{
}
*/

int
frt_xTaskGetCurrentTaskHandle(void)
{
        return (int) xTaskGetCurrentTaskHandle();
}

int
frt_xTaskGetIdleTaskHandle(void)
{
        return (int) xTaskGetIdleTaskHandle();
}

unsigned int
frt_uxTaskGetStackHighWaterMark(int xTask)
{
        if (xTask == NULL) return (unsigned int) uxTaskGetStackHighWaterMark((xTaskHandle) xTask);

        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);
        return (unsigned int) uxTaskGetStackHighWaterMark(frt_obj_array[xTask].obj);
}

// TODO: What to do with enumerated type?
/*
eTaskState
frt_eTaskGetState(int xTask)
{
        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);

        return (eTaskState) eTaskGetState(frt_obj_array[xTask].obj);
}
*/

// TODO: returns a char* pointer issue
/*
char*
frt_pcTaskGetName(int xTaskToQuery)
{
        if (xTaskToQuery == NULL) return (char *) pcTaskGetName((xTaskHandle) xTaskToQuery);

        assert(frt_obj_array[xTaskToQuery].type == FRT_OBJ_TASK);
        return (char *) pcTaskGetName(frt_obj_array[xTaskToQuery].obj);
}
*/

int
frt_xTaskGetTickCount(void)
{
        return (int) xTaskGetTickCount();
}

int
frt_xTaskGetTickCountFromISR(void)
{
        return (int) xTaskGetTickCountFromISR();
}

int
frt_xTaskGetSchedulerState(void)
{
        return (int) xTaskGetSchedulerState();
}

unsigned int
frt_uxTaskGetNumberOfTasks(void)
{
        return (unsigned int) uxTaskGetNumberOfTasks();
}

// TODO: char * pointer
/*
void
frt_vTaskList(char *pcWriteBuffer)
{
}
*/

// TODO: char * pointer
/*
void
frt_vTaskStartTrace(char *pcBuffer, unsigned long ulBufferSize)
{
}
*/

// removed this since it was removed in freeRTOS v7.1
/*
unsigned long
frt_ulTaskEndTrace(void)
{
        return (unsigned long) ulTaskEndTrace();
}
*/

// TODO: How do I pointer?
/*
void
frt_vTaskGetRunTimeStats(char *pcWriteBuffer)
{
        vTaskGetRunTimeStats(pcWriteBuffer);
}
*/

// TODO: More (function) pointers :(
// TaskHookFunction_t seems as if its a function pointer
/*
void
frt_vTaskSetApplicationTaskTag(int xTask, TaskHookFunction_t pxTagValue)
{
        if (xTask == NULL) {
                vTaskSetApplicationTaskTag( (xTaskHandle) xTask, pxTagValue);
                return;
        }

        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);
        vTaskSetApplicationTaskTag( (xTaskHandle) frt_obj_array[xTask].obj, pxTagValue);
        // probably need to case the function pointer above to something
        // but what?
}
*/

// TODO: Pointers :)
/*
frt_xTaskGetApplicationTaskTag(int xTask){
        // the two return statements probably need casts
        if (xTask == NULL) return xTaskGetApplicationTaskTag( (xTaskHandle) xTask);

        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);
        return xTaskGetApplicationTaskTag( (xTaskHandle) frt_obj_array[xTask].obj);
}
*/

// TODO: how to pointer?
// http://xkcd.com/138/
/*
int
frt_xTaskCallApplicationTaskHook(int xTask, void *pvParameter)
{
        if (xTask == NULL) return (int) xTaskCallApplicationTaskHook( (xTaskHandle) xTask, pvParameter);

        assert(frt_obj_array[xTask].type == FRT_OBJ_TASK);
        return (int) xTaskCallApplicationTaskHook( (xTaskHandle) xTask, pvParameter);
}
*/

/*******
 * FreeRTOS Queue API
*******/

int
frt_xQueueCreate(int uxQueueLength, int uxItemSize, int ptr, spdid_t spd)
{
        int ret = 0, wat = 99, t = 0;
        while (frt_obj_array[ret].type != FRT_OBJ_EMPTY && ret != MAX_FRT_OBJS) ret++;
	if (ret == MAX_FRT_OBJS) return -1;
        frt_obj_array[ret].type = FRT_OBJ_QUEUE;
        printc("xQueue ret is: %d\n", ret);

        //if (unlikely(memory_created == 0)) {
        if (memory_created == 0) {
                memory_created = 1;
                assert(spd >= 5);

                vaddr_t retn =  - 1;
                vaddr_t frt_ptr = -1;

                if (!(frt_ptr = mman_get_page(cos_spd_id(), (vaddr_t)cos_get_heap_ptr(), MAPPING_RW))) BUG();
                if (cos_get_heap_ptr() != frt_ptr) printc("Failure\n");
                cos_set_heap_ptr(frt_ptr + 4096);
                retn = mman_alias_page(cos_spd_id(), frt_ptr, spd, (vaddr_t) ptr, MAPPING_RW);

                app_rb = frt_ptr;
                app_rb->prod = app_rb->cons = 0;

                retn = -1;
                frt_ptr = -1;

                if (!(frt_ptr = mman_get_page(cos_spd_id(), (vaddr_t)cos_get_heap_ptr(), MAPPING_RW))) BUG();
                if (cos_get_heap_ptr() != frt_ptr) printc("Failure\n");
                cos_set_heap_ptr(frt_ptr + 4096);
                retn = mman_alias_page(cos_spd_id(), frt_ptr, spd, (vaddr_t) ptr + 4096, MAPPING_RW);

                k_rb = frt_ptr;
                k_rb->prod = k_rb->cons = 0;

                frt_obj_array[ret].obj = xQueueCreate((portBASE_TYPE) uxQueueLength, (portBASE_TYPE) uxItemSize);
                printc("queue is %d\n", frt_obj_array[ret].obj);
                return ret;
        } else {
                frt_obj_array[ret].obj = xQueueCreate((portBASE_TYPE) uxQueueLength, (portBASE_TYPE) uxItemSize);
                return ret;
        }
}

int
frt_xQueueSend(int xQueue, int item, int xTicksToWait)
{
        assert(frt_obj_array[(int) xQueue].type == FRT_OBJ_QUEUE);
        int wat = xQueueSend((xQueueHandle) frt_obj_array[xQueue].obj, (void *) &(app_rb->ring[item]), (portTickType) xTicksToWait);
        printc("val is %d\n", app_rb->ring[item]);
        return wat;
}

int
frt_xQueueReceive(int xQueue, int buff, int xTicksToWait)
{
        int ret, tmp;
        assert(frt_obj_array[(int) xQueue].type == FRT_OBJ_QUEUE);
        ret = xQueueReceive((xQueueHandle) frt_obj_array[xQueue].obj, (void *) &tmp, (portTickType) xTicksToWait);
        rb_insert(k_rb, (void *) &tmp, sizeof(int));
        rb_fastremove(app_rb, sizeof(int));
        return ret;
}
