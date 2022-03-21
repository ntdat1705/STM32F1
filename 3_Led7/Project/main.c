#include "stm32f10x.h"

static void GPIOLed7_Config(void);
static void ShowLed7(uint8_t Num);
static void Show2Led7(uint8_t Num);
void Delay_ms(uint32_t u32Delay);

uint16_t LED7SEG[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

int main(void)
{
	uint8_t i;
	GPIOLed7_Config();
	while(1)
	{
		for(i = 0; i < 100; i++)
			{
				Show2Led7(i);
			}
	}
}

static void GPIOLed7_Config(void)
{
	GPIO_InitTypeDef GPIO_LED;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_LED);
}

static void ShowLed7(uint8_t Num)
{
	uint16_t TempNum = 0U;
	TempNum = LED7SEG[Num];
	if((TempNum & 0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	}
	TempNum = TempNum >> 1;
	if((TempNum & 0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
	TempNum = TempNum >> 1;
	if((TempNum & 0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
	TempNum = TempNum >> 1;
	if((TempNum & 0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	}
	TempNum = TempNum >> 1;
	if((TempNum & 0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
	TempNum = TempNum >> 1;
	if((TempNum & 0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
	TempNum = TempNum >> 1;
	if((TempNum & 0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	}
}

static void Show2Led7(uint8_t Num)
{
	uint8_t chuc, donvi, i;
	chuc = Num  / 10;
	donvi = Num % 10;
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	for(i = 0; i < 20; i++)
		{
	ShowLed7(chuc);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	Delay_ms(1);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	Delay_ms(1);
	ShowLed7(donvi);
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
	Delay_ms(1);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	Delay_ms(1);
		}
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

