#include "main.h"

GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_ClockFreq;
long ClockSource, ClockHCLK, ClockPCLK2, ClockPCLK1;

int main(void)
{     
  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  SetSysClock();

  /* This function fills the RCC_ClockFreq structure with the current
     frequencies of different on chip clocks (for debug purpose) */
  RCC_GetClocksFreq(&RCC_ClockFreq);
	ClockSource = RCC_ClockFreq.SYSCLK_Frequency;
	ClockHCLK = RCC_ClockFreq.HCLK_Frequency;
	ClockPCLK1 = RCC_ClockFreq.PCLK1_Frequency;
	ClockPCLK2 = RCC_ClockFreq.PCLK2_Frequency;

  /* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails */
  RCC_ClockSecuritySystemCmd(ENABLE);
     
  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* Output HSE clock on MCO pin ---------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  RCC_MCOConfig(RCC_MCO_HSE);
  
  while (1)
  {
    
  }
}
