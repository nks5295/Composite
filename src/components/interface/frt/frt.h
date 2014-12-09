#ifndef FRT_H
#define FRT_H

int frt_vSemaphoreBinaryCreate(void);
int frt_xSemaphoreTake(int xSemaphore, int xTicksToWait);
int frt_xSemaphoreGive(int xSemaphore);
int frt_xTaskCreate(const char * const pcName, unsigned short usStackDepth, void *pvParameters, int uxPriority);
void frt_vTaskDelete(int xTask);
int frt_uxTaskPriorityGet(int xTask);
void frt_vTaskPrioritySet(int xTask, int uxNewPriority);
void frt_vTaskSuspend(int xTaskToSuspend);
void frt_vTaskDelay(int xTicksToDelay);
//void frt_vTaskDelayUntil(int *pxPreviousWakeTime, int xTimeIncrement);
int frt_vTaskResume(int xTaskToResume);
int frt_xTaskResumeFromISR(int xTaskToResume);
//void frt_uxTaskGetSystemState(...);
int frt_xTaskGetCurrentTaskHandle(void);
int frt_xTaskGetIdleTaskHandle(void);
unsigned int frt_uxTaskGetStackHighWaterMark(int xTask);
//eTaskState frt_eTaskGetState(int xTask);
//char* frt_pcTaskGetName(int xTaskToQuery);
//void frt_vTaskGetRunTimeStats(char *pcWriteBuffer);
//void frt_vTaskSetApplicationTaskTag(int xTask, TODO pxTagValue);
//TODO frt_xTaskGetApplicationTaskTag(int xTask);
//int frt_xTaskCallApplicationTaskHook(int xTask, void *pvParameter);
int frt_xTaskGetTickCount(void);
int frt_xTaskGetTickCountFromISR(void);
int frt_xTaskGetSchedulerState(void);
unsigned int frt_uxTaskGetNumberOfTasks(void);
//void frt_vTaskList(char *pcWriteBuffer);
//void frt_vTaskStartTrace(char *pcBuffer, unsigned long ulBufferSize);
//unsigned long frt_ulTaskEndTrace(void); // removed this since it was gone v7.1 ->
int frt_xQueueCreate(int uxQueueLength, int uxItemSize, int ptr, spdid_t spd);
int frt_xQueueSend(int xQueue, int item, int xTicksToWait);
int frt_xQueueReceive(int xQueue, int buff, int xTicksToWait);
#endif	/* FRT_H */
