#ifndef __MY_USART_H
#define __MY_USART_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

void GPIOUsart1_Config(void);
void USART1_Config(void);

#endif

