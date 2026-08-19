#pragma once

#include <assert.h>
#include <stdint.h>

#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                   1
#define configUSE_16_BIT_TICKS                   0
#define configTICK_RATE_HZ                       ((TickType_t)1000)
#define configMAX_PRIORITIES                     5
#define configMINIMAL_STACK_SIZE                 ((uint16_t)256)
#define configMAX_TASK_NAME_LEN                  16
#define configTOTAL_HEAP_SIZE                    ((size_t)(64 * 1024))
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configSUPPORT_STATIC_ALLOCATION          0
#define configUSE_MUTEXES                        1
#define configQUEUE_REGISTRY_SIZE                0
#define configUSE_TIMERS                         0
#define configUSE_COUNTING_SEMAPHORES            0
#define configUSE_RECURSIVE_MUTEXES               0
#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0
#define configCHECK_FOR_STACK_OVERFLOW           0
#define configUSE_MALLOC_FAILED_HOOK             0
#define configUSE_TRACE_FACILITY                 0
#define configGENERATE_RUN_TIME_STATS            0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0
#define configMESSAGE_BUFFER_LENGTH_TYPE         size_t
#define configASSERT(x)                          assert(x)

#define INCLUDE_vTaskDelay                       1
#define INCLUDE_vTaskDelayUntil                  1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskSuspend                     1
#define INCLUDE_xTaskGetSchedulerState           1
