#include "main.h"

uint8_t u8DataWrite[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void Delay_ms(uint32_t u32Delay);
void GPIOLed_Config(void);
FLASH_Status Flash_Write(uint32_t Address, uint8_t *u8BufferWrite, uint32_t u32Length);
FLASH_Status Flash_Read(uint32_t Address, uint8_t *u8BufferRead, uint32_t u32Length);

int main(void)
{
	uint8_t u8DataRead[6];
	GPIOLed_Config();
	FLASH_Status eFlashStatus;
	GPIOUsart1_Config();
	USART1_Config();
	//xoa page 4
	FLASH_Unlock();
	eFlashStatus = FLASH_ErasePage(0x08001000);
	if(eFlashStatus == FLASH_COMPLETE)
		{
			printf("\rPage 4 is erase\r\n");
		}
	Flash_Write(0x08001000, u8DataWrite, 10);
		if(eFlashStatus == FLASH_COMPLETE)
		{
			printf("Write complete\r\n");
		}
	Flash_Read(0x08001000, u8DataRead, 10);
		if(eFlashStatus == FLASH_COMPLETE)
		{
			printf("Read complete\r\n");
		}
	while(1)
	{		
		if(u8DataRead[0] == u8DataWrite[0])
		{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		}
	}
}

FLASH_Status Flash_Write(uint32_t Address, uint8_t *u8BufferWrite, uint32_t u32Length)
{
	uint32_t u32Count;
	for(u32Count = 0; u32Count < (u32Length / 2); u32Count++)
		{
			FLASH_ProgramHalfWord(Address + u32Count * 2, (((uint16_t)u8BufferWrite[u32Count*2U + 1] << 8) | 0x00FF) & ((uint16_t)u8BufferWrite[u32Count*2U] | 0xFF00));
		}
	return FLASH_COMPLETE;
}

FLASH_Status Flash_Read(uint32_t Address, uint8_t *u8BufferRead, uint32_t u32Length)
{
	uint32_t u32Count;
	for(u32Count = 0; u32Count < u32Length; u32Count++)
	{
		u8BufferRead[u32Count] = *(uint32_t*)(Address + u32Count);
	}
	return FLASH_COMPLETE;
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

void GPIOLed_Config(void)
{
	GPIO_InitTypeDef GPIO_LED;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_13;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC, &GPIO_LED);
}
