/*
 * FreeRTOSConfig.h
 *
 *  Created on: Nov 3, 2026
 *      Author: udupas
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* System configuration */
#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                  1
#define configCPU_CLOCK_HZ                      16000000UL
#define configTICK_RATE_HZ                      1000U
#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                128
#define configTOTAL_HEAP_SIZE                   (16U * 1024U)
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_MALLOC_FAILED_HOOK 1

/* Memory configuration */
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configSUPPORT_STATIC_ALLOCATION         0
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Scheduler configuration */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_TIMERS                        0
#define configUSE_EVENT_GROUPS                  1
#define configUSE_STREAM_BUFFERS                1

/* Cortex-M interrupt configuration */
#define configPRIO_BITS                         4

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY        15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   5

#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8U - configPRIO_BITS))

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8U - configPRIO_BITS))

/* Cortex-M4F configuration */
#define configENABLE_FPU                         1
#define configENABLE_MPU                         0
#define configENABLE_TRUSTZONE                   0

/* FreeRTOS API configuration */
#define INCLUDE_vTaskDelay                       1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskSuspend                     1
#define INCLUDE_vTaskDelayUntil                  1
#define INCLUDE_xTaskGetSchedulerState           1
#define INCLUDE_uxTaskPriorityGet                1
#define INCLUDE_vTaskPrioritySet                 1

/* Interrupt handlers */
#define vPortSVCHandler                          SVC_Handler
#define xPortPendSVHandler                       PendSV_Handler
#define xPortSysTickHandler                      SysTick_Handler


#define configTICK_TYPE_WIDTH_IN_BITS    TICK_TYPE_WIDTH_32_BITS

#endif /* FREERTOS_CONFIG_H */
