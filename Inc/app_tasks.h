/*
 * app_tasks.h
 *
 *  Created on: Nov 3, 2026
 *      Author: udupas
 */

#ifndef APP_TASKS_H_
#define APP_TASKS_H_

#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "task.h"
#include "stm32f4xx.h"

typedef struct
{
	uint8_t GPIO_PinNumber;
	TickType_t delayMs;
}LedTaskConfig_t;



void HelloTask(void *argument);
void vBlueLEDToggleTask(void *argument);
void vRedLEDToggleTask(void *argument);
void vLEDToggleTask(void *arguments);
void GNSS_ParserTask(void *argument);
void CAN_TransmitTask(void *argument);

#endif /* APP_TASKS_H_ */
