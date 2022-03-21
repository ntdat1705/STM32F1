#include "main.h"

DHT11_DataTypedef DHT11_Data;
uint8_t Temperature, Humidity;

int main(void)
{
	GPIOUsart2_Config();
	USART2_Config();
	Timer2_StandardLibrary();
	while(1)
	{		
	  DHT11_GetData(&DHT11_Data);
  	  Temperature = DHT11_Data.Temperature;
	  Humidity = DHT11_Data.Humidity;
	  USART_PutString(USART2, "Temperature: ");
	  USART_PutNumber(USART2, Temperature);
	  USART_PutString(USART2, "\n");
	  USART_PutString(USART2, "Humidity: ");
	  USART_PutNumber(USART2, Humidity);
	  USART_PutString(USART2, "\n");
	  Delay_Timer2_StandardLibrary_ms(1500);
	}
}

