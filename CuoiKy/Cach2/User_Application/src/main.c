#include "main.h"

DHT11_DataTypedef DHT11_Data;
extern float TemperatureDS18B20;
uint8_t RxBuffer[20];
__IO uint8_t RxCounter = 0;
uint8_t Time[2];
uint32_t Time1 = 2000, Time2 = 3000;
char menu[] = {"\
	\r\nChange period of vGetDataDHT11Task	 ----> 1\
	\r\nChange period of vGetDataDS18B20Task ----> 2\
	\r\nEXIT    												     ----> 0\
	\r\nType your opption here : "};
uint8_t Flag = 0;
uint8_t Flag2 = 0;
/* Define an enumerated type used to identify the source of the data. */
typedef enum
{
	eDataDHT11,
	eDataDS18B20
} DataSource_t;

/* Define the structure type that will be passed on the queue. */
typedef struct
{
	uint8_t ucValue;
	DataSource_t eDataSource;
} Data_t;

char Str[10];

QueueHandle_t xQueueLCD;
QueueHandle_t xQueuePC;

TaskHandle_t xGetDataDHT11Task = NULL;
TaskHandle_t xGetDataDS18B20Task = NULL;
TaskHandle_t xDisplayLCDTask = NULL;
TaskHandle_t xSendToPCTask = NULL;
TaskHandle_t xSetTimeTask = NULL;

static void Project_Init(void);
static void vGetDataDHT11Task( void *pvParameters );
static void vGetDataDS18B20Task( void *pvParameters );
static void vDisplayLCDTask( void *pvParameters );
static void vSendToPCTask( void *pvParameters );
static void vSetTimeTask( void *pvParameters );
static void Menu(void);
static void ChangePeriod(void);
static void NewPeriod(void);

int main( void )
{
	Project_Init();
	xQueueLCD = xQueueCreate( 1, sizeof( Data_t ) );
	xQueuePC = xQueueCreate( 1, sizeof( Data_t ) );
	if( (xQueueLCD != NULL) && (xQueuePC != NULL) )
	{
		xTaskCreate( vGetDataDHT11Task, "vGetDataDHT11Task", 500, NULL, 2, &xGetDataDHT11Task );
		xTaskCreate( vGetDataDS18B20Task, "vGetDataDS18B20Task", 500, NULL, 2, &xGetDataDS18B20Task );
		xTaskCreate( vDisplayLCDTask, "vDisplayLCDTask", 500, NULL, 3, &xDisplayLCDTask );
		xTaskCreate( vSendToPCTask, "vSendToPCTask", 500, NULL, 3, &xSendToPCTask );
		xTaskCreate( vSetTimeTask, "vSetTimeTask", 500, NULL, 4, &xSetTimeTask );
		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
	}
	else
	{
		/*The queue could not be created*/
	}
	for( ;; );
}

static void vGetDataDHT11Task( void *pvParameters )
{
	Data_t xValueToSend;
	BaseType_t xStatus;
	for( ;; )
	{
		taskENTER_CRITICAL();
		DHT11_GetData(&DHT11_Data);
		taskEXIT_CRITICAL();
		xValueToSend.ucValue = DHT11_Data.Temperature;
		xValueToSend.eDataSource = eDataDHT11; 
		xStatus = xQueueSend( xQueueLCD, &xValueToSend, portMAX_DELAY );
		xStatus = xQueueSend( xQueuePC, &xValueToSend, portMAX_DELAY );
		if( xStatus != pdPASS )
		{
			/*Could not send to the queue*/
		}
		vTaskDelay( pdMS_TO_TICKS( Time1 ) );
	}
}

static void vGetDataDS18B20Task( void *pvParameters )
{
	Data_t xValueToSend;
	BaseType_t xStatus;
	for( ;; )
	{
		taskENTER_CRITICAL();
		DS18B20_GetData();
		taskEXIT_CRITICAL();
		xValueToSend.ucValue = (uint8_t)TemperatureDS18B20;
		xValueToSend.eDataSource = eDataDS18B20; 
		xStatus = xQueueSend( xQueueLCD, &xValueToSend, portMAX_DELAY );
		xStatus = xQueueSend( xQueuePC, &xValueToSend, portMAX_DELAY );
		if( xStatus != pdPASS )
		{
			/*Could not send to the queue*/
		}
		vTaskDelay( pdMS_TO_TICKS( Time2 ) );
	}
}

static void vDisplayLCDTask( void *pvParameters )
{
	Data_t xReceivedStructure;
	BaseType_t xStatus;
	for( ;; )
	{
		xStatus = xQueueReceive( xQueueLCD, &xReceivedStructure, portMAX_DELAY ); 
		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
			value and the source of the value. */
			if( xReceivedStructure.eDataSource == eDataDHT11 )
			{
				I2C_LCD_WriteCmd(0x8A);
				sprintf(&Str[0], "%d", xReceivedStructure.ucValue);
				I2C_LCD_Puts(&Str[0]);
			}
			else
			{
				I2C_LCD_WriteCmd(0xCA);
				sprintf(&Str[0], "%d", xReceivedStructure.ucValue);
				I2C_LCD_Puts(&Str[0]);
			}
		}
		else
		{
		/* Nothing was received from the queue. This must be an error as this
		task should only run when the queue is full. */
		/*Could not receive from the queue*/
		}
	}
}

static void vSendToPCTask( void *pvParameters )
{
	Data_t xReceivedStructure;
	BaseType_t xStatus;
	for( ;; )
	{
		xStatus = xQueueReceive( xQueuePC, &xReceivedStructure, portMAX_DELAY ); 
		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
			value and the source of the value. */
			if( xReceivedStructure.eDataSource == eDataDHT11 )
			{
				printf("\r\nDHT11: %d", xReceivedStructure.ucValue);
			}
			else
			{
				printf("\r\nDS18B20: %d", xReceivedStructure.ucValue);
			}			
		}
		else
		{
		/* Nothing was received from the queue. This must be an error as this
		task should only run when the queue is full. */
		/*Could not receive from the queue*/
		}
	}

}

static void Project_Init(void)
{
	Timer2_StandardLibrary();
	I2C_Config();
	GPIOUsart1_Config();
	USART1_Config();
	printf(" \n\r");
	I2C_LCD_Init();
	I2C_LCD_Clear();
	I2C_LCD_BackLight(1);
	I2C_LCD_Puts("DHT11:");
	I2C_LCD_WriteCmd(0x8C);
	LCD_Write_Chr(0xDF);
	I2C_LCD_Puts("C");
	I2C_LCD_NewLine();
	I2C_LCD_Puts("DS18B20:");
	I2C_LCD_WriteCmd(0xCC);
	LCD_Write_Chr(0xDF);
	I2C_LCD_Puts("C");
}

static void vSetTimeTask( void *pvParameters )
{
	for( ;; )
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		if(RxCounter == 1)
		{
			if(Flag == 0)
			{
				if(RxBuffer[0] == 's')
				{
					printf("\r\n***************________MENU________***************");
					Menu();
				}else
				{
					RxCounter = 0;
				}
			}
		}else if(RxCounter == 2)
		{
			ChangePeriod();
		}else if(RxCounter > 2)
		{
			NewPeriod();
			if(Flag == 1)
			{
				if(Flag2 == 1)
				{
					Time1 = (Time[0]*10 + Time[1]) * 1000;
				}else
				{
					Time1 = Time[0] * 1000;
				}
			}else if(Flag == 2)
			{
				if(Flag2 == 1)
				{
					Time2 = (Time[0]*10 + Time[1]) * 1000;
				}else
				{
					Time2 = Time[0] * 1000;
				}
			}
		}
	}
}

static void Menu(void)
{
			vTaskSuspend(xGetDataDHT11Task);
			vTaskSuspend(xGetDataDS18B20Task);
			vTaskSuspend(xDisplayLCDTask);
			vTaskSuspend(xSendToPCTask);
			printf("\r\nPeriod of vGetDataDHT11Task current:%d(s)\
			\r\nPeriod of vGetDataDS18B20Task current:%d(s)", Time1/1000, Time2/1000);
			printf("%s",menu);
}

static void ChangePeriod(void)
{
	switch(RxBuffer[1])
		{
			case '0':
			vTaskResume(xGetDataDHT11Task);
			vTaskResume(xGetDataDS18B20Task);
			vTaskResume(xDisplayLCDTask);
			vTaskResume(xSendToPCTask);
			Flag = 0;
			RxCounter = 0;
			printf("\r\nChange success!!!");
			printf("\r\n**************************************************");
			break;
			case '1':
				printf("\r\nChange period of vGetDataDHT11Task-->New value(s):");
			Flag = 1;
				break;
			case '2':
				printf("\r\nChange period of vGetDataDS18B20Task-->New value(s):");
			Flag = 2;
				break;
			default:
				printf("\r\nReply");
				printf("\r\nType your opption here :");
				RxCounter = 1;
			Flag = 5;
		}
}

static void NewPeriod(void)
{
	if(RxCounter == 3)
	{
		if((0 < (RxBuffer[2]-48)) && ((RxBuffer[2]-48) <= 9))
		{
			Time[0] = RxBuffer[2]-48;
		}
		else
		{
			printf("\r\nReply");
			printf("\r\nNew value:");
			RxCounter = 2;
		}
	}else if(RxCounter == 4)
	{
		if((0 <= (RxBuffer[3]-48)) && ((RxBuffer[3]-48) <= 9))
		{
			Time[1] = RxBuffer[3]-48;
			Menu();
			RxCounter = 1;
			Flag2 = 1;
		}else if(RxBuffer[3] == '\r')
		{
			Menu();
			RxCounter = 1;
			Flag2 = 0;
		}else
		{
			printf("\r\nReply");
			printf("\r\nNew value:");
			RxCounter = 2;
		}
	}
}

void vApplicationIdleHook(void)
{ 
	__WFI(); 
}
