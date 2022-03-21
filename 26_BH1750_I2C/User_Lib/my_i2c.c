#include "my_i2c.h"

static void Start_I2C_Transmitting(I2C_TypeDef* I2Cx, uint8_t Address);
static void Start_I2C_Receiving(I2C_TypeDef* I2Cx, uint8_t Address);
static void Send_I2C_Data(I2C_TypeDef* I2Cx, uint8_t data);
static uint8_t ReadAck_I2C_Data(I2C_TypeDef* I2Cx);
static uint8_t ReadNack_I2C_Data(I2C_TypeDef* I2Cx);

void I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	//clock i2c, gpio
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	//config SCL, SDA I2C2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//config I2C
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_OwnAddress1 = 0;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_Init(I2C2, &I2C_InitStructure);

	//en i2c
	I2C_Cmd(I2C2, ENABLE);
}

static void Start_I2C_Transmitting(I2C_TypeDef* I2Cx, uint8_t Address){
I2C_AcknowledgeConfig(I2Cx, DISABLE);
//For transmiting to the slave, start the data transmitting
I2C_GenerateSTART(I2Cx, ENABLE);
 //Waiting for flag
 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
 //Send address to slave
 I2C_Send7bitAddress(I2Cx, Address, I2C_Direction_Transmitter);
//And check the transmitting
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

}

static void Start_I2C_Receiving(I2C_TypeDef* I2Cx, uint8_t Address){
I2C_AcknowledgeConfig(I2Cx, DISABLE);
//For receiving to the slave
I2C_GenerateSTART(I2Cx, ENABLE);
 //Waiting for flag
 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
 //Send address to slave
 I2C_Send7bitAddress(I2Cx, Address, I2C_Direction_Receiver);
//And check the transmitting
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

}

static void Send_I2C_Data(I2C_TypeDef* I2Cx, uint8_t data)
{
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE));
	I2C_SendData(I2Cx, data);
	// wait for the data trasnmitted flag
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

static uint8_t ReadAck_I2C_Data(I2C_TypeDef* I2Cx){
	uint8_t data;
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
	data = I2C_ReceiveData(I2Cx);
	return data;
}


static uint8_t ReadNack_I2C_Data(I2C_TypeDef* I2Cx){
	uint8_t data;
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
	data = I2C_ReceiveData(I2Cx);
	return data;
}

void I2C_Write(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t Data)
{
	Start_I2C_Transmitting(I2Cx, Address);
	Send_I2C_Data(I2Cx, Data);
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t Address)
{
	uint8_t received_data;
	Start_I2C_Receiving(I2Cx, Address);
	received_data = ReadNack_I2C_Data(I2Cx);
	return received_data;
}

void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t* Data, uint16_t Count)
{
	uint8_t i;
	Start_I2C_Receiving(I2Cx, Address);
	for(i = 0; i < Count; i++)
		{
			if(i == (Count - 1))
				{
					Data[i] = ReadNack_I2C_Data(I2Cx);
				}else
					{
					Data[i] = ReadAck_I2C_Data(I2Cx);
					}
		}
}
