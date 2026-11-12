#include "stm32f407xx.h"



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

void Logger_SendString(const char *str)
{
    USART_Transmit(USART2,
                   (uint8_t *)str,
                   (uint8_t)strlen(str));
}


int32_t GNSS_ConvertToInt32(float coordinate)
{
    return (int32_t)(coordinate * GNSS_SCALE_FACTOR);
}



static void GNSS_PackInt32(uint8_t *buffer,
                           uint8_t offset,
                           int32_t value)
{
    buffer[offset]     = (uint8_t)(value);
    buffer[offset + 1] = (uint8_t)(value >> 8);
    buffer[offset + 2] = (uint8_t)(value >> 16);
    buffer[offset + 3] = (uint8_t)(value >> 24);
}


void GNSS_PrepareCANMessage(GNSS_Data_t *gnss,
                            uint8_t *can_data)
{
    int32_t latitude;
    int32_t longitude;

    latitude = GNSS_ConvertToInt32(gnss->latitude);

    longitude = GNSS_ConvertToInt32(gnss->longitude);

    GNSS_PackInt32(can_data, 0, latitude);

    GNSS_PackInt32(can_data, 4, longitude);
}
