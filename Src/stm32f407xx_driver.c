/*
 * stm32f407xx_driver.c
 *
 *  Created on: Sep 25, 2026
 *      Author: udupas
 */

#include "stm32f407xx.h"
#include "math.h"


// Lookup tables according to STM32F4 Reference Manual (RM0090)
static const uint16_t AHB_PrescalerTable[16] = {
    1, 1, 1, 1, 1, 1, 1, 1, // 0..7   -> SYSCLK not divided
    2, 4, 8, 16,            // 8..11  -> SYSCLK / 2, 4, 8, 16
    64, 128, 256, 512       // 12..15 -> SYSCLK / 64, 128, 256, 512
};

static const uint8_t APB_PrescalerTable[8] = {
    1, 1, 1, 1,             // 0..3 -> HCLK not divided
    2, 4, 8, 16             // 4..7 -> HCLK / 2, 4, 8, 16
};

/**
 * @brief Function to get the APB1 Clock
 * 
 * @return uint32_t APB1 Clock frequency
 */
uint32_t RCC_GetPCLK1Val(void)
{
    uint32_t sys_clk = 0;
    uint8_t clk_src = (RCC->CFGR >> 2U) & 0x03U;

    switch (clk_src)
    {
        case 0:
            sys_clk = 16000000U; // HSI (16 MHz)
            break;
        case 1:
            sys_clk = 8000000U;  // HSE (8 MHz external crystal)
            break;
        case 2:
            // Optional: Add PLL clock retrieval if PLL is configured
            break;
        default:
            sys_clk = 16000000U;
            break;
    }

    // Extract Prescaler Register Index Bits
    uint8_t ahb_idx  = (RCC->CFGR >> 4U)  & 0x0FU;
    uint8_t apb1_idx = (RCC->CFGR >> 10U) & 0x07U;

    uint32_t ahb_pre  = AHB_PrescalerTable[ahb_idx];
    uint32_t apb1_pre = APB_PrescalerTable[apb1_idx];

    return (sys_clk / ahb_pre) / apb1_pre;
}

/**
 * @brief Function to Return the APB2 Clock
 * 
 * @return uint32_t APB2 Clock Frequency
 */

uint32_t RCC_GetPCLK2Val(void)
{
    uint32_t sys_clk = 0;
    uint8_t clk_src = (RCC->CFGR >> 2U) & 0x03U;

    switch (clk_src)
    {
        case 0:
            sys_clk = 16000000U; // HSI (16 MHz)
            break;
        case 1:
            sys_clk = 8000000U;  // HSE (8 MHz external crystal)
            break;
        case 2:
            // Optional: Add PLL clock retrieval if PLL is configured
            break;
        default:
            sys_clk = 16000000U;
            break;
    }

    // Extract Prescaler Register Index Bits
    uint8_t ahb_idx  = (RCC->CFGR >> 4U)  & 0x0FU;
    uint8_t apb2_idx = (RCC->CFGR >> 13U) & 0x07U;

    uint32_t ahb_pre  = AHB_PrescalerTable[ahb_idx];
    uint32_t apb2_pre = APB_PrescalerTable[apb2_idx];

    return (sys_clk / ahb_pre) / apb2_pre;
}
/**
 * @brief USART Initilization Function
 * 
 * @param USARTx USART Channel
 * @param USART_Conf USART Conf Struct
 */

void USART_Init(USART_RegDef_t * USARTx, USART_Conf_t USART_Conf)
{
    uint32_t USARTDIV;
    uint32_t Mantissa, Fraction, Reminder, Scaling;
    uint32_t USARTx_Clk;
    uint32_t tempreg = 0;

    // 1. Disable USART peripheral during configuration (UE = 0)
    USARTx->CR1 &= ~(1U << 13U);

    // 2. Configure CR1 parameters (Word Length, Parity, Mode, Oversampling)
    tempreg |= (USART_Conf.WordLenght << 12U);
    tempreg |= (USART_Conf.Parity << 9U);
    tempreg |= (USART_Conf.Mode << 2U);
    tempreg |= (USART_Conf.OverSampleing << 15U);
    USARTx->CR1 = tempreg; // Write clean configuration to CR1

    // 3. Configure CR2 (Stop Bits)
    USARTx->CR2 &= ~(3U << 12U); // Clear Stop Bits
    USARTx->CR2 |= (USART_Conf.StopBits << 12U);

    // 4. Get Bus Clock Frequency
    if (USARTx == USART1 || USARTx == USART6)
    {
        USARTx_Clk = RCC_GetPCLK2Val();
    }
    else
    {
        USARTx_Clk = RCC_GetPCLK1Val();
    }

    // 5. Calculate Baud Rate Register (BRR) values
    // Scaling is 16 for OVER8=0, and 8 for OVER8=1
    Scaling = 8 * (2 - USART_Conf.OverSampleing);

    // Integer part (Mantissa)
    USARTDIV = USARTx_Clk / (Scaling * USART_Conf.BaudRate);
    Mantissa = USARTDIV;

    // Fractional part rounding
    Reminder = USARTx_Clk % (Scaling * USART_Conf.BaudRate);
    Fraction = ((Reminder * 100U) + ((Scaling * USART_Conf.BaudRate) / 2U)) / (Scaling * USART_Conf.BaudRate);

    if (USART_Conf.OverSampleing == 0) // 16x Oversampling
    {
        Fraction = ((Fraction * 16U) + 50U) / 100U;
    }
    else // 8x Oversampling
    {
        Fraction = (((Fraction * 8U) + 50U) / 100U) & 0x07U; // Only 3 bits used
    }

    // 6. Write to BRR cleanly
    USARTx->BRR = 0; // Clear BRR register completely
    USARTx->BRR |= (Mantissa << 4U);
    USARTx->BRR |= (Fraction & 0x0FU);

    // 7. Enable USART Peripheral (UE = 1)
    USARTx->CR1 |= (1U << 13U);
}

