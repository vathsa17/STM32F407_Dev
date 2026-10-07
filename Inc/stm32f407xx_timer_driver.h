#ifndef STM32F407XX_TIMER_DRIVER_H_
#define STM32F407XX_TIMER_DRIVER_H_

#include "stm32f407xx.h"

typedef struct
{
    uint16_t Period;
    uint8_t Prescaler;
    uint32_t AutoReloadPreload;
}TIM_Base_InitTypeDef;


#define TIM_AUTO_RELOAD_PRELOAD_ENABLE 1U
#define TIM_AUTO_RELOAD_PRELOAD_DISABLE 0U

#define TIM_CR1_ARPE 7U

#define TIM_EGR_UG 0U

#define TIM_SR_UIF 0U
#define TIM_CR1_CEN 0U
#define TIM6_UEV_STS() ((TIM6->SR >> TIM_SR_UIF) & 1U)
#define TIM6_UEV_CLEAR() (TIM6->SR &= ~(1U << TIM_SR_UIF))

void TIM6_Base_Init(TIM_RegDef_t *TIMx, TIM_Base_InitTypeDef TIM_BaseConf);
void TIM_Base_Start(TIM_RegDef_t *TIMx);
void TIM_Base_Stop(TIM_RegDef_t *TIMx);


#endif /* STM32F407XX_TIMER_DRIVER_H_ */
