#include "stm32f407xx.h" 

#define MAX_FIELDS       20
#define MAX_FIELD_LENGTH 20

/*Helper Functions*/

float NMEA_ConvertLatitude(
    const char *latitude,
    char direction
)
{
    float raw = atof(latitude);

    float latitude_degrees = floorf(raw / 100.0f);
    float latitude_minutes = raw -
                             (latitude_degrees * 100.0f);

    float decimal_latitude =
        latitude_degrees +
        (latitude_minutes / 60.0f);

    if (direction == 'S')
    {
        decimal_latitude = -decimal_latitude;
    }

    return decimal_latitude;
}

float NMEA_ConvertLongitude(
    const char *longitude,
    char direction
)
{
    float raw = atof(longitude);

    float longitude_degrees = floorf(raw / 100.0f);
    float longitude_minutes = raw -
                               (longitude_degrees * 100.0f);

    float decimal_longitude =
        longitude_degrees +
        (longitude_minutes / 60.0f);

    if (direction == 'W')
    {
        decimal_longitude = -decimal_longitude;
    }

    return decimal_longitude;
}

void NMEA_ResetParser(NMEA_Parser_t *parser)
{
    parser->state = WAIT_START;
    parser->field_index = 0;
    parser->temp_index = 0;
    parser->calculated_checksum = 0;
    parser->received_checksum = 0;


}


uint8_t HexToValue(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    return 0xFF;   // invalid hex character
}



void NMEA_ParseByte(NMEA_Parser_t *parser, uint8_t byte)
{
    switch (parser->state)
    {
        case WAIT_START:

                    if (byte == '$')
                    {
                        parser->state = RECEIVE_DATA;
                        parser->field_index = 0;
                        parser->temp_index = 0;
                        parser->calculated_checksum = 0;
                    }

                    break;


        case RECEIVE_DATA:

                    if (byte == ',')
                    {
                        parser->fields[parser->field_index][parser->temp_index] = '\0';
                        parser->field_index++;
                        parser->temp_index = 0;
                        parser->calculated_checksum ^= byte;
                        
                    }

                    else if (byte == '*')
                    {
                        parser->fields[parser->field_index][parser->temp_index] = '\0';                        
                        parser->state = RECEIVE_CHECKSUM;
                        parser->temp_index = 0;
                    }

                    else
                    {
                        if (parser->temp_index < sizeof(parser->fields[parser->field_index]) - 1)
                        {
                            parser->fields[parser->field_index][parser->temp_index++]= byte;
                        }
                        else
                        {
                            /* Field too long */
                            parser->state = WAIT_START;
                        }

                        parser->calculated_checksum ^= byte;
                    }

                    break;


        case RECEIVE_CHECKSUM:

                    /*
                    * Receive two hexadecimal characters here.
                    */
                    if (byte == '\r')
                    {
                        // optionally wait for '\n'
                    }
                    else if (byte == '\n')
                    {
                        if (parser->temp_index == 2)
                        {
                                // convert and validate
                                parser->received_checksum = 0;
                                parser->received_checksum = HexToValue(parser->checksum[0]) << 4;
                                parser->received_checksum |= HexToValue(parser->checksum[1]);
                

                            if (parser->calculated_checksum ==  parser->received_checksum)
                            {
                                /* Valid NMEA sentence */
                            	
                                    // Process each field as needed

                                    if (strcmp(parser->fields[0], "GNGGA") == 0)
                                    {
                                        ParseGGA(parser);
                                        
                                    }
                                    else if (strcmp(parser->fields[0], "GNGSA") == 0)
                                    {
                                        //ParseGSA(parser);
                                    }
                                    else if (strcmp(parser->fields[0], "GNRMC") == 0)
                                    {
                                       // ParseRMC(parser);
                                    }
                                    else
                                    {
                                        // Unknown sentence type
                                    }
                                
                            }
                            else
                            {
                                /* Checksum error */
                            	parser->isValid=FALSE;
                            }

                            NMEA_ResetParser(parser);

                        }

                    }   
                        
                    else
                    {  
                        if (parser->temp_index < CHECKSUM_LENGTH)
                            {
                                parser->checksum[parser->temp_index++] = byte;
                            }
                            else
                            {
                                /* Field too long */
                                parser->state = WAIT_START;
                            }
                    }
                    
                    break;
    }
}


void ParseGGA(NMEA_Parser_t *parser)
{
    // Example: $GNGGA,123456.00,3723.2475,N,12158.3416,W,1,08,0.9,545.4,M,46.9,M,,*47

    GNSS_Data_t *data = &parser->data;
    // Extract relevant fields

    if (parser->fields[2][0] != '\0' && parser->fields[4][0] != '\0')
    {

       parser->isValid=TRUE;
        data->latitude =NMEA_ConvertLatitude(parser->fields[2], parser->fields[3][0]);
        data->longitude =NMEA_ConvertLongitude(parser->fields[4], parser->fields[5][0]);
    	//data->latitude = 37.387458f;
    	//data->longitude = -121.97236f;

        // parser->fields[11] -> Geoidal Separation
        // parser->fields[12] -> Geoidal Separation Units (M)

        // parser->fields[6] -> Fix Quality
        data->fix_quality = (uint8_t)atoi(parser->fields[6]);

        // parser->fields[7] -> Number of Satellites
        data->num_satellites = (uint8_t)atoi(parser->fields[7]);

        // parser->fields[8] -> Horizontal Dilution of Precision (HDOP)
        data->hdop = (float)atof(parser->fields[8]);

        // parser->fields[9] -> Altitude
        data->altitude = (float)atof(parser->fields[9]);
        // parser->fields[10] -> Altitude Units (M)

        //data->geoidal_separation = (float)atof(parser->fields[11]);
    }
    else
    {
        parser->isValid = FALSE; // Invalid data, set isValid to FALSE
        NMEA_ResetParser(parser);
    }
    
}
