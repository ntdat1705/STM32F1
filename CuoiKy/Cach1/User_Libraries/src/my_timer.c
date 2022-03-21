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

void Timer3_IT(void)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TimerInit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TimerInit.TIM_CounterMode = TIM_CounterMode_Up;
	TimerInit.TIM_Period = 0xFFFF;
	TimerInit.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInit(TIM3, &TimerInit);
	/* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, 7200 - 1, TIM_PSCReloadMode_Immediate);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	
	/* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,ENABLE);
}
