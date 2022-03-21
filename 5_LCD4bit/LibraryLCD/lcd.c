#include "lcd.h"

/* define macro for debug-------------------------------------------------------------------*/
#define DEBUG_LCD

static void delay(u32 nCount);

static void delay(uint32_t nCount){
	__IO uint32_t index = 0;

	/* default system clock is 168MHz */
	for(index = (500 * nCount); index != 0; index--);

}

void lcd_data_line_write(u8 data){
	LCD_EN_SET;
	if(data&0x80) LCD_D7_SET;
	else LCD_D7_CLEAR;
	if(data&0x40) LCD_D6_SET;
	else LCD_D6_CLEAR;	
	if(data&0x20) LCD_D5_SET;
	else LCD_D5_CLEAR;
	if(data&0x10) LCD_D4_SET;
	else LCD_D4_CLEAR;
	LCD_EN_CLEAR;
	
	delay(1);
	
	LCD_EN_SET;
	if(data&0x08) LCD_D7_SET;
	else LCD_D7_CLEAR;
	if(data&0x04) LCD_D6_SET;
	else LCD_D6_CLEAR;	
	if(data&0x02) LCD_D5_SET;
	else LCD_D5_CLEAR;
	if(data&0x1) LCD_D4_SET;
	else LCD_D4_CLEAR;
	
	LCD_EN_CLEAR;
	
	delay(1);
}

void lcd_Init_HW(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LCD_RS_GPIO_CLK | LCD_EN_GPIO_CLK|LCD_D4_GPIO_CLK|LCD_D5_GPIO_CLK|LCD_D6_GPIO_CLK|LCD_D7_GPIO_CLK, ENABLE);
	
	/* initialize LCD control lines to output */
	GPIO_InitStructure.GPIO_Pin 	= LCD_RS_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin 	= LCD_EN_GPIO_PIN;
	GPIO_Init(LCD_EN_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin 	= LCD_D4_GPIO_PIN;
	GPIO_Init(LCD_D4_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= LCD_D5_GPIO_PIN;
	GPIO_Init(LCD_D5_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= LCD_D6_GPIO_PIN;
	GPIO_Init(LCD_D6_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= LCD_D7_GPIO_PIN;
	GPIO_Init(LCD_D7_GPIO_PORT, &GPIO_InitStructure);	
}

/**
  * @brief  writes a control command to the LCD.
  * @param  data: Specifies the control command to the LCD. 
  *   This parameter can be one of following parameters:
  *     @arg LCD_DD_RAM_PTR		: Address Display Data RAM pointer
  *     @arg LCD_CLEAR_DISPLAY		: Clear entire display and set Display Data Address to 0
  *     @arg LCD_RETRN_HOME		: sets DDRAM address 0 and returns display from being shifted to original position.
  *     @arg LCD_DISP_INIT			: 8 bit data length and 2 lines
  *     @arg LCD_INC_MODE			: Entry mode is display Data RAM pointer incremented after write
  *     @arg LCD_DISP_ON			: Sets entire display on, cursor on and blinking of cursor position character
  *     @arg LCD_DISP_OFF			: Sets entire display off, cursor off
  *     @arg LCD_CURSOR_ON		: turn on cursor
  *     @arg LCD_CURSOR_OFF		: turn off cursor
  *     @arg LCD_CUR_MOV_LEFT		: Cursor move and shift to left
  *     @arg LCD_CUR_MOV_RIGHT	: Cursor move and shift to right
  * @retval None
  */
void lcd_Control_Write(u8 data)
{
	
	LCD_RS_CLEAR;
	lcd_data_line_write(data);
}

void lcd_Data_Write(u8 data){
	LCD_RS_SET;
	lcd_data_line_write(data);
	delay(20);
}

void lcd_Init(void){
	/* initialize hardware */
	lcd_Init_HW();

	/* Set 4-bits interface */
	lcd_Control_Write(0x33);		 
	delay(100);
	lcd_Control_Write(0x32);

	delay(500);

	/* Start to set LCD function */
	lcd_Control_Write(LCD_DISP_INIT);
		delay(500);
	
	/* clear LCD */
	lcd_Control_Write(LCD_CLEAR_DISPLAY);
		delay(500);
	/* wait 60ms */
	
	/* set entry mode */
	lcd_Control_Write(LCD_INC_MODE);	delay(500);
	
	/* set display to on */	
	lcd_Control_Write(LCD_DISP_ON);		delay(500);
	
	/* move cursor to home and set data address to 0 */
	lcd_Control_Write(LCD_RETRN_HOME);	delay(500);
}

void lcd_Home(void)
{
	// move cursor to home
	lcd_Control_Write(LCD_RETRN_HOME);
	delay(5);
}

void lcd_Clear(void)
{	
	lcd_Control_Write(LCD_CLEAR_DISPLAY);
	delay(200);
}

/**
  * @brief  moves the cursor/position to the row,col requested,
  *			this may not be accurate for all displays.
  * @param  x: the position of row of the LCD. 
  *   This parameter can be one of following parameters:
  *     @arg 0-1		: the LCD has 2 row
  * @param  y: the position of column of the LCD. 
  *   This parameter can be one of following parameters:
  *     @arg 0-15		: the LCD has 16 column
  * @retval None
  */
void lcd_Goto(u8 x, u8 y)
{
	__IO u8 DDRAMAddr;

	// remap lines into proper order
	switch(x)
	{
	case 0: DDRAMAddr = LCD_LINE0_ADDR+y; break;
	case 1: DDRAMAddr = LCD_LINE1_ADDR+y; break;
	case 2: DDRAMAddr = LCD_LINE2_ADDR+y; break;	// for LCD 16x4 or 20x4 only
	case 3: DDRAMAddr = LCD_LINE3_ADDR+y; break;
	default: DDRAMAddr = LCD_LINE0_ADDR+y;
	}

	// set data address
	lcd_Control_Write(LCD_DD_RAM_PTR | DDRAMAddr);
	delay(10);
}

void lcd_Print_Data(char* data)
{
u8 i = 0;
	while(data[i] != 0)
		{
		lcd_Data_Write(data[i]);
		i++;
		}
}
