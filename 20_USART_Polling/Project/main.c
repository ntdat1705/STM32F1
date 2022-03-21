#include "main.h"
#include <stdio.h>
char str1[10];
char str2[10];

void GPIOUsart2_Config(void);
void RCC_Configuration(void);
void USART2_Config(void);
void USART_PutChar(USART_TypeDef* USARTx, uint8_t ch);
void USART_PutString(USART_TypeDef* USARTx, uint8_t *str);
void USART_PutNumber(USART_TypeDef* USARTx, uint32_t x);
void Delay_ms(uint32_t u32Delay);

int main(void)
{
	float c = 1.234;
	uint32_t d = 20173727;
	sprintf(&str1[0], "%0.3f", c);
	sprintf(&str2[0], "%d", d);
	RCC_Configuration();
	GPIOUsart2_Config();
	USART2_Config();
	USART_PutString(USART2, "Nguyen Tien Dat\n");
	USART_PutString(USART2, (unsigned char*)str2);
	USART_PutString(USART2, "\n");
	USART_PutString(USART2, (unsigned char*)str1);
	while(1)
	{
	}
}

void RCC_Configuration(void)
{    
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  /* Enable USARTy Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
}

void GPIOUsart2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USART2_Config(void)
{
	/* USARTy and USARTz configured as follow:
        - BaudRate = 230400 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure USARTy */
  USART_Init(USART2, &USART_InitStructure);
  
  /* Enable the USARTy */
  USART_Cmd(USART2, ENABLE);
}

void USART_PutChar(USART_TypeDef* USARTx, uint8_t ch)
{
	/* Loop until USARTy DR register is empty */ 
    		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
    		{
    		}
	/* Send one byte from USARTy to USARTz */
   			 USART_SendData(USARTx, ch);
}

void USART_PutString(USART_TypeDef* USARTx, uint8_t *str)
{
	while(*str != 0)
	{
		USART_PutChar(USARTx, *str);
		str++;
	}
}

void USART_PutNumber(USART_TypeDef* USARTx, uint32_t x)
{
  char value[10]; //a temp array to hold results of conversion
  int i = 0; //loop index
  
  do
  {
    value[i++] = (char)(x % 10) + '0'; //convert integer to character
    x /= 10;
  } while(x);
  
  while(i) //send data
  {
    USART_PutChar(USARTx, value[--i]); 
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
