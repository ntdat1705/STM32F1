#ifndef __MY_I2C_H
#define __MY_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void I2C_Config(void);
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t Data);
uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t Address);
void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t* Data, uint16_t Count);

#endif

