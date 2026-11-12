#ifndef UTILS_H
#define UTILS_H

#include "stm32f407xx.h"
#include "string.h"
#define GNSS_SCALE_FACTOR   10000000.0f


typedef enum
{
    BOOTLOADER_MODE_CMD,
    BOOTLOADER_MODE_METADATA,
    BOOTLOADER_MODE_FIRMWARE
} Bootloader_Mode_t;



void simDelay(void);
void Logger_SendString(const char *str);
int32_t GNSS_ConvertToInt32(float coordinate);
void GNSS_PrepareCANMessage(GNSS_Data_t *gnss,uint8_t *can_data);



#endif /* UTILS_H */
