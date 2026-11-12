/*
 * stm32f407xx_can__driver.c
 *
 *  Created on: Sep 30, 2026
 *      Author: udupas
 */
#include "stm32f407xx.h"
void CAN_Init()
{
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

	CAN1->MCR &= ~CAN_MCR_SLEEP;

	volatile uint32_t timeout = 1000000U;
	while (CAN1->MSR & CAN_MSR_SLAK)
	{
		if (timeout-- == 0U)
		{
			/* Record/report initialization failure; don't wait forever. */
			return;
		}
	}

	CAN1->MCR |= CAN_MCR_INRQ;
while (!(CAN1->MSR & CAN_MSR_INAK))
	{
		//Wait until the Init is acknowledged
	}



	/*Initilize the CAN Communication*/
	/*Configure Bit Timing*/
	CAN1->BTR =
		((CAN_PRESCALER - 1U) << BRP_SHIFT) |
		((CAN_TS1 - 1U)       << CAN_TS1_SHIFT) |
		((CAN_TS2 - 1U)       << CAN_TS2_SHIFT) |
		((CAN_SJW - 1U)       << CAN_SJW_SHIFT);

	CAN1->FMR |= (1U << 0U);       /* Enter filter init */
	CAN1->FA1R &= ~(1U << 0U);     /* Deactivate bank 0 */
	CAN1->FM1R &= ~(1U << 0U);     /* Mask mode */
	CAN1->FS1R |= (1U << 0U);      /* 32-bit scale */
	CAN1->FFA1R &= ~(1U << 0U);    /* FIFO0 */

	CAN1->sFilterRegister[0].FR1 = 0U;               /* Filter ID */
	CAN1->sFilterRegister[0].FR2 = 0U;               /* Mask: accept all IDs */

	CAN1->FA1R |= (1U << 0U);      /* Activate bank 0 */
	CAN1->FMR &= ~(1U << 0U);     /* Leave filter init */


	CAN1->MCR &= ~CAN_MCR_INRQ; //Leave initialization and wait for Acknowledgment
	
		// Enable automatic bus-off recovery (ABOM, bit 6)
	CAN1->MCR |= (1U << 6U);

// Enable automatic retransmission by clearing NART (bit 4)
	CAN1->MCR &= ~(1U << 4U);
	while (CAN1->MSR & CAN_MSR_INAK)
	{
		//Wait until the Init is acknowledged
	}





}

CAN_Status_t  CAN_ReadMessage(CAN_TypeDef  * CANx, CAN_Message_t *inComingMsg)
{

	
	CAN_Status_t Status;
	if ((CANx->RF0R & 0x03U) != 0U) //Check if the FMP0 Has pending messages
	{


		inComingMsg->id=((CANx->sFIFOMailBox[0].RIR>>21U)&0x7FF);
		inComingMsg->dlc=((CANx->sFIFOMailBox[0].RDTR)&0x0F);

		uint32_t low  = CANx->sFIFOMailBox[0].RDLR;
		uint32_t high = CANx->sFIFOMailBox[0].RDHR;

		for (uint8_t i = 0; i < inComingMsg->dlc && i < 4; i++)
		{
			inComingMsg->data[i] = (low >> (i * 8U)) & 0xFFU;
		}

		for (uint8_t i = 4; i < inComingMsg->dlc; i++)
		{
			inComingMsg->data[i] = (high >> ((i - 4U) * 8U)) & 0xFFU;
		}

		CANx->RF0R |= (1U << 5);

		Status=CAN_OK;

	}
	else
	{
		/*Do Nothing*/
		Status=CAN_NO_MESSAGE;
	}

	return Status;

}



CAN_Status_t CAN_SendMessage(CAN_TypeDef *CANx,const CAN_Message_t *OutGoingMsg)
{
    uint8_t mailBox;
    uint32_t low = 0U;
    uint32_t high = 0U;

    if ((CANx == NULL) || (OutGoingMsg == NULL))
    {
        return CAN_TX_ERROR;
    }

    /* Standard data frames only */
    if ((OutGoingMsg->id > 0x7FFU) ||
        (OutGoingMsg->dlc > 8U))
    {
        return CAN_TX_ERROR;
    }

    /* Find an available mailbox */
    uint32_t tsr = CANx->TSR;

    if (tsr & CAN_TSR_TME0)
    {
        mailBox = 0U;
    }
    else if (tsr & CAN_TSR_TME1)
    {
        mailBox = 1U;
    }
    else if (tsr & CAN_TSR_TME2)
    {
        mailBox = 2U;
    }
    else
    {
        return CAN_STATUS_BUSY;
    }

    /* Configure standard identifier */
    CANx->sTxMailBox[mailBox].TIR =
        ((uint32_t)OutGoingMsg->id << 21U);

    /* Configure DLC */
    CANx->sTxMailBox[mailBox].TDTR =
        (uint32_t)OutGoingMsg->dlc;

    /* Pack payload */
    for (uint8_t i = 0U; i < OutGoingMsg->dlc; i++)
    {
        if (i < 4U)
        {
            low |= ((uint32_t)OutGoingMsg->data[i])
                   << (8U * i);
        }
        else
        {
            high |= ((uint32_t)OutGoingMsg->data[i])
                    << (8U * (i - 4U));
        }
    }

    CANx->sTxMailBox[mailBox].TDLR = low;
    CANx->sTxMailBox[mailBox].TDHR = high;

    /* Request transmission */
    CANx->sTxMailBox[mailBox].TIR |= CAN_TI0R_TXRQ;

    return CAN_OK;
}



