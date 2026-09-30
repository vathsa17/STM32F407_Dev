/*
 * stm32f407_usart_driver.c
 *
 *  Created on: Sep 27, 2026
 *      Author: udupas
 */
#include "stm32f407xx.h"

/**
 * @brief Function to transmit the USART Frames
 * 
 * @param USARTx The Selected USART Channel
 * @param Mess The Message
 * @param MessSize The Message Lenght
 */
void USART_Transmit(USART_RegDef_t * USARTx, uint8_t * Mess, uint8_t MessSize)
{
	uint8_t *pDataBit;
	uint16_t *pData16Bit;
	uint8_t TxCounter;

	TxCounter=MessSize;

	/*If the Wordlength is 9B and Parity is None*/
	if((((USARTx->CR1>>12U)&0x01) == USART_WORDLENGTH_9B) \
			&& (((USARTx->CR1>>9U)&0x01) == USART_PARITY_NONE))
	{
		pDataBit=NULL;
		pData16Bit = (uint16_t *) Mess;
	}
	else /*Else data is 8B*/
	{
		pDataBit=Mess;
		pData16Bit=NULL;


	}

	while(TxCounter > 0)
		{
			// 1. Wait until TXE (bit 7) becomes 1
			while(!((USARTx->SR >> 7U) & 0x01))
			{
				// Wait for transmit data register to empty
			}

			// 2. Write data to DR register
			if(pDataBit == NULL)
			{
				USARTx->DR = (uint16_t)(*pData16Bit & 0x01FF);
				pData16Bit++;
			}
			else
			{
				USARTx->DR = (uint8_t)(*pDataBit & 0x00FF);
				pDataBit++;
			}

			// 3. Decrement only after successful write
			TxCounter--;
		}
}

/**
 * @brief Function to recieve the USART Frames
 * 
 * @param USARTx Desired USART Channel
 * @param Mess Message
 * @param MessSize Message Size
 */

void USART_Recieve(USART_RegDef_t * USARTx, uint8_t * Mess, uint8_t MessSize)
{
	uint8_t RxBufferCounter = MessSize;
	uint8_t * pData8bit;
	uint16_t * pData16bit;

	if ((((USARTx->CR1 >> 12U) & 0x01) == USART_WORDLENGTH_9B) &&
	    (((USARTx->CR1 >> 9U) & 0x01) == USART_PARITY_NONE))
	{
		pData8bit = NULL;
		pData16bit = (uint16_t *)Mess;
	}
	else
	{
		pData8bit = Mess;
		pData16bit = NULL;
	}

	while (RxBufferCounter > 0)
	{
		// 1. BLOCKING WAIT: Pause here until RXNE (bit 5) is set by hardware
		while (!((USARTx->SR >> 5U) & 0x01))
		{
			// Waiting for incoming byte...
		}

		// 2. Read received byte from Data Register
		if (pData8bit == NULL)
		{
			*pData16bit = (uint16_t)(USARTx->DR & 0x01FF);
			pData16bit++;
		}
		else
		{
			if ((((USARTx->CR1 >> 12U) & 0x01) == USART_WORDLENGTH_9B) ||
			    ((((USARTx->CR1 >> 12U) & 0x01) == USART_WORDLENGTH_8B) && (((USARTx->CR1 >> 9U) & 0x01) == USART_PARITY_NONE)))
			{
				*pData8bit = (uint8_t)(USARTx->DR & 0x00FF);
			}
			else
			{
				*pData8bit = (uint8_t)(USARTx->DR & 0x007F);
			}
			pData8bit++;
		}

		// 3. Decrement counter ONLY after a byte has actually been read
		RxBufferCounter--;
	}
}
