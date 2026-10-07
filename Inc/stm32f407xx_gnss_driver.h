/*
 * stm32f407xx_gnss_driver.h
 *
 *  Created on: 7 Oct 2026
 *      Author: shriv
 */

#ifndef STM32F407XX_GNSS_DRIVER_H_
#define STM32F407XX_GNSS_DRIVER_H_
#include "stm32f407xx.h"

#define MAX_FIELDS       20
#define MAX_FIELD_LENGTH 20
#define CHECKSUM_LENGTH 2

typedef enum
{
    WAIT_START,
    RECEIVE_DATA,
    RECEIVE_CHECKSUM
}ParserState_t;

typedef struct
{
    ParserState_t state;

    uint8_t field_index;
    uint8_t temp_index;
    char fields[MAX_FIELDS][MAX_FIELD_LENGTH];
    char checksum[CHECKSUM_LENGTH];
    uint8_t calculated_checksum;
    uint8_t received_checksum;
    bool isValid;
} NMEA_Parser_t;


void NMEA_Init(NMEA_Parser_t *parser);



void NMEA_ParseByte(NMEA_Parser_t *parser, uint8_t byte);

#include "stm32f407xx.h"
#endif /* STM32F407XX_GNSS_DRIVER_H_ */
