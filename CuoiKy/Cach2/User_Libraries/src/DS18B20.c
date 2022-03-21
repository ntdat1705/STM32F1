#include "DS18B20.h"

static uint8_t Temp_byte1, Temp_byte2;
static uint16_t TEMP = 0;
float TemperatureDS18B20;
static void GPIO_DataDS18B20_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Pin_Data_DS18B20;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
static void GPIO_DataDS18B20_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Pin_Data_DS18B20;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static uint8_t DS18B20_Start(void)
{
	uint8_t Response = 0;
	GPIO_DataDS18B20_Output();   // set the pin as output
	GPIO_ResetBits(GPIOA, Pin_Data_DS18B20);  // pull the pin low
	Delay_Timer2_StandardLibrary_us(480);   // delay according to datasheet

	GPIO_DataDS18B20_Input();    // set the pin as input
	Delay_Timer2_StandardLibrary_us(80);    // delay according to datasheet

	if (!(GPIO_ReadInputDataBit(GPIOA, Pin_Data_DS18B20))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = 0;

	Delay_Timer2_StandardLibrary_us(480); // 480 us delay totally.

	return Response;
}

static void DS18B20_Write(uint8_t data)
{
	GPIO_DataDS18B20_Output();  // set as output

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			GPIO_DataDS18B20_Output();  // set as output
			GPIO_ResetBits(GPIOA, Pin_Data_DS18B20);  // pull the pin LOW
			Delay_Timer2_StandardLibrary_us(1);  // wait for 1 us

			GPIO_DataDS18B20_Input();  // set as input
			Delay_Timer2_StandardLibrary_us(50);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			GPIO_DataDS18B20_Output();
			GPIO_ResetBits(GPIOA, Pin_Data_DS18B20);  // pull the pin LOW
			Delay_Timer2_StandardLibrary_us(50);  // wait for 60 us

			GPIO_DataDS18B20_Input();
		}
	}
}

static uint8_t DS18B20_Read(void)
{
	uint8_t value=0;
	GPIO_DataDS18B20_Input();

	for (int i=0;i<8;i++)
	{
		GPIO_DataDS18B20_Output();  // set as output

		GPIO_ResetBits(GPIOA, Pin_Data_DS18B20); // pull the data pin LOW
		Delay_Timer2_StandardLibrary_us(2);  // wait for 2 us

		GPIO_DataDS18B20_Input();  // set as input
		if (GPIO_ReadInputDataBit(GPIOA, Pin_Data_DS18B20))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		Delay_Timer2_StandardLibrary_us(60);  // wait for 60 us
	}
	return value;
}

void DS18B20_GetData(void)
{
  DS18B20_Start();
	Delay_Timer2_StandardLibrary_ms(1);
	DS18B20_Write (0xCC);  // skip ROM
	DS18B20_Write (0x44);  // convert t
	Delay_Timer2_StandardLibrary_ms(800);

	DS18B20_Start();
	Delay_Timer2_StandardLibrary_ms(1);
	DS18B20_Write (0xCC);  // skip ROM
	DS18B20_Write (0xBE);  // Read Scratch-pad

	Temp_byte1 = DS18B20_Read();
	Temp_byte2 = DS18B20_Read();
	TEMP = (Temp_byte2<<8)|Temp_byte1;
	TemperatureDS18B20 = TEMP/16;
}
