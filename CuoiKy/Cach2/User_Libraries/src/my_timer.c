#include "my_timer.h"

void Timer2_StandardLibrary(void)
{
	TIM_TimeBaseInitTypeDef TimerInit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TimerInit.TIM_CounterMode = TIM_CounterMode_Up;
	TimerInit.TIM_Period = 0xFFFF;
	TimerInit.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM2, &TimerInit);
	TIM_Cmd(TIM2,ENABLE);
}

void Delay_Timer2_StandardLibrary_ms(uint32_t u32Delay)
{
	while(u32Delay)
		{
			TIM_SetCounter(TIM2, 0U);
			while(TIM_GetCounter(TIM2) < 1000U);
			u32Delay--;
		}
}

void Delay_Timer2_StandardLibrary_us(uint32_t u32Delay)
{
	while(u32Delay)
		{
			TIM_SetCounter(TIM2, 0U);
			while(TIM_GetCounter(TIM2) < 1U);
			u32Delay--;
		}
}

