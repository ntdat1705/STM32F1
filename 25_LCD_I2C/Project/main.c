#include "main.h"

int main(void)
{
	Timer2_StandardLibrary();
	I2C_Config();
	I2C_LCD_Init();
	I2C_LCD_Clear();
	I2C_LCD_BackLight(1);
	I2C_LCD_Puts("Nguyen Tien Dat");
	while(1)
	{		
		I2C_LCD_Clear();
		I2C_LCD_Puts("Nguyen Tien Dat");
		Delay_Timer2_StandardLibrary_ms(1000);
		for(uint8_t i = 0; i < 16; i++)
		{
			I2C_LCD_WriteCmd(0x18);
			Delay_Timer2_StandardLibrary_ms(500);
		}
	}
}

