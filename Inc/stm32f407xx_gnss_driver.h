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
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;

}UTCTime_t;

typedef struct
{
    UTCTime_t utc_time;
    float latitude;
    float longitude;
    uint8_t fix_quality;
    uint8_t num_satellites;
    float altitude;
    uint8_t hdop;
    float geoidal_separation;

}GNSS_Data_t;


typedef struct
{
    ParserState_t state;

    uint8_t field_index;
    uint8_t temp_index;
    char fields[MAX_FIELDS][MAX_FIELD_LENGTH];
    char checksum[CHECKSUM_LENGTH];
    uint8_t calculated_checksum;
    uint8_t received_checksum;
    volatile bool isValid;

    GNSS_Data_t data;
} NMEA_Parser_t;


void ParseGGA(NMEA_Parser_t *parser);
void ParseGSA(NMEA_Parser_t *parser);
void ParseRMC(NMEA_Parser_t *parser);
void NMEA_ResetParser(NMEA_Parser_t *parser);



void NMEA_ParseByte(NMEA_Parser_t *parser, uint8_t byte);

void ProcessNMEASentence(NMEA_Parser_t *parser);

#include "stm32f407xx.h"
#endif /* STM32F407XX_GNSS_DRIVER_H_ */
