/*
 * References:
 * (1) https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/examples/template_configuration/FreeRTOSConfig.h
 * (2) https://github.com/raspberrypi/FreeRTOS-Kernel/tree/main/portable/ThirdParty/GCC/RP2350_ARM_NTZ
 * (3) https://github.com/fred13kim/hall-effect-keypad/blob/dev/include/FreeRTOSConfig.h
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// use 32bit TickType_t
#define configTICK_TYPE_WIDTH_IN_BITS TICK_TYPE_WIDTH_32_BITS

// enable Floating Point Unit (FPU)
#define configENABLE_FPU 1

// enable Memory Protection Unit (MPU)
// this must be 0 (2)
#define configENABLE_MPU 0

// we don't have anything to protect
// the port does not even support this
#define configENABLE_TRUSTZONE 0

// the call stack
#define configSYSTEM_CALL_STACK_SIZE 256
#define configMINIMAL_STACK_SIZE     256

// during the development, build with
// cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
// assert() will be ignored otherwise
#include <assert.h>
#define configASSERT(x) assert(x)
// behavior: prints over serial

// this should be good enough?
#define configMAX_PRIORITIES 32

#define configUSE_PREEMPTION        1
#define configUSE_PASSIVE_IDLE_HOOK 0
#define configUSE_TICK_HOOK         0
#define configUSE_IDLE_HOOK         0

// tested value for this (2)
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 16
#define configRUN_FREERTOS_SECURE_ONLY       1

#define configTICK_RATE_HZ ((TickType_t)1000)

// these are required if we want to use APIs that are required
#define configUSE_TIMERS             1
#define configTIMER_TASK_PRIORITY    (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH     10
#define configTIMER_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE

// Set the following INCLUDE_* constants to 1 to include the named API function,
// or 0 to exclude the named API function.  Most linkers will remove unused
// functions even when the constant is 1.
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_xTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_uxTaskGetStackHighWaterMark 0
#define INCLUDE_xTaskGetIdleTaskHandle      0
#define INCLUDE_eTaskGetState               0
#define INCLUDE_xTimerPendFunctionCall      1
#define INCLUDE_xTaskAbortDelay             0
#define INCLUDE_xTaskGetHandle              0
#define INCLUDE_xTaskResumeFromISR          1

#define configTOTAL_HEAP_SIZE (128 * 1024)

#endif
