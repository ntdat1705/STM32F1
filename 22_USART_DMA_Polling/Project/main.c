#include "main.h"
#include<string.h>

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   strlen((char*)TxBuffer1)

/* Private define ------------------------------------------------------------*/
uint8_t TxBuffer1[] = "DAI HOC BACH KHOA HA NOI";
uint8_t RxBuffer1[15];

void GPIOUsart2_Config(void);
void RCC_Configuration(void);
void USART2_Config(void);
void DMA_Configuration(void);
void Delay_ms(uint32_t u32Delay);

int main(void)
{
	RCC_Configuration();
	GPIOUsart2_Config();
	DMA_Configuration();
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
  /* DMA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
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
  /* Enable USARTy DMA Rx and TX request */
  USART_DMACmd(USART2, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
  /* Enable the USARTy */
  USART_Cmd(USART2, ENABLE);
}

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

  /* USARTy TX DMA1 Channel (triggered by USARTy Tx event) Config */
  DMA_DeInit(DMA1_Channel7);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_BASE + 0x04;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);
  /* USARTy RX DMA1 Channel (triggered by USARTy Rx event) Config */
  DMA_DeInit(DMA1_Channel6);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_BASE + 0x04;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 15;
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);
  /* Enable USARTy TX DMA1 Channel */
  DMA_Cmd(DMA1_Channel7, ENABLE);
  /* Enable USARTy RX DMA1 Channel */
  DMA_Cmd(DMA1_Channel6, ENABLE);
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
