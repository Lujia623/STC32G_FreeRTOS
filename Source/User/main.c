#include "assert.h"
#include "app_uart.h"
#include "app_cli.h"
#include "STC32G_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

static TaskHandle_t AppTaskCreate_Handle = NULL;
#define APPLICATION_STACK_SIZE             128
#define APPLICATION_PRIORITIES             configDEFAULT_PRIORITIES

static TaskHandle_t APPTask1_Handle = NULL;
#define TASK1_STACK_SIZE             128
#define TASK1_PRIORITIES             6

static TaskHandle_t APPTask2_Handle = NULL;
#define TASK2_STACK_SIZE             128
#define TASK2_PRIORITIES             5

static TaskHandle_t APPTask3_Handle = NULL;
#define TASK3_STACK_SIZE             128
#define TASK3_PRIORITIES             5

void vTimerCallback( TimerHandle_t pxTimer );

void Application(void *pvParameters);
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

int main(void) 
{
    BaseType_t xReturn = pdPASS;;

    xSystemInit();
    xprintf("main\r\n");

    xReturn = xTaskCreate((TaskFunction_t)Application,
							(const char*)"Application",
							(uint16_t)APPLICATION_STACK_SIZE,
							(void* )NULL,
							(UBaseType_t)APPLICATION_PRIORITIES,
							(TaskHandle_t*)&AppTaskCreate_Handle);
	if(pdPASS == xReturn)
		vTaskStartScheduler();
	else
		LOG_E("ERROR\r\n",0);

    while (1);
    
}

void Application(void *pvParameters)
{
    BaseType_t xReturn = pdPASS;
    TimerHandle_t xTime;

    xprintf("Application\r\n");

	xReturn = xTaskCreate((TaskFunction_t)Task1,
							(const char*)"Task1",
							(uint16_t)TASK1_STACK_SIZE,
							(void* )NULL,
							(UBaseType_t)TASK1_PRIORITIES,
							(TaskHandle_t*)&APPTask1_Handle);
    if(pdPASS != xReturn)
        LOG_E("%s ERROR\r\n", pcTaskGetName(APPTask1_Handle));

    xReturn = xTaskCreate((TaskFunction_t)Task2,
							(const char*)"Task2",
							(uint16_t)TASK2_STACK_SIZE,
							(void* )NULL,
							(UBaseType_t)TASK2_PRIORITIES,
							(TaskHandle_t*)&APPTask2_Handle);
    if(pdPASS != xReturn)
        LOG_E("%s ERROR\r\n", pcTaskGetName(APPTask2_Handle));

    xReturn = xTaskCreate((TaskFunction_t)Task3,
							(const char*)"Task3",
							(uint16_t)TASK3_STACK_SIZE,
							(void* )NULL,
							(UBaseType_t)TASK3_PRIORITIES,
							(TaskHandle_t*)&APPTask3_Handle);
    if(pdPASS != xReturn)
        LOG_E("%s ERROR\r\n", pcTaskGetName(APPTask3_Handle));

    xTime = xTimerCreate((const char*)"soft_time", 
                        (TickType_t)(5000 / configTICK_RATE_HZ), 
                        (UBaseType_t)pdTRUE, 
                        (void *)0,
                        (TimerCallbackFunction_t)vTimerCallback);
    if(NULL != xTime) {
        xTimerStart(xTime, 0);
    } else {
        LOG_E("%s ERROR\r\n", pcTaskGetName(xTime));
    }

    xprintf("AppTaskCreate_Handle: %p\r\n",AppTaskCreate_Handle);
    // xprintf("tskTCB:%d\r\n",sizeof(tskTCB));
    vTaskDelete(AppTaskCreate_Handle);
    if(AppTaskCreate_Handle != NULL)
    {
        xprintf("vTaskDelete\r\n");
    }
}

void vTimerCallback( TimerHandle_t pxTimer )
{
    static uint8_t flag = 0;

    P72 = !P72;
    xprintf("xPortGetFreeHeapSize :%0x\r\n",xPortGetFreeHeapSize());
    // if((xTaskGetTickCount() >= 100 && xTaskGetTickCount() < 300) && !flag) {
    //     flag = 1;
    //     vTaskSuspend(APPTask1_Handle);
    //     xprintf("APPTask1_Handle vTaskSuspend\r\n");
    // } else if (xTaskGetTickCount() >= 300 && flag) {
    //     flag = 0;
    //     vTaskResume(APPTask1_Handle);
    //     xprintf("APPTask1_Handle vTaskResume\r\n"); 
    // }
    
}

void Task1(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 10;
    BaseType_t xWasDelayed;

    while (1) {
        P41 = !P41;
        xprintf("Task1 xTaskGetTickCount:%ld\r\n",xTaskGetTickCount());
        xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
        if(xWasDelayed != pdTRUE)
            LOG_E("xTaskDelayUntil ERROR\r\n",0);
    }
    
}

void Task2(void *pvParameters)
{
    while (1) {
        P43 = !P43;
        xprintf("Task2 xTaskGetTickCount:%ld\r\n",xTaskGetTickCount());
        vTaskDelay(20);
    }
    
}

void Task3(void *pvParameters)
{
    while (1) {
        P20 = !P20;
        xprintf("Task3 xTaskGetTickCount:%ld\r\n",xTaskGetTickCount());
        vTaskDelay(20);
    }
    
}
