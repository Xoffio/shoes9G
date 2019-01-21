/*
 * @File  demo_gpsSimpleTestn.c
 * @Info:
 *      Main code of shoes9G. shoes GPS tracker.
 * @More info: https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/c-sdk/first-code.html
 * 
 * @Author: Ricx8 
 * @Date: 01-20-2019 11:59:50 (Lunar eclipse) 
 * @Last Modified by: Ricx8
 * @Last Modified time: 01-20-2019 11:59:50
 */

#include "stdbool.h"
#include "stdint.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_hal_gpio.h"


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;


void EventDispatch(API_Event_t* pEvent){
    switch(pEvent->id){
        default:
            break;
    }
}

// Secondary task where I'm going to execute everything.
void SecondTask(void *pData){

    // GPIO configuration
    GPIO_config_t gpioLedBlue = {
        .mode         = GPIO_MODE_OUTPUT,
        .pin          = GPIO_PIN27,
        .defaultLevel = GPIO_LEVEL_LOW
    };

    GPIO_Init(gpioLedBlue); // Initialize GPIO

    while(1){
        GPIO_SetLevel(gpioLedBlue, GPIO_LEVEL_HIGH); 
        Trace(1,"## TESTING 01 ##");
        OS_Sleep(1000);
        GPIO_SetLevel(gpioLedBlue, GPIO_LEVEL_LOW);
        OS_Sleep(1000);
    }
}

// Main task
void MainTask(void *pData){
    API_Event_t* event=NULL;

    // Create another task (secondary task).
    secondTaskHandle = OS_CreateTask(SecondTask,
        NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);

    while(1){
        // Wait for events from the lower system and is handle in EventDispatch
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER)){
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

// Main entrance
void shoes9G_Main(void){
    // Create the main task. It will return the pointer os that task.
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle); // Set the main handle.
}
