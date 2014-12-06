/**
 * Copyright 2011 by Gabriel Parmer, gparmer@gwu.edu
 *
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 */

#include <cos_asm_server_stub_simple_stack.h>
.text	

cos_asm_server_stub(frt_vSemaphoreBinaryCreate)
cos_asm_server_stub(frt_xSemaphoreTake)
cos_asm_server_stub(frt_xSemaphoreGive)
cos_asm_server_stub(frt_xTaskCreate)
cos_asm_server_stub(frt_vTaskDelete)
cos_asm_server_stub(frt_vTaskSuspend)
cos_asm_server_stub(frt_vTaskDelay)
//cos_asm_server_stub(frt_vTaskDelayUntil)
cos_asm_server_stub(frt_vTaskResume)
cos_asm_server_stub(frt_xTaskResumeFromISR)
//cos_asm_server_stub(frt_uxTaskGetSystemState)
cos_asm_server_stub(frt_xTaskGetCurrentTaskHandle)
cos_asm_server_stub(frt_xTaskGetIdleTaskHandle)
cos_asm_server_stub(frt_uxTaskGetStackHighWaterMark)
//cos_asm_server_stub(frt_eTaskGetState)
//cos_asm_server_stub(frt_pcTaskGetName)
cos_asm_server_stub(frt_xTaskGetTickCount)
cos_asm_server_stub(frt_xTaskGetTickCountFromISR)
cos_asm_server_stub(frt_xQueueCreate)
cos_asm_server_stub(frt_xQueueSend)
cos_asm_server_stub(frt_xQueueReceive)
