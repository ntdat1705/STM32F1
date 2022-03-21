#include "stm32f10x.h"

static void GPIOLed_Config(void);
void Delay_ms(uint32_t u32Delay);

int main(void)
{
	GPIOLed_Config();
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
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

void Delay_ms(uint32_t u32Delay)
{
	while(u32Delay)
		{
			SysTick->LOAD = 72U * 1000U - 1U;
			SysTick->VAL = 0U;
			SysTick->CTRL = 5U;
			while(!(SysTick->CTRL & (1U << 16U)));
			--u32Delay;
		}
}

