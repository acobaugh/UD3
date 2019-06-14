/*
 * UD3
 *
 * Copyright (c) 2018 Jens Kerrinnes
 * Copyright (c) 2015 Steve Ward
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "cyapicallbacks.h"
#include <cytypes.h>

#include "tsk_fault.h"


/* RTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "ZCDtoPWM.h"

xTaskHandle tsk_fault_TaskHandle;
uint8 tsk_fault_initVar = 0u;

xQueueHandle qAlarms;



/* ------------------------------------------------------------------------ */
/*
 * Place user included headers, defines and task global data in the
 * below merge region section.
 */
/* `#START USER_INCLUDE SECTION` */
#include "cli_common.h"
#include "telemetry.h"
#include "tsk_priority.h"
#include "tsk_analog.h"

/* `#END` */
/* ------------------------------------------------------------------------ */
/*
 * User defined task-local code that is used to process commands, control
 * operations, and/or generrally do stuff to make the taks do something
 * meaningful.
 */
/* `#START USER_TASK_LOCAL_CODE` */

TimerHandle_t xWD_Timer;

void alarm_push(uint8_t level, const char* message){
    ALARMS temp;
    if(uxQueueSpacesAvailable(qAlarms)==0){
        xQueueReceive(qAlarms,&temp,0);
    }
    temp.alarm_level = level;
    temp.message = message;
    temp.timestamp = xTaskGetTickCount() * portTICK_RATE_MS;
    xQueueSend(qAlarms,&temp,0);   
}

uint32_t alarm_get_num(){
    return uxQueueMessagesWaiting(qAlarms);    
}

uint32_t alarm_get(uint32_t index, ALARMS * alm){
    if(uxQueueMessagesWaiting(qAlarms)){
        xQueuePeekIndex(qAlarms, alm,0,index);
        return pdPASS;
    }else{
        return pdFAIL;
    }
}

void alarm_clear(){
    xQueueReset(qAlarms);    
}


void WD_enable(uint8_t enable){
    if(xWD_Timer!=NULL){
        if(enable){
            xTimerStart(xWD_Timer,10);
        }else{  
            xTimerStop(xWD_Timer,10);
        }
    } 
}

void WD_reset(){
    if(xWD_Timer!=NULL && configuration.watchdog){
        xTimerStart(xWD_Timer,0);
    }
}


void handle_UVLO(void) {
	//UVLO feedback via system_fault (LED2)
	if(UVLO_status_Status==0){
        if(telemetry.sys_fault[SYS_FAULT_UVLO]==0){
            alarm_push(ALM_PRIO_CRITICAL,warn_driver_undervoltage);
        }
        telemetry.sys_fault[SYS_FAULT_UVLO]=1;
    }else{
        telemetry.sys_fault[SYS_FAULT_UVLO]=0;
    }
    
    /*
	if ((bus_command == BUS_COMMAND_FAULT) || (telemetry.uvlo_stat == 0) || (telemetry.bus_status == BUS_CHARGING)) {
		system_fault_Control = 0;
	} else {
		system_fault_Control = 1;
	}
    */
}

void handle_FAULT(void) {
	//UVLO feedback via system_fault (LED2)
	uint8_t flag=1;
    for(uint8_t i=0;i<SYS_FAULT_NUM;i++){
        if(telemetry.sys_fault[i]) flag = 0;
    }
    system_fault_Control = flag;
}

void handle_no_fb(void){
    uint32_t time = ((uint32_t)fb_filter_out * 15625ul)/1000; //ns
    telemetry.fres = (5000000ul / time);
}

void vWD_Timer_Callback(TimerHandle_t xTimer){
    telemetry.sys_fault[SYS_FAULT_WD] = 1;
}

/* `#END` */
/* ------------------------------------------------------------------------ */
/*
 * This is the main procedure that comprises the task.  Place the code required
 * to preform the desired function within the merge regions of the task procedure
 * to add functionality to the task.
 */
void tsk_fault_TaskProc(void *pvParameters) {
	/*
	 * Add and initialize local variables that are allocated on the Task stack
	 * the the section below.
	 */
	/* `#START TASK_VARIABLES` */

	/* `#END` */

	/*
	 * Add the task initialzation code in the below merge region to be included
	 * in the task.
	 */
	/* `#START TASK_INIT_CODE` */
    
    xWD_Timer = xTimerCreate("WD-Tmr", 500 / portTICK_PERIOD_MS, pdFALSE,(void * ) 0, vWD_Timer_Callback);
    WD_enable(configuration.watchdog);
    
    alarm_push(ALM_PRIO_INFO,warn_task_fault);
	/* `#END` */

	for (;;) {
		/* `#START TASK_LOOP_CODE` */
		handle_UVLO();
        handle_FAULT();
        rx_blink_Write(0);
        handle_no_fb();
		/* `#END` */

		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
}
/* ------------------------------------------------------------------------ */
void tsk_fault_Start(void) {
	/*
	 * Insert task global memeory initialization here. Since the OS does not
	 * initialize ANY global memory, execute the initialization here to make
	 * sure that your task data is properly 
	 */
	/* `#START TASK_GLOBAL_INIT` */
    
    qAlarms = xQueueCreate(50, sizeof(ALARMS));

	/* `#END` */

	if (tsk_fault_initVar != 1) {

		/*
	 	* Create the task and then leave. When FreeRTOS starts up the scheduler
	 	* will call the task procedure and start execution of the task.
	 	*/
		xTaskCreate(tsk_fault_TaskProc, "Fault-Svc", STACK_FAULT, NULL, PRIO_FAULT, &tsk_fault_TaskHandle);
		tsk_fault_initVar = 1;
	}
}
/* ------------------------------------------------------------------------ */
/* ======================================================================== */
/* [] END OF FILE */
