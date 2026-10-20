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

	CAN1->CAN_MCR &= ~CAN_MCR_SLEEP;

	volatile uint32_t timeout = 1000000U;
	while (CAN1->CAN_MSR & CAN_MSR_SLAK)
	{
		if (timeout-- == 0U)
		{
			/* Record/report initialization failure; don't wait forever. */
			return;
		}
	}

	CAN1->CAN_MCR |= CAN_MCR_INRQ;
while (!(CAN1->CAN_MSR & CAN_MSR_INAK))
	{
		//Wait until the Init is acknowledged
	}



	/*Initilize the CAN Communication*/
	/*Configure Bit Timing*/
	CAN1->CAN_BTR =
		((CAN_PRESCALER - 1U) << BRP_SHIFT) |
		((CAN_TS1 - 1U)       << CAN_TS1_SHIFT) |
		((CAN_TS2 - 1U)       << CAN_TS2_SHIFT) |
		((CAN_SJW - 1U)       << CAN_SJW_SHIFT);

	CAN1->CAN_FMR |= (1U << 0U);       /* Enter filter init */
	CAN1->CAN_FA1R &= ~(1U << 0U);     /* Deactivate bank 0 */
	CAN1->CAN_FM1R &= ~(1U << 0U);     /* Mask mode */
	CAN1->CAN_FS1R |= (1U << 0U);      /* 32-bit scale */
	CAN1->CAN_FFA1R &= ~(1U << 0U);    /* FIFO0 */

	CAN1->CAN_F0R1 = 0U;               /* Filter ID */
	CAN1->CAN_F0R2 = 0U;               /* Mask: accept all IDs */

	CAN1->CAN_FA1R |= (1U << 0U);      /* Activate bank 0 */
	CAN1->CAN_FMR &= ~(1U << 0U);     /* Leave filter init */


	CAN1->CAN_MCR &= ~CAN_MCR_INRQ; //Leave initialization and wait for Acknowledgment
	
		// Enable automatic bus-off recovery (ABOM, bit 6)
	CAN1->CAN_MCR |= (1U << 6U);

// Enable automatic retransmission by clearing NART (bit 4)
	CAN1->CAN_MCR &= ~(1U << 4U);
	while (CAN1->CAN_MSR & CAN_MSR_INAK)
	{
		//Wait until the Init is acknowledged
	}





}

CAN_Status_t  CAN_ReadMessage(CAN_RegDef_t * CANx, CAN_Message_t *inComingMsg)
{

	
	CAN_Status_t Status;
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

		Status=CAN_OK;

	}
	else
	{
		/*Do Nothing*/
		Status=CAN_NO_MESSAGE;
	}

	return Status;

}



CAN_Status_t CAN_SendMessage(
    CAN_RegDef_t *CANx,
    const CAN_Message_t *OutGoingMsg)
{
    uint8_t mailBox;

    if (CANx == 0 || OutGoingMsg == 0)
    {
        return CAN_TX_ERROR;
    }

    /* This implementation supports standard data frames only. */
    if (OutGoingMsg->id > 0x7FFU ||
        OutGoingMsg->dlc > 8U)
    {
        return CAN_TX_ERROR;
    }

    if (CANx->CAN_TSR & CAN_TSR_TME0)
    {
        mailBox = 0U;
    }
    else if (CANx->CAN_TSR & CAN_TSR_TME1)
    {
        mailBox = 1U;
    }
    else if (CANx->CAN_TSR & CAN_TSR_TME2)
    {
        mailBox = 2U;
    }
    else
    {
        return CAN_STATUS_BUSY;
    }

    /* Standard ID, data frame, IDE=0 and RTR=0 */
    CANx->sCAN_TxR[mailBox].CAN_TIxR =
        ((uint32_t)OutGoingMsg->id << 21U);

    CANx->sCAN_TxR[mailBox].CAN_TDTxR =
        (uint32_t)OutGoingMsg->dlc;

    uint32_t low = 0U;
    uint32_t high = 0U;

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

    CANx->sCAN_TxR[mailBox].CAN_TDLxR = low;
    CANx->sCAN_TxR[mailBox].CAN_TDHxR = high;

    /* Request transmission */
    CANx->sCAN_TxR[mailBox].CAN_TIxR |= 1U;

    /* Request queued; actual TX outcome is not known yet. */
    return CAN_OK;
}



CAN_Status_t CAN_GetTxStatus(CAN_RegDef_t * CANx)
{
	uint8_t mailBox;
	CAN_Status_t CAN_Tx_Status;
	switch(mailBox)
	{
		case 0: 
				if(CANx->CAN_TSR & CAN_TXOK0)
				{
					CAN_Tx_Status=CAN_OK;
				}
				else if(CANx->CAN_TSR & CAN_TXERR0)
				{
					CAN_Tx_Status=CAN_TX_ERROR;
				}
				break;
		case 1:
				if(CANx->CAN_TSR & CAN_TXOK1)
				{
					CAN_Tx_Status=CAN_OK;
				}
				else if(CANx->CAN_TSR & CAN_TXERR1)
				{
					CAN_Tx_Status=CAN_TX_ERROR;
				}
				break;
		case 2:
				if(CANx->CAN_TSR & CAN_TXOK2)
				{
					CAN_Tx_Status=CAN_OK;
				}
				else if(CANx->CAN_TSR & CAN_TXERR2)
				{
					CAN_Tx_Status=CAN_TX_ERROR;
				}
			break;
		default:
			CAN_Tx_Status=CAN_TX_ERROR;
			break;
	}
}
