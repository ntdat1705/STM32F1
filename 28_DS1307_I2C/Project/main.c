#include "main.h"

char sdate[10], smonth[10], syear[10], sday[10], shour[10], sminute[10], ssecond[10];
const char *DAYS_OF_WEEK[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };


int main(void)
{
	Timer2_StandardLibrary();
	I2C_Config();
	DS1307_Init();
	I2C_LCD_Init();
	I2C_LCD_Clear();
	I2C_LCD_BackLight(1);
	/* To test leap year correction.
	DS1307_SetTimeZone(+7, 00);
	DS1307_SetDate(25);
	DS1307_SetMonth(11);
	DS1307_SetYear(2021);
	DS1307_SetDayOfWeek(4);
	DS1307_SetHour(17);
	DS1307_SetMinute(43);
	DS1307_SetSecond(00);*/
	while(1)
	{		
	  /* Get current date and time. */
		uint8_t date = DS1307_GetDate();
	    sprintf(&sdate[0], "%d", date);
		uint8_t month = DS1307_GetMonth();
		sprintf(&smonth[0], "%d", month);
		uint16_t year = DS1307_GetYear();
		sprintf(&syear[0], "%d", year);
		uint8_t hour = DS1307_GetHour();
		sprintf(&shour[0], "%d", hour);
		uint8_t minute = DS1307_GetMinute();
		sprintf(&sminute[0], "%d", minute);
		uint8_t second = DS1307_GetSecond();
		sprintf(&ssecond[0], "%d", second);
		I2C_LCD_Puts(&sdate[0]);
		I2C_LCD_Puts("-");
		I2C_LCD_Puts(&smonth[0]);
		I2C_LCD_Puts("-");
		I2C_LCD_Puts(&syear[0]);
		I2C_LCD_NewLine();
		I2C_LCD_Puts(&shour[0]);
		I2C_LCD_Puts(":");
		I2C_LCD_Puts(&sminute[0]);
		I2C_LCD_Puts(":");
		I2C_LCD_Puts(&ssecond[0]);
		I2C_LCD_WriteCmd(0x02);
	}
}

