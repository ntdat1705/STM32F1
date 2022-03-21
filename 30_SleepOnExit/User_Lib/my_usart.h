#ifndef __MY_USART_H
#define __MY_USART_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void GPIOUsart1_Config(void);
void USART1_Config(void);
void USART_PutChar(USART_TypeDef* USARTx, uint8_t ch);
void USART_PutString(USART_TypeDef* USARTx, uint8_t *str);
void USART_PutNumber(USART_TypeDef* USARTx, uint32_t x);

#endif

