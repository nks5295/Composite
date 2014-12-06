#ifndef FRT_H
#define FRT_H

int frt_vSemaphoreBinaryCreate(void);
int frt_xSemaphoreTake(int xSemaphore, int xTicksToWait);
int frt_xSemaphoreGive(int xSemaphore);
int frt_xTaskCreate(const char * const pcName, unsigned short usStackDepth, void *pvParameters, int uxPriority);
void frt_vTaskDelete(int xTask);
void frt_vTaskSuspend(int xTaskToSuspend);
void frt_vTaskDelay(int xTicksToDelay);
//void frt_vTaskDelayUntil(int *pxPreviousWakeTime, int xTimeIncrement);
int frt_vTaskResume(int xTaskToResume);
int frt_xTaskResumeFromISR(int xTaskToResume);
//void frt_uxTaskGetSystemState(...);
int frt_xTaskGetCurrentTaskHandle(void);
int frt_xTaskGetIdleTaskHandle(void);
int frt_uxTaskGetStackHighWaterMark(int xTask);
//eTaskState frt_eTaskGetState(int xTask);
//char* frt_pcTaskGetName(int xTaskToQuery);
int frt_xTaskGetTickCount(void);
int frt_xTaskGetTickCountFromISR(void);
int frt_xQueueCreate(int uxQueueLength, int uxItemSize, int ptr, spdid_t spd);
int frt_xQueueSend(int xQueue, int item, int xTicksToWait);
int frt_xQueueReceive(int xQueue, int buff, int xTicksToWait);
#endif	/* FRT_H */
