#include "utils.h"


void GNSS_PrintPosition(USART_RegDef_t *USARTx, const GNSS_Data_t *data)
{
    char buffer[128];


    int n =snprintf(buffer, sizeof(buffer),
             "Lat: %f\r\n"
             "Lon: %f\r\n"
             "Alt: %f m\r\n"
             "Fix: %u\r\n"
             "Sat: %u\r\n",
             data->latitude,
             data->longitude,
             data->altitude,
             data->fix_quality,
             data->num_satellites);



    USART_Transmit(USARTx, (uint8_t *)buffer, strlen(buffer));
}


void delay_ms(uint32_t ms)
{
	SYSTICK_LOAD= 16000 - 1;     // 1 ms @ 16 MHz
    SYSTICK_VAL  = 0;
    SYSTICK_CTRL = 5;             // Enable, processor clock, no interrupt

    for (uint32_t i = 0; i < ms; i++)
    {
        while ((SYSTICK_CTRL & (1U << 16)) == 0);
    }

    SYSTICK_CTRL = 0;
}

/**
 * @brief Function Introduses a Simulated Delay
 *
 */
void simDelay(void)
{
	uint32_t delayCounter;
	for(delayCounter=0;delayCounter<100000;delayCounter++)
	{

	}

}