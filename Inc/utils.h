#ifndef UTILS_H
#define UTILS_H

#include "stm32f407xx.h"

void GNSS_PrintPosition(USART_RegDef_t *USARTx, const GNSS_Data_t *data);
void delay_ms(uint32_t ms);
void simDelay(void);


#endif /* UTILS_H */
