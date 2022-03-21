#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "my_timer.h"
#include "my_i2c.h"
#include "my_usart.h"
#include "LCD_I2C.h"
#include "DHT11.h"
#include "DS18B20.h"
#include<stdio.h>
#include<stdlib.h>

void Task_GetData_DHT11(void);
void Task_GetData_DS18B20(void);
void Task_Display_LCD(void);
void Task_Send_To_PC(void);
void Start_BreakPoint(void);
void Stop_BreakPoint(void);
void LCD_DHT11(void);
void LCD_DS18B20(void);
void Project_Init(void);

#endif
