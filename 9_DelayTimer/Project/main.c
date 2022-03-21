#include "main.h"

static void GPIOLed_Config(void);
void Timer2_StandardLibrary(void);
void Delay_Timer2_StandardLibrary_ms(uint32_t u32Delay);

int main(void)
{     
  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  SetSysClock();
  
  GPIOLed_Config();
	Timer2_StandardLibrary();
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_Timer2_StandardLibrary_ms(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay_Timer2_StandardLibrary_ms(1000);
	}
}

void Timer2_StandardLibrary(void)
{
	TIM_TimeBaseInitTypeDef TimerInit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TimerInit.TIM_CounterMode = TIM_CounterMode_Up;
	TimerInit.TIM_Period = 0xFFFF;
	TimerInit.TIM_Prescaler = 72 -1;
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

static void GPIOLed_Config(void)
{
	GPIO_InitTypeDef GPIO_LED;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_13;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC, &GPIO_LED);
}

