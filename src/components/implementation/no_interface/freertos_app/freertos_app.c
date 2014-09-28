#include <cos_component.h>
#include <print.h>
#include <cos_alloc.h>

#include "freertos_lib.h"

static volatile int a = 1;
typedef void (*funcptr)(void);
xQueueHandle queue;

void
send(void){
        int in = 10;
        xQueueSend(queue, (void *) &in, 0);
        xQueueSend(queue, (void *) &(in), 0);
        in++;
       xQueueSend(queue, (void *) &in, 0);
       xQueueSend(queue, (void *) &in, 0);
       xQueueSend(queue, (void *) &in, 0);
       xQueueSend(queue, (void *) &in, 0);
       xQueueSend(queue, (void *) &in, 0);
       xQueueSend(queue, (void *) &in, 0);
       xQueueSend(queue, (void *) &in, 0);
        printc("In %d\n", in);
        vTaskSuspend(NULL);
}

void
receive(void)
{
        int out;
        xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
       xQueueReceive(queue, (void *) &out, 0);
        printc("Out %d\n", out);
}


void
cos_init(void)
{
        if (a) {
                a = 0;

                printc("!!!!!!!!!!!!In FreeRTOS APP!\n");
/*
                //FreeRTOS Semaphore Test
                //int a = 0, b = 0, take = 0, give = 0, i = 0;
                int i = 0;
                SemaphoreHandle_t xSemaphore;
                printc("!!!!!!!!!!!! I LIVE !!!!!!!!!!!!%d\n", xSemaphore);
                printc("%d\n", xSemaphore);
                vSemaphoreBinaryCreate(xSemaphore);

                unsigned long long a, b, take, give;

                take = give = 0;

                printc("Sem is %d\n", xSemaphore);
                printc("About to take sem\n");
                for(i; i < 100; i++) {
                        rdtscll(a);
                        xSemaphoreTake(xSemaphore, (portTickType) 0);
                        rdtscll(b);
                        take+= (b-a);
                        rdtscll(a);
                        xSemaphoreGive(xSemaphore);
                        rdtscll(b);
                        give+= (b-a);
                }
                give = give / i;
                take = take / i;
                printc("Virt Give avg = %llu\n", give);
                printc("Virt Take avg = %llu\n", take);
                */
                //FreeRTOS task test
                xTaskHandle th, th2;
                portBASE_TYPE uxQueueLength = 10, uxItemSize = sizeof(int);
                queue = xQueueCreate(uxQueueLength, uxItemSize);
                xTaskCreate(send, "Wat", (unsigned int) 4, NULL, 8, &th);
                //printc("Maybeeeee??!!?!?! %d\n", th);
                xTaskCreate(receive, "wat2", (unsigned int) 4, NULL, 7, &th2);
                //printc("Maybeeeee??!!?!?! %d\n", th);
                vTaskSuspend(NULL);
        } else {
                int ret = 0;
                printc("wooooooooooooooo! %d\n", cos_get_thd_id());
                while (frt_thd_array[ret].tid != cos_get_thd_id() && ret <= MAX_FRT_THDS)
                        ret++;
                printc("frt_thd_tid: %d ret: %d\n", frt_thd_array[ret].tid, ret);
                funcptr exe;
                exe = frt_thd_array[ret].fptr;
                exe();
                while(1);
        }
}
