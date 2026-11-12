/*
 * app_task.c
 *
 *  Created on: Nov 3, 2026
 *      Author: udupas
 */


#include "app_tasks.h"


NMEA_Parser_t nmea_parser;


void HelloTask(void *argument)
{
    (void)argument;

    while (1)
    {
        /* TODO: Transmit Hello World over UART */

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



void vBlueLEDToggleTask(void *argument)
{
    (void)argument;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 100 ms period

    while(1) {

        GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_15);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }

}
void vRedLEDToggleTask(void *argument)
{
    (void)argument;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500); // 100 ms period

    while(1) {

        GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_14);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }

}

void vLEDToggleTask(void *arguments)
{
        LedTaskConfig_t *pConfig = (LedTaskConfig_t *)arguments;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    int myPin = pConfig->GPIO_PinNumber;
    //TickType_t xFrequency = pdMS_TO_TICKS(pConfig->delayMs);
     TickType_t xFrequency = pConfig->delayMs;

    while(1) {

        GPIO_TogglePin(GPIOD,myPin);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(xFrequency));
    }
}




void GNSS_ParserTask(void *argument)
{
    uint8_t rx_data[64];
    StreamBufferHandle_t rx_stream =  (StreamBufferHandle_t)argument;

    while (1)
    {
        size_t received = xStreamBufferReceive(
            rx_stream,
            rx_data,
            sizeof(rx_data),
            portMAX_DELAY
        );

        for (size_t i = 0; i < received; i++)
        {
            NMEA_ParseByte(&nmea_parser, rx_data[i]);
        }
    }
}


void CAN_TransmitTask(void *argument)
{
    TickType_t xFrequency = 1000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {		
            CAN_Message_t can_msg;
            uint8_t can_data[8];
            
            GNSS_PrepareCANMessage(&nmea_parser.data, can_data);
			// Send CAN message
            can_msg.id = 0x123; // Example ID
			can_msg.dlc = 8;
            can_msg.data[0]=can_data[0];
            can_msg.data[1]=can_data[1];
            can_msg.data[2]=can_data[2];
            can_msg.data[3]=can_data[3];
            can_msg.data[4]=can_data[4];
            can_msg.data[5]=can_data[5];
            can_msg.data[6]=can_data[6];
            can_msg.data[7]=can_data[7];

			CAN_SendMessage(CAN1, &can_msg);	
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(xFrequency));		

    }
}

