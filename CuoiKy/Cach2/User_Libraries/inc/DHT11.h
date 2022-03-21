#ifndef __DHT11_H
#define __DHT11_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define Pin_Data_DHT11 GPIO_Pin_1

typedef struct
{
	uint8_t Temperature;
	//float Humidity;
}DHT11_DataTypedef;

void DHT11_GetData(DHT11_DataTypedef *DHT11_Data);

#endif

