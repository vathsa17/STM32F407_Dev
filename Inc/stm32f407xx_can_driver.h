/*
 * stm32f407xx_can_driver.h
 *
 *  Created on: Sep 29, 2026
 *      Author: udupas
 */

#ifndef STM32F407XX_CAN_DRIVER_H_
#define STM32F407XX_CAN_DRIVER_H_

#include "stm32f407xx.h"

#define CAN_MCR_INRQ_Pos    0U
#define CAN_MCR_INRQ        (1U << CAN_MCR_INRQ_Pos)

#define CAN_MSR_INAK_Pos    0U
#define CAN_MSR_INAK        (1U << CAN_MSR_INAK_Pos)

#define CAN_PRESCALER 2U
#define CAN_TS1 11
#define CAN_TS2 4
#define CAN_SJW 1
#define CAN_TS1_SHIFT 16U
#define CAN_TS2_SHIFT 20U
#define BRP_SHIFT 0U
#define CAN_SJW_SHIFT 24U
typedef struct
{
	volatile uint32_t CAN_MCR;
	volatile uint32_t CAN_MSR;
	volatile uint32_t CAN_TSR;
	volatile uint32_t CAN_RF0R;
	volatile uint32_t CAN_RF1R;
	volatile uint32_t CAN_IER;
	volatile uint32_t CAN_ESR;
	volatile uint32_t CAN_BTR;
	uint32_t Reserved1[88];
	volatile uint32_t CAN_TI0R;
	volatile uint32_t CAN_TDT0R;
	volatile uint32_t CAN_TDL0R;
	volatile uint32_t CAN_TDH0R;
	volatile uint32_t CAN_TI1R;
	volatile uint32_t CAN_TDT1R;
	volatile uint32_t CAN_TDL1R;
	volatile uint32_t CAN_TDH1R;
	volatile uint32_t CAN_TI2R;
	volatile uint32_t CAN_TDT2R;
	volatile uint32_t CAN_TDL2R;
	volatile uint32_t CAN_TDH2R;
	volatile uint32_t CAN_RI0R;
	volatile uint32_t CAN_RDT0R;
	volatile uint32_t CAN_RDL0R;
	volatile uint32_t CAN_RDH0R;
	volatile uint32_t CAN_RI1R;
	volatile uint32_t CAN_RDT1R;
	volatile uint32_t CAN_RDL1R;
	volatile uint32_t CAN_RDH1R;
	uint32_t Reserved2[12];
	volatile uint32_t CAN_FMR;         // 0x200
	volatile uint32_t CAN_FM1R;        // 0x204
	uint32_t Reserved3;                // 0x208

	volatile uint32_t CAN_FS1R;        // 0x20C
	uint32_t Reserved4;                // 0x210

	volatile uint32_t CAN_FFA1R;       // 0x214
	uint32_t Reserved5;                // 0x218

	volatile uint32_t CAN_FA1R;        // 0x21C

	uint32_t Reserved6[8];             // 0x220 - 0x23C

	volatile uint32_t CAN_F0R1;        // 0x240
	volatile uint32_t CAN_F0R2;        // 0x244
	volatile uint32_t CAN_F1R1;        // 0x248
	volatile uint32_t CAN_F1R2;        // 0x24C





}CAN_RegDef_t;


typedef struct
{
    uint32_t id;
    uint8_t  dlc;
    uint8_t  data[8];
} CAN_Message_t;

void CAN_Init(void);
void CAN_ReadMessage(CAN_RegDef_t *CANx, CAN_Message_t *msg);

#endif /* STM32F407XX_CAN_DRIVER_H_ */
