/*
 * stm32f407xx_can__driver.c
 *
 *  Created on: Sep 30, 2026
 *      Author: udupas
 */
#include "stm32f407xx.h"
void CAN_Init()
{
	CAN1_ENB(); //Enable the CAN1 clock in the RCC

	CAN1->CAN_MCR |= CAN_MCR_INRQ; //Request initialization and wait for Acknowledgment
	while (!(CAN1->CAN_MSR & CAN_MSR_INAK))
	{
		//Wait until the Init is acknowledged
	}



	/*Initilize the CAN Communication*/
	/*Configure Bit Timing*/
	CAN1->CAN_BTR|=((CAN_PRESCALER-1)<<BRP_SHIFT);
	CAN1->CAN_BTR|=((CAN_TS1-1)<<CAN_TS1_SHIFT);
	CAN1->CAN_BTR|=((CAN_TS2-1)<<CAN_TS2_SHIFT);
	CAN1->CAN_BTR|=((CAN_SJW-1)<<CAN_SJW_SHIFT);

	CAN1->CAN_FMR|=(1U<<0U); /*Filter Init*/
	CAN1->CAN_FM1R &=~(1<<0); /*We want to use the first filter bank with mask mode. Hence explicitly bit is 0*/
	CAN1->CAN_FS1R |= (1U << 0);/*1 32 Bit Scale*/
	CAN1->CAN_FFA1R &= ~(1U << 0);   // Bank 0 → FIFO0
	CAN1->CAN_FA1R |= (1U << 0); //Activate Bank0
	CAN1->CAN_F0R1 = 0x00000000; // Filter ID
	CAN1->CAN_F0R2 = 0x00000000; // Mask
	CAN1->CAN_FMR &=~(1U<<0U); /*Filter Init*/


	CAN1->CAN_MCR &= ~CAN_MCR_INRQ; //Leave initialization and wait for Acknowledgment
	while (CAN1->CAN_MSR & CAN_MSR_INAK)
	{
		//Wait until the Init is acknowledged
	}

}

void CAN_ReadMessage(CAN_RegDef_t * CANx, CAN_Message_t *inComingMsg)
{

	if ((CANx->CAN_RF0R & 0x03U) != 0U) //Check if the FMP0 Has pending messages
	{


		inComingMsg->id=((CANx->CAN_RI0R>>21U)&0x7FF);
		inComingMsg->dlc=((CANx->CAN_RDT0R)&0x0F);

		uint32_t low  = CANx->CAN_RDL0R;
		uint32_t high = CANx->CAN_RDH0R;

		for (uint8_t i = 0; i < inComingMsg->dlc && i < 4; i++)
		{
			inComingMsg->data[i] = (low >> (i * 8U)) & 0xFFU;
		}

		for (uint8_t i = 4; i < inComingMsg->dlc; i++)
		{
			inComingMsg->data[i] = (high >> ((i - 4U) * 8U)) & 0xFFU;
		}

		CANx->CAN_RF0R |= (1U << 5);

	}
	else
	{
		/*Do Nothing*/
	}



}

