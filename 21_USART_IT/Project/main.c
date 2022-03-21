#include "main.h"

/* Private define ------------------------------------------------------------*/
uint8_t RxBuffer[20];
__IO uint8_t RxCounter = 0;

void GPIOUsart2_Config(void);
void RCC_Configuration(void);
void USART2_Config(void);
void Delay_ms(uint32_t u32Delay);
void NVIC_Configuration(void);

int main(void)
{
	RCC_Configuration();
	GPIOUsart2_Config();
	NVIC_Configuration();
	USART2_Config();
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
  /* Enable USARTy Receive and Transmit interrupts */
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  /* Enable the USARTy */
  USART_Cmd(USART2, ENABLE);
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

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
