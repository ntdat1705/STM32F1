#include "mlx90614.h"

uint16_t MLX90614_ReadReg(I2C_TypeDef* I2Cx, uint8_t devAddr, uint8_t regAddr){
	uint16_t data;
	uint8_t in_buff[3];
	
	Start_I2C_Transmitting(I2Cx, (devAddr<<1));
	Send_I2C_Data(I2Cx, regAddr);
	Start_I2C_Receiving(I2Cx, (devAddr<<1));
	for(uint8_t i = 0; i < 3; i++)
		{
			if(i == (3 - 1))
				{
					in_buff[i] = ReadNack_I2C_Data(I2Cx);
				}else
					{
					in_buff[i] = ReadAck_I2C_Data(I2Cx);
					}
		}
	data = (in_buff[1] <<8 | in_buff[0]);

	return data;
}
float MLX90614_ReadTemp(I2C_TypeDef* I2Cx, uint8_t devAddr, uint8_t regAddr) {
	float temp;
	uint16_t data;

	data = MLX90614_ReadReg(I2Cx, devAddr, regAddr);
	temp = data*0.02 - 273.15;

	return temp;
}
