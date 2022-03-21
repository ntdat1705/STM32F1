#include "DHT11.h"

static uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
static uint16_t SUM; uint8_t Presence = 0;

static void GPIO_DataDHT11_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Pin_Data_DHT11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
static void GPIO_DataDHT11_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Pin_Data_DHT11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void DHT11_Start(void)
{
	GPIO_DataDHT11_Output();
	GPIO_ResetBits(GPIOA, Pin_Data_DHT11);
	Delay_Timer2_StandardLibrary_ms(20);
	GPIO_SetBits(GPIOA, Pin_Data_DHT11);
	Delay_Timer2_StandardLibrary_us(30);
}

static uint8_t DHT11_Check_Response(void)
{
	GPIO_DataDHT11_Input();
	uint8_t Response = 0;
	Delay_Timer2_StandardLibrary_us(40);
	if (!(GPIO_ReadInputDataBit(GPIOA, Pin_Data_DHT11)))
	{
		Delay_Timer2_StandardLibrary_us(80);
		if ((GPIO_ReadInputDataBit(GPIOA, Pin_Data_DHT11))) Response = 1;
		else Response = 0;
	}
	while ((GPIO_ReadInputDataBit(GPIOA, Pin_Data_DHT11)));   // wait for the pin to go low

	return Response;
}

static uint8_t DHT11_Read(void)
{
	uint8_t i,j;
	for(j=0;j<8;j++)
	{
		while(!(GPIO_ReadInputDataBit(GPIOA, Pin_Data_DHT11)));   // wait for the pin to go high
		Delay_Timer2_StandardLibrary_us(40);   // wait for 40 us
		if(!(GPIO_ReadInputDataBit(GPIOA, Pin_Data_DHT11)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while((GPIO_ReadInputDataBit(GPIOA, Pin_Data_DHT11)));  // wait for the pin to go low
	}
	return i;
}

void DHT11_GetData(DHT11_DataTypedef *DHT_Data)
{
  DHT11_Start();
	Presence = DHT11_Check_Response();
	Rh_byte1 = DHT11_Read();
	Rh_byte2 = DHT11_Read();
	Temp_byte1 = DHT11_Read();
	Temp_byte2 = DHT11_Read();
	SUM = DHT11_Read();
	if(SUM == (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2))
	{
			DHT_Data->Temperature = Temp_byte1;
			//DHT_Data->Humidity = Rh_byte1;
	}
}
