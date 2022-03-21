#include "main.h"

uint8_t RxBuffer[20];
__IO uint8_t RxCounter = 0;

TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;
TaskHandle_t xTaskHandle3 = NULL;
TaskHandle_t xTaskHandle4 = NULL;

TimerHandle_t led_timer_handle = NULL;

void vTask1_menu_display(void *params);
void vTask2_cmd_handling(void *params);
void vTask3_cmd_processing(void *params);
void vTask4_uart_write(void *params);
uint8_t getCommandcode(uint8_t *buffer);
void getArguments(uint8_t *buffer);

static void GPIOLed_Config(void);
static void GPIONutNhan_Config(void);
void make_led_on(void);
void make_led_off(void);
void led_toggle_start(uint32_t duration);
void led_toggle_stop(void);
void read_led_status(char *task_msg);
void print_error_message(char *task_msg);
void led_toggle(TimerHandle_t xTimer);

QueueHandle_t command_queue = NULL;
QueueHandle_t uart_write_queue = NULL;

typedef struct APP_CMD
{
	uint8_t COMMAND_NUM;
	uint8_t COMMAND_ARGS[10];
}APP_CMD_t;

char menu[] = {"\
	\r\nLED_ON													----> 1\
	\r\nLED_OFF													----> 2\
	\r\nLED_TOGGLE											----> 3\
	\r\nLED_TOGGLE_OFF  								----> 4\
	\r\nLED_READ_STATUS 								----> 5\
	\r\nEXIT_APP												----> 0\
	\r\nType your opption here : "};

#define LED_ON						1
#define LED_OFF						2
#define LED_TOGGLE				3
#define LED_TOGGLE_OFF		4
#define LED_READ_STATUS 	5
	
int main(void)
{
	GPIOLed_Config();
	GPIONutNhan_Config();
	GPIOUsart1_Config();
	USART1_Config();
	command_queue = xQueueCreate(10,sizeof(APP_CMD_t*));
	uart_write_queue = xQueueCreate(10,sizeof(char*));
	if((command_queue != NULL) && (uart_write_queue != NULL))
	{
	xTaskCreate(vTask1_menu_display, "TASK1-MENU", 500, NULL, 1, &xTaskHandle1);
	xTaskCreate(vTask2_cmd_handling, "TASK2-CMD-HANDLING", 500, NULL, 2, &xTaskHandle2);
	xTaskCreate(vTask3_cmd_processing, "TASK3-CMD_PROCESS", 500, NULL, 2, &xTaskHandle3);
	xTaskCreate(vTask4_uart_write, "TASK4-UART-WRITE", 128, NULL, 2, &xTaskHandle4);
	vTaskStartScheduler();
	}else
	{
		USART_PutString(USART1, "Queue create fail");
	}
	while(1)
	{		
	  
	}
}

void vTask1_menu_display(void *params)
{
	char *pData = menu;
	while(1)
	{
		xQueueSend(uart_write_queue,&pData,portMAX_DELAY);
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}

void vTask2_cmd_handling(void *params)
{
	uint8_t command_code = 0;
	APP_CMD_t *new_cmd;
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		new_cmd = (APP_CMD_t*) pvPortMalloc(sizeof(APP_CMD_t));
		taskENTER_CRITICAL();
		command_code = getCommandcode(RxBuffer);
		new_cmd->COMMAND_NUM = command_code;
		getArguments(new_cmd->COMMAND_ARGS);
		taskEXIT_CRITICAL();
		xQueueSend(command_queue,&new_cmd,portMAX_DELAY);
	}
}

void vTask3_cmd_processing(void *params)
{
	APP_CMD_t *new_cmd;
	char task_msg[50];
	uint32_t toggle_duration = pdMS_TO_TICKS(500);
	while(1)
	{
		xQueueReceive(command_queue,(void*)&new_cmd,portMAX_DELAY);
		if(new_cmd->COMMAND_NUM == LED_ON)
		{
			make_led_on();
		}else if(new_cmd->COMMAND_NUM == LED_OFF)
		{
			make_led_off();
		}else if(new_cmd->COMMAND_NUM == LED_TOGGLE)
		{
			led_toggle_start(toggle_duration);
		}else if(new_cmd->COMMAND_NUM == LED_TOGGLE_OFF)
		{
			led_toggle_stop();
		}else if(new_cmd->COMMAND_NUM == LED_READ_STATUS)
		{
			read_led_status(task_msg);
		}else
		{
			print_error_message(task_msg);
		}
		vPortFree(new_cmd);
	}
}

void vTask4_uart_write(void *params)
{
	char *pData = NULL;
	while(1)
	{
		xQueueReceive(uart_write_queue,&pData,portMAX_DELAY);
		USART_PutString(USART1, pData);
	}
}

uint8_t getCommandcode(uint8_t *buffer)
{
	return buffer[0] - 48;
}

void getArguments(uint8_t *buffer)
{
	
}

static void GPIOLed_Config(void)
{
	GPIO_InitTypeDef GPIO_LED;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_13;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC, &GPIO_LED);
}

static void GPIONutNhan_Config(void)
{
	GPIO_InitTypeDef GPIO_NutNhan;
	GPIO_NutNhan.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_NutNhan.GPIO_Pin = GPIO_Pin_0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &GPIO_NutNhan);
}

void make_led_on(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
void make_led_off(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
void led_toggle(TimerHandle_t xTimer)
{
	if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}else
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
}
void led_toggle_start(uint32_t duration)
{
	if(led_timer_handle == NULL)
	{
		led_timer_handle = xTimerCreate("LED-TIMER",duration,pdTRUE,NULL,led_toggle);
	  xTimerStart(led_timer_handle,portMAX_DELAY);
	}else
	{
		xTimerStart(led_timer_handle,portMAX_DELAY);
	}
}
void led_toggle_stop(void)
{
	xTimerStop(led_timer_handle,portMAX_DELAY);
}
void read_led_status(char *task_msg)
{
	sprintf(task_msg, "\r\nLED status is : %d\r\n", !(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)));
	xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);
}
void read_rtc_info(char *task_msg)
{
}
void print_error_message(char *task_msg)
{
}
