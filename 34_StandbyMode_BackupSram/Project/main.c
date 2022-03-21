#include "main.h"

char usr_msg[250];

void printmsg(char *msg);
void Delay_ms(uint32_t u32Delay);
static void GPIONutNhan_Config(void);
static void EXTI0_Config(void);

int main(void)
{
	uint16_t uData = 0;
	//GPIONutNhan_Config();
	//EXTI0_Config();
	GPIOUsart1_Config();
	USART1_Config();
	/* Enable PWR and BKP clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Enable WKUP pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
	
	if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
	{
		PWR_ClearFlag(PWR_FLAG_SB);
		PWR_ClearFlag(PWR_FLAG_WU);
		SetSysClock();
		USART_PutString(USART1, "Woke up from Standby Mode\r\n");
		uData = BKP_ReadBackupRegister(BKP_DR1);
		sprintf(usr_msg,"BKP_DR1 after is %i\r\n", uData);
		printmsg(usr_msg);
		
	}else
	{
		BKP_WriteBackupRegister(BKP_DR1, 100);
		USART_PutString(USART1, "BKP_DR1 before is 100\r\n");
		USART_PutString(USART1, "Standby Mode\r\n");
		PWR_EnterSTANDBYMode();
	}
	while(1)
	{		
		
	}
}

static void GPIONutNhan_Config(void)
{
	GPIO_InitTypeDef GPIO_NutNhan;
	GPIO_NutNhan.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_NutNhan.GPIO_Pin = GPIO_Pin_0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_NutNhan);
}

static void EXTI0_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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

void printmsg(char *msg)
{
	for(uint32_t i = 0; i < strlen(msg); i++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) != SET);
		USART_SendData(USART1,msg[i]);
	}
}
