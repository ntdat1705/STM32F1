#include "main.h"

static void GPIOLed_Config(void);
void Timer2_Interrupt(void);
void NVIC_Configuration(void);

int main(void)
{     
  GPIOLed_Config();
  Timer2_Interrupt();
  NVIC_Configuration();
	while(1)
	{
	}
}

void Timer2_Interrupt(void)
{
	TIM_TimeBaseInitTypeDef TimerInit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TimerInit.TIM_CounterMode = TIM_CounterMode_Up;
	TimerInit.TIM_Period = 5000 - 1;
	TimerInit.TIM_Prescaler = 7200 -1;
	TIM_TimeBaseInit(TIM2, &TimerInit);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}


static void GPIOLed_Config(void)
{
	GPIO_InitTypeDef GPIO_LED;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_13;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC, &GPIO_LED);
}

