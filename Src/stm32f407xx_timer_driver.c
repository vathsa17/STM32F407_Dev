#include "stm32f407xx_timer_driver.h"

void TIM_Base_Init(TIM_RegDef_t *TIMx, TIM_Base_InitTypeDef TIM_BaseConf)
{
    TIMx->PSC = TIM_BaseConf.Prescaler;
    TIMx->ARR = TIM_BaseConf.Period;
    if (TIM_BaseConf.AutoReloadPreload == TIM_AUTO_RELOAD_PRELOAD_ENABLE)
    {
        TIMx->CR1 |= (1U << TIM_CR1_ARPE);
    }
    else
    {
        TIMx->CR1 &= ~(1U << TIM_CR1_ARPE);
    }
    TIMx->EGR |= (1U << TIM_EGR_UG);
    TIMx->SR &= ~(1U << TIM_SR_UIF);
}

void TIM6_Base_Init(TIM_RegDef_t *TIMx, TIM_Base_InitTypeDef TIM_BaseConf)
{
    TIM_Base_Init(TIMx, TIM_BaseConf);
}

void TIM7_Base_Init(TIM_RegDef_t *TIMx, TIM_Base_InitTypeDef TIM_BaseConf)
{
    TIM_Base_Init(TIMx, TIM_BaseConf);
}

void TIM_Base_Start(TIM_RegDef_t *TIMx)
{
    TIMx->CR1 |= (1U << TIM_CR1_CEN);
}

void TIM_Base_Stop(TIM_RegDef_t *TIMx)
{
    TIMx->CR1 &= ~(1U << TIM_CR1_CEN);
}

void TIM_Base_IT_Init(TIM_RegDef_t *TIMx, uint8_t Priority)
{
    TIMx->DIER |= (1U << 0U);
    if(TIMx == TIM6)
    {
        NVIC_SetPriority(IRQ_NO_TIM6, Priority);
        NVIC_EnableIRQ(IRQ_NO_TIM6);
    }
    else if(TIMx == TIM7)
    {
        NVIC_SetPriority(IRQ_NO_TIM7, Priority);
        NVIC_EnableIRQ(IRQ_NO_TIM7);
    }

}
