#include "main.h"

DHT11_DataTypedef DHT11_Data;
uint8_t TemperatureDHT11;
extern float TemperatureDS18B20;

char Str[10];

volatile float fTimeMeasurement = 0.0;

int main(void)
{
	Project_Init();
	//Start_BreakPoint();
	//Task_GetData_DS18B20();
	//Task_GetData_DHT11();
	//Task_Send_To_PC();
	//Task_Display_LCD();
	//Stop_BreakPoint();
	while(1)
	{
		__WFI();
	}
}

void Task_GetData_DHT11(void)
{
	DHT11_GetData(&DHT11_Data);
  TemperatureDHT11 = DHT11_Data.Temperature;
}
void Task_GetData_DS18B20(void)
{
	DS18B20_GetData();
}

void Task_Send_To_PC(void)
{
	printf("\nDHT11: %d°C\n\rDS18B20: %0.0f°C\n\r", TemperatureDHT11, TemperatureDS18B20);
}

void Task_Display_LCD(void)
{
	LCD_DHT11();
	LCD_DS18B20();
}

void Project_Init(void)
{
	Timer2_StandardLibrary();
	I2C_Config();
	GPIOUsart1_Config();
	USART1_Config();
	printf(" \n\r");
	I2C_LCD_Init();
	I2C_LCD_Clear();
	I2C_LCD_BackLight(1);
	I2C_LCD_Puts("DHT11:");
	I2C_LCD_WriteCmd(0x8C);
	LCD_Write_Chr(0xDF);
	I2C_LCD_Puts("C");
	I2C_LCD_NewLine();
	I2C_LCD_Puts("DS18B20:");
	I2C_LCD_WriteCmd(0xCC);
	LCD_Write_Chr(0xDF);
	I2C_LCD_Puts("C");
	Timer3_IT();
}

void LCD_DHT11(void)
{
	I2C_LCD_WriteCmd(0x8A);
	sprintf(&Str[0], "%d", TemperatureDHT11);
	I2C_LCD_Puts(&Str[0]);
}
void LCD_DS18B20(void)
{
	I2C_LCD_WriteCmd(0xCA);
		sprintf(&Str[0], "%0.0f", TemperatureDS18B20);
		I2C_LCD_Puts(&Str[0]);
}

void Start_BreakPoint(void)
{
	SysTick->LOAD = 72U * 1000U - 1U;
	SysTick->VAL = 0U;
	SysTick->CTRL = 7U;
}
void Stop_BreakPoint(void)
{
	fTimeMeasurement = fTimeMeasurement + (71999U - (SysTick->VAL))/71999.0;
	SysTick->CTRL &= ~(1 << 1);
}


