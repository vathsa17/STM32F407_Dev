#include "stm32f407xx.h" 

#define MAX_FIELDS       20
#define MAX_FIELD_LENGTH 20

void NMEA_Init(NMEA_Parser_t *parser)
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
                            	parser->isValid=TRUE;
                            	for (uint8_t i = 0; i <= parser->field_index; i++)
                                {
                                    // Process each field as needed

                                    //ProcessNMEAField(parser->fields[i]); // Implement this function as needed
                                }
                            }
                            else
                            {
                                /* Checksum error */
                            	parser->isValid=FALSE;
                            }

                            NMEA_Init(parser);

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
