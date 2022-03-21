#ifndef __MY_I2C_H
#define __MY_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
void Start_I2C_Transmitting(I2C_TypeDef* I2Cx, uint8_t Address);
void Start_I2C_Receiving(I2C_TypeDef* I2Cx, uint8_t Address);
void Send_I2C_Data(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t ReadAck_I2C_Data(I2C_TypeDef* I2Cx);
uint8_t ReadNack_I2C_Data(I2C_TypeDef* I2Cx);
void I2C_Config(void);
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t Data);
uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t Address);
void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t* Data, uint16_t Count);
void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t* Data, uint16_t Count);

#endif

