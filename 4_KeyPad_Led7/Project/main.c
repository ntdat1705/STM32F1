#include "stm32f10x.h"

static void GPIOLed7_Config(void);
static void GPIOKeyPad_Config(void);
static uint8_t Read_KeyPad(void);
static void ShowLed7(uint8_t Num);

uint16_t LED7SEG[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

int main(void)
{
	uint8_t i = 0;
	GPIOLed7_Config();
	GPIOKeyPad_Config();
	ShowLed7(0);
	while(1)
	{
		i = Read_KeyPad();
		if(i < 10)
			{
				ShowLed7(i);
			}
	}
}

static void GPIOLed7_Config(void)
{
	GPIO_InitTypeDef GPIO_LED;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_LED);
}

static void GPIOKeyPad_Config(void)
{
	GPIO_InitTypeDef GPIO_KeyPad;
	GPIO_KeyPad.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_KeyPad.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_KeyPad.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_KeyPad);
	GPIO_KeyPad.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_KeyPad.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOA, &GPIO_KeyPad);
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

static uint8_t Read_KeyPad(void)
{
	//check col1
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10));
			return 1;
		}
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11));
			return 4;
		}
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));
			return 7;
		}
	//check col2
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10));
			return 2;
		}
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11));
			return 5;
		}
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));
			return 8;
		}
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13));
			return 0;
		}
	//check col3
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10));
			return 3;
		}
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11));
			return 6;
		}
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));
			return 9;
		}
	return 10;
}

