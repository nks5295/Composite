/*
    FreeRTOS V7.3.0 - Copyright (C) 2012 Real Time Engineers Ltd.

    FEATURES AND PORTS ARE ADDED TO FREERTOS ALL THE TIME.  PLEASE VISIT 
    http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!
    
    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    
    http://www.FreeRTOS.org - Documentation, training, latest versions, license 
    and contact details.  
    
    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool.

    Real Time Engineers ltd license FreeRTOS to High Integrity Systems, who sell 
    the code with commercial support, indemnification, and middleware, under 
    the OpenRTOS brand: http://www.OpenRTOS.com.  High Integrity Systems also
    provide a safety engineered and independently SIL3 certified version under 
    the SafeRTOS brand: http://www.SafeRTOS.com.
*/

#include "FreeRTOS.h"
#include "task.h"
#include <jw_freertos.h>
#include <cos_types.h>
#include <inttypes.h>

#define MAX_NUMBER_OF_TASKS 256
#define CHECKPOINT_INTERVAL 16

#define portSAVE_CONTEXT()

#define portDISABLE_INTERRUPTS() vPortDisableInterrupts()

#define portENABLE_INTERRUPTS() vPortEnableInterrupts()

typedef struct taskMapping
{
	int thd_id;
	xTaskHandle task;
} taskMapping_t;

taskMapping_t taskMappings[MAX_NUMBER_OF_TASKS];

typedef struct XPARAMS 
{
	pdTASK_CODE pxCode;
	void *pvParams;
} xParams;

xParams *task_thread_params[MAX_NUMBER_OF_TASKS];

static volatile int lastCreatedTask;

static volatile portBASE_TYPE xInterruptsEnabled = pdTRUE;
static volatile portBASE_TYPE xPendingYield = pdFALSE;

volatile uint64_t ticks; 
volatile uint64_t wakeup_time;
volatile uint64_t child_wakeup_time;

static int freertos_timer_thread = -1;
//static int scheduler_done = 0;


/* 
 * Opposite to portSAVE_CONTEXT().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer. 
 */

#define portRESTORE_CONTEXT()

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

int prvGetThreadHandle( xTaskHandle task_handle) {
	int i;
	for (i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
		if (taskMappings[i].task == task_handle) {
			return taskMappings[i].thd_id;
		}
	}
	freertos_print("NO TASK FOUND FOR THE TASK HANDLE\n");
	return 0;
}

void vPortCosSwitchThread(int flags) {

	freertos_lock();

	prvGetThreadHandle(xTaskGetCurrentTaskHandle());
	
	vTaskSwitchContext();
	
	int next_thd = prvGetThreadHandle(xTaskGetCurrentTaskHandle());
        freertos_print("About to switch to %d\n", next_thd);
        freertos_print("should twerk: %d\n", xTaskGetCurrentTaskHandle());
	
	freertos_unlock();

	if (freertos_get_thread_id() != next_thd) {
				//freertos_print("Switching from thd %d to thd %d\n", freertos_get_thread_id(), next_thd);	
		freertos_switch_thread(next_thd, flags);
	}
}

/*
 * Manual context switch.  
 */
void vPortYield( void )
{
	vPortCosSwitchThread(0);
	return;
}
/*-----------------------------------------------------------*/

// Neel Shah
// I changed line 176 to xTaskIncrementTick() since the name changed in v8
/*
 * Context switch function used by the tick.  This must be identical to 
 * vPortYield() from the call to vTaskSwitchContext() onwards.  The only
 * difference from vPortYield() is the tick count is incremented as the
 * call comes from the tick ISR.
 */
void vPortYieldFromTick( void )
{
	/* Save context, increment tick (vtaskincrementtick()), switch context, restore context */
	xTaskIncrementTick();
	freertos_clear_pending_events();
	//Need to fix!
        freertos_print("About to switch\n");
        vPortCosSwitchThread(0);
	return;
}
/*-----------------------------------------------------------*/


/*
 * Perform hardware setup to enable ticks from timer 1, compare match A.
 */
static void prvSetupTimerInterrupt( void );
/*-----------------------------------------------------------*/

extern int
parent_sched_child_timer_int(spdid_t spdid, int idle, unsigned long wake_diff);

extern void
timer_tick( void );

void timer_init(void) {
	freertos_print("Timer init called...\n");
	ticks = 0;
	wakeup_time = 0;
	child_wakeup_time = 0;
        freertos_print("Calling timer_tick from timer_init\n");
	timer_tick();

	// should never get here
	freertos_print("ERROR ERROR ERROR IN TIMER_INIT: TIMER_TICK() RETURNED\n");
	while(1);

}

void *prvWaitForStart( void *pvParams) {
	freertos_print("In prvWaitForStart\n");
	if (freertos_get_thread_id() == freertos_timer_thread) {
		printc("nerp\n");
                timer_init();
	}

	xParams *pxParams = task_thread_params[freertos_get_thread_id()];
	pdTASK_CODE pvCode = pxParams->pxCode;
	void *pParams = pxParams->pvParams;
	
	freertos_print("Running task's function!\n");
	pvCode (pParams);
	
	return (void *) NULL;
}

/* 
 * See header file for description. 
 */
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
	
	xParams *pxThisThreadParams = pvPortMalloc( sizeof(xParams));
	pxThisThreadParams->pxCode = pxCode;
	pxThisThreadParams->pvParams = pvParameters;

	freertos_lock();

	int thd_id = freertos_create_thread((int) prvWaitForStart, (int) pxThisThreadParams, 0);

	task_thread_params[thd_id] = pxThisThreadParams;
	lastCreatedTask = thd_id;

	freertos_print("FreeRTOS started thd %d\n", thd_id);
	
	freertos_unlock();

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

void pvAddTaskMapping (int thread_id, xTaskHandle task_handle) {
	// FIXME: should make this mapping more dynamic.
	taskMappings[thread_id].thd_id = thread_id;
	taskMappings[thread_id].task = task_handle;
}

void pvAddTaskHandle (void *pxTaskHandle) {
	freertos_print("Adding task handle, thread id: %d\n", lastCreatedTask);
	pvAddTaskMapping((int) lastCreatedTask, (xTaskHandle) pxTaskHandle);
}

/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* Probably don't need this. */
}


/*
 * Setup timer 1 compare match A to generate a tick interrupt.
 */
static void prvSetupTimerInterrupt( void )
{
	freertos_timer_thread = create_timer((int) timer_init);
	return;
}

void vPortDisableInterrupts(void) {
	xInterruptsEnabled = pdFALSE;
}

void vPortEnableInterrupts(void) {
	xInterruptsEnabled = pdTRUE;
	
}

portBASE_TYPE xPortStartScheduler( void )
{
	// need to call prvSetupTimerInterrupt()
	// and then restore the context of the first task that is going to run
	// and start it.
	freertos_print("Starting freeRTOS scheduler\n");
	vPortEnableInterrupts();
	prvSetupTimerInterrupt();
	
	freertos_print("freertos: Switching to timer thread...\n");
	freertos_switch_thread(freertos_timer_thread, 0);

	freertos_print("Switched back to main from timer thread.\n");

	/* int first_thread = prvGetThreadHandle(xTaskGetCurrentTaskHandle()); */
	/* freertos_print("First thread: %d\n", first_thread); */
	/* freertos_switch_thread(first_thread, 0); */

	return 0;
}
