#include "main.h"

float Temperature = 0;
char str1[10];

int main(void)
{
	GPIOUsart2_Config();
	USART2_Config();
	Timer2_StandardLibrary();
	while(1)
	{		
	  DS18B20_GetData();
	  sprintf(&str1[0], "%0.2f", Temperature);
	  USART_PutString(USART2, "Temperature: ");
	  USART_PutString(USART2, (unsigned char*)str1);
	  USART_PutString(USART2, "\n");
	  Delay_Timer2_StandardLibrary_ms(1000);
	}
}

