#ifndef __MY_USART_H
#define __MY_USART_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void GPIOUsart2_Config(void);
void USART2_Config(void);
void USART_PutChar(USART_TypeDef* USARTx, uint8_t ch);
void USART_PutString(USART_TypeDef* USARTx, uint8_t *str);
void USART_PutNumber(USART_TypeDef* USARTx, uint32_t x);

#endif

