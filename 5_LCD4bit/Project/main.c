#include "stm32f10x.h"
#include "lcd.h"
#include <stdio.h>

void Delay_ms(uint32_t u32Delay);

int main(void)
{
	lcd_Init();		
	while(1){
			lcd_Clear();
			lcd_Goto(0,2);
 			lcd_Print_Data("EE4251-129087");
			Delay_ms(1000);
			lcd_Clear();
			lcd_Goto(0,5);
			lcd_Print_Data("(+_+)");
			Delay_ms(1000);
			lcd_Clear();
			lcd_Goto(0,16);
			lcd_Print_Data("Nguyen Tien Dat");
			for(uint8_t i = 0; i < 16; i++)
			{
				lcd_Control_Write(0x18);
				Delay_ms(500);
			}
			Delay_ms(500);
			lcd_Clear();
			Delay_ms(500);
			lcd_Print_Data("Nguyen Tien Dat");
			lcd_Clear();
			Delay_ms(500);
			lcd_Print_Data("Nguyen Tien Dat");
			Delay_ms(1000);
	}
}

void Delay_ms(uint32_t u32Delay)
{
	while(u32Delay)
		{
			SysTick->LOAD = 72U * 1000U - 1U;
			SysTick->VAL = 0U;
			SysTick->CTRL = 5U;
			while(!(SysTick->CTRL & (1U << 16U)));
			--u32Delay;
		}
}
