#include "stm32f10x.h"
#include "lcd.h"
#include <stdio.h>

void Delay_ms(uint32_t u32Delay);
static void GPIOKeyPad_Config(void);
static uint8_t Read_KeyPad(void);
static void Show_LCD(void);

int main(void)
{
	lcd_Init();	
	GPIOKeyPad_Config();
	while(1){
		lcd_Goto(0,8);
		Show_LCD();
	}
}

static void GPIOKeyPad_Config(void)
{
	GPIO_InitTypeDef GPIO_KeyPad;
	GPIO_KeyPad.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_KeyPad.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5;
	GPIO_KeyPad.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_Init(GPIOB, &GPIO_KeyPad);
	GPIO_KeyPad.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_KeyPad.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_KeyPad);
}

static uint8_t Read_KeyPad(void)
{
	//check col1
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
			return 13;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
			return 12;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8));
			return 11;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9));
			return 10;
		}
	//check col2
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
			return 15;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
			return 9;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8));
			return 6;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9));
			return 3;
		}
	//check col3
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
			return 0;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
			return 8;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8));
			return 5;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9));
			return 2;
		}
	//check col3
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
			return 14;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
			return 7;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8));
			return 4;
		}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		{
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9));
			return 1;
		}
	return 16;
}

static void Show_LCD(void)
{
	uint8_t i = Read_KeyPad();
	if(i < 10)
	{
		lcd_Data_Write(i + 48);
	}else if(10 <= i && i <= 13)
	{
		lcd_Data_Write(i + 55);
	}else if(i == 14)
	{
		lcd_Print_Data("*");
	}else if(i == 15)
	{
		lcd_Print_Data("#");
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
