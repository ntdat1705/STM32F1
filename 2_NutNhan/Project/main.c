#include "stm32f10x.h"

static void GPIOLed_Config(void);
static void GPIONutNhan_Config(void);

void Delay_ms(uint32_t u32Delay);

int main(void)
{
	GPIOLed_Config();
	GPIONutNhan_Config();
	uint32_t count = 0;
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
		{
			Delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
			{
				if(count % 2 == 0)
				{
					GPIO_SetBits(GPIOC, GPIO_Pin_13);
				}else
				{
					GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				}
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET);
				count++;
			}
		}
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

static void GPIONutNhan_Config(void)
{
	GPIO_InitTypeDef GPIO_NutNhan;
	GPIO_NutNhan.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_NutNhan.GPIO_Pin = GPIO_Pin_0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_NutNhan);
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

