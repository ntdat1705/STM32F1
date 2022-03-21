#ifndef __LCD_I2C_H
#define __LCD_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define I2C_LCD_ADDR 0x4E

#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3

void I2C_LCD_Init(void);
void I2C_LCD_Puts(char *szStr);
void I2C_LCD_Clear(void);
void I2C_LCD_NewLine(void);
void I2C_LCD_BackLight(uint8_t u8BackLight);
void LCD_Write_Chr(char chr);
void I2C_LCD_WriteCmd(uint8_t u8Cmd);


#endif

