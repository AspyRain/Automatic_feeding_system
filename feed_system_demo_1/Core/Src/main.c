/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <rtthread.h>
#include "stdio.h"
#include "string.h"
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

/* USER CODE BEGIN PV */
#define BUFFER_SIZE 200
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define BUFFER_SIZE 200

 char usart2_c;
char usart1_c;

uint8_t usart2_rx_buffer[BUFFER_SIZE];
uint8_t usart2_rx_index = 0;

uint8_t usart1_rx_buffer[BUFFER_SIZE];
uint8_t usart1_rx_index = 0;

int feedNumFlag=0;

void Esp01s_Init(char* ip, char* password,char* port);
void sendData(UART_HandleTypeDef *huart, const char *str) ;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
 
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  HAL_UART_Receive_IT(&huart1, (uint8_t*)usart1_c, 1);
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */
 Esp01s_Init("AspyRain","Lxr20030106","8080");

rt_thread_t usart_task_tid = rt_thread_create("controller",/* 线程名称 */
                            controller, RT_NULL,
                            1024, 3, 10); //
    if(usart_task_tid != RT_NULL)
    {
        /* 启动线程 */
        rt_thread_startup(usart_task_tid);
        rt_kprintf("usart thread is already started\n");
        
    }
    else
    {
        rt_kprintf("usart thread is not started\n");
    }
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    rt_thread_mdelay(500);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}
void Esp01s_Init(char* ip, char* password, char* port) {
  char command[50];

  // 发送初始化指令到ESP01S
  sprintf(command, "AT+CWJAP=\"%s\",\"%s\"\r\n", ip, password);
  sendData(&huart1, command);
  rt_thread_mdelay(1000);  // 等待一段时间

  sprintf(command, "AT+CIPMUX=1\r\n");
  sendData(&huart1, command);
  rt_thread_mdelay(1000);


  sprintf(command, "AT+CIPSERVER=1,%s\r\n", port);
  sendData(&huart1, command);
  rt_thread_mdelay(1000);
  HAL_GPIO_WritePin(esp_enable_flag_GPIO_Port,esp_enable_flag_Pin,GPIO_PIN_SET);
}
void sendData(UART_HandleTypeDef *huart, const char *str) {
  HAL_UART_Transmit(huart, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}
void controller(){
  while (1){
    
  }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart1) {
    // 在这里处理接收到的数据
    if (usart1_c == '+') {
      // 开始接收一条新指令
      usart1_rx_index = 0;
    }
    
    usart1_rx_buffer[usart1_rx_index++] = usart1_c;

    if (usart1_c == '\n') {
      // 收到一条完整的指令
      usart1_rx_buffer[usart1_rx_index] = '\0'; // 添加字符串结束符

      // 在这里解析指令
      parseAndProcessCommand(usart1_rx_buffer);

      // 清空接收缓冲区，准备接收下一条指令
      usart1_rx_index = 0;
    }
    
    // 继续启动下一次接收
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&usart1_c, 1);
  }
}

void parseAndProcessCommand(char *command) {
  // 在这里解析和处理指令
  // 例如，查找 +IPD,0,1: 后面的数据并去掉末尾的 0
  char *substring = strstr(command, "+IPD,0,1:");
  if (substring != NULL) {
    int value;
    sscanf(substring + strlen("+IPD,0,1:"), "%d,", &value);
    // 在这里处理提取出的值
    // 你可以将提取出的值存储到全局变量中或者进行其他逻辑操作
    // 这里的 value 就是你要的数据，例如这里的 3
    feedNumFlag = value;
  }
}





/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */