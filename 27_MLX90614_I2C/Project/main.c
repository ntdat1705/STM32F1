#include "main.h"

uint8_t dataRecv[3] = {0};
uint16_t i2cdata = 0;
float temp_obj1, temp_obj2, temp_amb;
char str[10];

int main(void)
{
	Timer2_StandardLibrary();
	I2C_Config();
	I2C_LCD_Init();
	I2C_LCD_Clear();
	I2C_LCD_BackLight(1);
	I2C_LCD_Puts("Temperature");
	I2C_LCD_NewLine();
	while(1)
	{		
	  temp_amb = MLX90614_ReadTemp(I2C2, MLX90614_DEFAULT_SA, MLX90614_TAMB);
		I2C_LCD_WriteCmd(0xC5);
		sprintf(&str[0], "%0.2f", temp_amb);
		I2C_LCD_Puts(&str[0]);
	}
}

