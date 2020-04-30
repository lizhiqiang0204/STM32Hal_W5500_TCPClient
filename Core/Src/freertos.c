/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "usart.h"
#include "w5500_conf.h"
#include "w5500.h"
#include "tcp_demo.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTaskLedHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

uint8_t ConnectTcpServer();

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTaskLed(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTaskLed */
  osThreadDef(myTaskLed, StartTaskLed, osPriorityIdle, 0, 128);
  myTaskLedHandle = osThreadCreate(osThread(myTaskLed), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */

	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/
	set_w5500_ip();											/*配置IP地址*/
//
	socket_buf_init(txsize, rxsize);		/*初始�??8个Socket的发送接收缓存大�??*/
  /* Infinite loop */
	for(;;)
	{
		do_tcp_client();                  /*TCP_Client 数据回环测试程序*/
		osDelay(10);
	}

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskLed */
/**
* @brief Function implementing the myTaskLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLed */
void StartTaskLed(void const * argument)
{
  /* USER CODE BEGIN StartTaskLed */
  /* Infinite loop */
  for(;;)
  {
//		HAL_Delay(500);
		osDelay(500);
		HAL_GPIO_TogglePin(GPIOB,LED2_Pin|LED3_Pin);
  }
  /* USER CODE END StartTaskLed */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

// 连接TCP服务器函�??????????
// 返回�?????????? 1：连接失败，0：连接成�??????????
 uint8_t ConnectTcpServer()
 {
	 uint8_t count = 0;
	  bWiFiResponse = False;
	  count = 0;
	  USART_SendString(&huart2,"AT+CIPSTART=\"TCP\",\"192.168.2.155\",8080\r\n");//连接TCP服务�??????????
	  while(bWiFiResponse == False)
	  {
		  count++;
		  if(count >= 5)
		  {
			  USART_SendString(&huart1,"connect TCP server time out!!!\r\n");
			  return 1;
		  }
		  osDelay(2000);
		  USART_SendString(&huart2,"AT+CIPSTART=\"TCP\",\"192.168.2.155\",8080\r\n");//重发连接TCP服务器命�??????????
	  }

	  bWiFiResponse = False;
	  count = 0;
	  USART_SendString(&huart2,"AT+CIPMODE=1\r\n");//�??????????启�?�传模式
	  while(bWiFiResponse == False)
	  {
		  count++;
		  if(count >= 5)
		  {
			  USART_SendString(&huart1,"Open the passthrough time out!!!\r\n");
			  return 1;;
		  }
		  osDelay(2000);
		  USART_SendString(&huart2,"AT+CIPMODE=1\r\n");// 重发�??????????启�?�传模式
	  }

	  bWiFiResponse = False;
	  count = 0;
	  USART_SendString(&huart2,"AT+CIPSEND\r\n");//�??????????始传�??????????
	  while(bWiFiResponse == False)
	  {
		  count++;
		  if(count >= 5)
		  {
			  USART_SendString(&huart1,"Began to transport time out!!!\r\n");
			  return 1;;
		  }
		  osDelay(2000);
		  USART_SendString(&huart2,"AT+CIPSEND\r\n");//重发�??????????始传输命�??????????

	  }
	  return 0;
 }
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
