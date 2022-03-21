#include "main.h"

#define BH1750_ADDR (0x5C << 1)
#define Power_On 0x01
#define One_Time 0x23


int main(void)
{
	uint8_t buffer[2];
	uint16_t temp = 0;
	int LUX = 0;
	uint8_t chucnghin, nghin, tram, chuc, donvi;
	Timer2_StandardLibrary();
	I2C_Config();
	I2C_LCD_Init();
	I2C_LCD_Clear();
	I2C_LCD_BackLight(1);
	I2C_LCD_Puts("INTENSITY");
	I2C_LCD_NewLine();
	while(1)
	{		
		I2C_LCD_WriteCmd(0xC5);
		Delay_Timer2_StandardLibrary_ms(200);
		I2C_Write(I2C2, BH1750_ADDR, Power_On);
		Delay_Timer2_StandardLibrary_ms(2);
		I2C_Write(I2C2, BH1750_ADDR, One_Time);
		Delay_Timer2_StandardLibrary_ms(30);
		I2C_ReadMulti(I2C2, BH1750_ADDR, buffer, 2);
		Delay_Timer2_StandardLibrary_ms(30);
		temp = (uint16_t) (buffer[0] << 8) | buffer[1];
		LUX = temp / 1.2;
		chucnghin = (LUX / 10000) + 48;
		nghin = (LUX % 10000) / 1000 + 48;
		tram = (LUX % 1000) / 100 + 48;
		chuc = ((LUX % 100 / 10)) + 48;
		donvi = (LUX % 10) + 48;
		if (chucnghin == '0') {
			if (nghin == '0') {
				if (tram == '0') {
					if (chuc == '0') {
						LCD_Write_Chr(' ');
						LCD_Write_Chr(' ');
						LCD_Write_Chr(' ');
						LCD_Write_Chr(' ');
						LCD_Write_Chr(donvi);
						Delay_Timer2_StandardLibrary_ms(500);
					} else {
						LCD_Write_Chr(' ');
						LCD_Write_Chr(' ');
						LCD_Write_Chr(' ');
						LCD_Write_Chr(chuc);
						LCD_Write_Chr(donvi);
						Delay_Timer2_StandardLibrary_ms(500);
					}
				} else {
					LCD_Write_Chr(' ');
					LCD_Write_Chr(' ');
					LCD_Write_Chr(tram);
					LCD_Write_Chr(chuc);
					LCD_Write_Chr(donvi);
					Delay_Timer2_StandardLibrary_ms(500);
				}
			} else {
				LCD_Write_Chr(' ');
				LCD_Write_Chr(nghin);
				LCD_Write_Chr(tram);
				LCD_Write_Chr(chuc);
				LCD_Write_Chr(donvi);
				Delay_Timer2_StandardLibrary_ms(500);
			}
		} else {
			LCD_Write_Chr(chucnghin);
			LCD_Write_Chr(nghin);
			LCD_Write_Chr(tram);
			LCD_Write_Chr(chuc);
			LCD_Write_Chr(donvi);
			Delay_Timer2_StandardLibrary_ms(500);
		}
		I2C_LCD_Puts(" LUX");
	}
}

