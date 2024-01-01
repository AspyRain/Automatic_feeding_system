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
#include "rtthread.h"
#include "esp01s.h"
#include "feeding.h"
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"
#include "timer.h"
#include <stdio.h>
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void parseAndProcessCommand(char *command);
void reciveData();
void processJson(void *parameter);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char usart1_c;
int dataLen;
char dataLenStr[dataMaxLen];
char *realCommand;
char *jsonMessage;
Plan *plans = NULL;
Device *devices = NULL;
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
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1_c, 1);
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */
  Esp01s_Init("AspyRain", "Lxr20030106", "8080");
  /* USER CODE END 2 */
  // 例子插入一个计划
  insertPlan(&plans, 1, *newTime(10, 5, 10), 5, *newDate(2023, 12, 27), *newDate(2023, 12, 28));
  insertDevice(&devices, "feed_1");
  insertDevice(&devices, "feed_2");
  insertDevice(&devices, "feed_3");
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    rt_thread_mdelay(20);
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void parseAndProcessCommand(char *command)
{
  // 在这里解析和处理指令
  if (command != NULL)
  {
    // 在这里处理提取出的值
    // 将提取出的值存储到全局变量中
    rt_kprintf("%s\n", command);
    jsonMessage = (char *)malloc(strlen(command) + 1);
    strcpy(jsonMessage, command);

    rt_kprintf("jsonMessage=%s\n", jsonMessage);
    // 创建线程并传递 command 参数
    rt_thread_t processJsonThread = rt_thread_create("processJson", processJson, RT_NULL, 2048, 5, 10);
    if (processJsonThread != RT_NULL)
    {
      // 启动线程
      rt_thread_startup(processJsonThread);
    }
  }
}

void processJson(void *parameter)
{
  int status;
  int type;
  int device;
  int h, m, s;
  int duration;
  int beginYear, beginMonth, beginDay;
  int endYear, endMonth, endDay;
  // 使用sscanf进行简单的解析

  // 使用sscanf进行简单的解析
  int parsedItems = sscanf(jsonMessage, "{status:%d,", &status);

  // 检查解析是否成功
  if (parsedItems != 1)
  {
    // 解析失败，可以输出错误信息或进行其他处理
    rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
    // 释放内存
    free(jsonMessage);
    return;
  }

  // 根据不同的status执行不同的操作
  switch (status)
  {
  case 0:
  {
    rt_kprintf("进入status: 0\n");
    rt_thread_mdelay(20);
    char *dataString;
    parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{type:%d", &type);
    if (parsedItems != 1)
    {
      rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
      // 释放内存
      free(jsonMessage);
      return;
    }
    rt_kprintf("type: %s\n", type);
    switch (type)
    {
    case 0:
    {
      rt_kprintf("进入type: 0\n");
      deviceToString(&devices, deviceCount, &dataString);
      break;
    }
    case 1:
    {
      rt_kprintf("进入type: 1\n");
      planToString(&plans, planCount, &dataString);
      break;
    }
    default:
      break;
    }
    rt_kprintf("main函数执行espSend\n");
    espSend(dataString);
    rt_kprintf("释放先前分配的内存\n");
    free(dataString);
    break;
  }
  case 1:
  {
    rt_kprintf("进入status: 1\n");
    // 类型1的处理
    // 使用sscanf解析详细信息
    parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{type:%d,", &type);
    if (parsedItems != 1)
    {
      rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
      // 释放内存
      free(jsonMessage);
      return;
    }
    switch (type)
    {
    case 0:
    {
      rt_kprintf("进入type: 0\n");
      char *deviceName = malloc(30); // 假设 MAX_DEVICE_NAME_LENGTH 是足够大的一个常数
      if (deviceName == NULL)
      {
        // 处理内存分配失败的情况
        rt_kprintf("Memory allocation failed\n");
        return;
      }

      parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{type:%*d,device:{deviceName:%s}}}", deviceName);
      if (parsedItems != 1)
      {
        rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
        // 释放内存
        free(deviceName);
        free(jsonMessage);
        return;
      }

      insertDevice(&devices, deviceName);
      rt_kprintf("device写入完成 ,id:%d\n",devices[deviceCount-1].id);
      // 释放内存
      free(deviceName);
      break;
    }
    case 1:
    {
      parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{type:%*d,{device:%d,Time:{h:%d,m:%d,s:%d},duration:%d,beginDate:{year:%d,month:%d,day:%d},endDate:{year:%d,month:%d,day:%d}}}}",
                           &device, &h, &m, &s, &duration, &beginYear, &beginMonth, &beginDay, &endYear, &endMonth, &endDay);
      // 检查解析是否成功
      if (parsedItems != 11)
      {
        rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
        // 释放内存
        free(jsonMessage);
        return;
      }
      // 在这里执行新建计划的操作，你可以将解析得到的数据传递给相应的函数
      insertPlan(&timerData->plans, device, *newTime(h, m, s), duration, *newDate(beginYear, beginMonth, beginDay), *newDate(endYear, endMonth, endDay));
      break;
    }
    default:
      break;
    }

    break;
  }
  case 2:
  {
    rt_kprintf("进入status: 2\n");
    // 类型2的处理
    // 使用sscanf解析详细信息
    parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{device:%d}}", &device);

    // 检查解析是否成功
    if (parsedItems != 1)
    {
      rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
      // 释放内存
      free(jsonMessage);
      return;
    }

    // 在这里执行启动投喂器的操作，你可以将解析得到的设备编号传递给相应的函数
    if (device > 0 && device < 4)
    {
      toggle_feed(device);
    }
    break;
  }
  }

  // 处理完毕后，释放内存
  free(jsonMessage);
}

void reciveData()
{
  if (usart1_rx_index == -1)
    usart1_rx_index++;
  else
  {
    if (esp_command_flag == 0)
    {
      usart1_rx_buffer[usart1_rx_index++] = usart1_c;
    }
    else if (esp_command_flag == 1)
    {
      if (usart1_rx_index < dataMaxLen)
      {
        dataLenStr[usart1_rx_index++] = usart1_c;
      }
      else
      {
        rt_kprintf("data too long!\n");
      }
    }
    else if (esp_command_flag == 2)
    {
      if (usart1_rx_index < dataLen)
      {
        realCommand[usart1_rx_index++] = usart1_c;
      }
      else
      {
        rt_kprintf("data error!\n");
      }
    }
  }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
    if (esp_command_flag == 3)
      esp_command_flag = 0;
    if (usart1_rx_index >= BUFFER_SIZE - 1)
    {
      memset(usart1_rx_buffer, 0, sizeof(usart1_rx_buffer));
      usart1_rx_index = 0;
    }
    if (esp_flag == 1)
    {
      if (usart1_c == '+')
      {
        // 开始接收一条新指令
        memset(usart1_rx_buffer, 0, sizeof(usart1_rx_buffer));
        usart1_rx_index = 0;
      }
      else if (strstr((const char *)usart1_rx_buffer, "+IPD,0,") != NULL)
      {
        // 收到数据段长度
        esp_command_flag = 1;
        // 清空接收缓冲区，准备接收下一条指令
        usart1_rx_index = 0;
        memset(usart1_rx_buffer, 0, sizeof(usart1_rx_buffer));
      }
      else if (esp_command_flag == 1 && usart1_c == ':')
      {
        esp_command_flag = 2;
        dataLen = atoi(dataLenStr);
        usart1_rx_index = -1;
        memset(dataLenStr, 0, sizeof(dataLenStr));
        realCommand = (char *)malloc((dataLen * sizeof(char)) + 1);
        realCommand[dataLen] = '\0';
      }
      else if (usart1_rx_index == dataLen - 1 && esp_command_flag == 2)
      {
        reciveData();
        parseAndProcessCommand(realCommand);
        esp_command_flag = 3;
        usart1_rx_index = -1;
        // 释放先前分配的内存
        free(realCommand);
        // 将指针设置为 NULL
        realCommand = NULL;
      }
    }
    reciveData();
    // 继续启动下一次接收
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1_c, 1);
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

#ifdef USE_FULL_ASSERT
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