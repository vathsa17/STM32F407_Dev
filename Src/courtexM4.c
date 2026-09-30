/*
 * courtexM4.c
 *
 *  Created on: Sep 24, 2026
 *      Author: udupas
 */
#include "cortexM4.h"

/**
 * @brief Function to set the Interrupt Priority in NVIC
 * 
 * @param IRQNumber Interrupt Request Number
 * @param IT_Priority Desired Interrupt Priorty
 */
void NVIC_SetPriority(uint8_t IRQNumber,uint8_t IT_Priority)
{
	uint8_t index, bitField;

	/*NVIC IPR Register Index*/
	index=IRQNumber/4U;
	bitField=(IRQNumber%4U)*8U;
	NVIC->IPR[index]&=~(IT_Priority<<(bitField+4));
	NVIC->IPR[index]|=(IT_Priority<<(bitField+4));
}

/**
 * @brief Function to set the Interrept request enable in NVIC
 * 
 * @param IRQNumber The IRQ Number to be enabled
 */
void NVIC_EnableIRQ(uint8_t IRQNumber)
{
	uint8_t index,bitPos;

	index=IRQNumber/32;
	bitPos=IRQNumber%32;
	NVIC->ISER[index] |= (0x01<<bitPos);
}

