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
#include "BPC_DECODE.h"
#include "oled.h"
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
void timerThreadEntry(void *pmt);
void timeInit(void *pmt);

void handleStateZero(void *pmt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char usart1_c;
int dataLen;
char dataLenStr[dataMaxLen];
char *realCommand;
char *jsonMessage;
int AT_OK_Flag = 0;
int AT_Ready_Flag = 0;
int AT_Connect_Net_Flag = 0;
char *status = "初始";
int checkReciveFlag = 0;
DeviceList *deviceList = NULL;
/* USER CODE END 0 */
void initESP(void *pmt)
{
  Esp01s_Init("AspyRain", "Lxr20030106", "8080");
}
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
  OLED_Init();
  OLED_Clear();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1_c, 1);
  OLED_ShowStart();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */
  // 获取初始时间
  checkReciveFlag = 1;
  rt_thread_t checkRecive = rt_thread_create("checkRecive", handleStateZero, RT_NULL, 1024, 1, 10);
  if (checkRecive != RT_NULL)
  {
    rt_thread_startup(checkRecive);
    rt_kprintf("初始化ESP01S中\n");
    Esp01s_Init("AspyRain", "Lxr20030106", "8080");
  }

  /* USER CODE END 2 */
  // 例子插入一个计划

  newPlanList(&planList);
  newDeviceList(&deviceList);
  insertPlan(planList, 1, *newTime(10, 5, 10), 5, *newDate(2023, 12, 27), *newDate(2023, 12, 28));
  insertPlan(planList, 2, *newTime(14, 19, 10), 20, *newDate(2024, 1, 7), *newDate(2024, 12, 29));

  insertDevice(deviceList, "feed_1");
  insertDevice(deviceList, "feed_2");
  insertDevice(deviceList, "feed_3");
  timerInit(newDate(2024, 1, 8), newTime(14, 18, 50), planList);

  //  getCurrentTime(1);
  rt_thread_t timerThread = rt_thread_create("timerThread", timerThreadEntry, RT_NULL, 2048, 3, 10);
  if (timerThread != RT_NULL)
  {
    rt_thread_startup(timerThread);
  }
  rt_thread_t showTimeThread = rt_thread_create("showTimeThread", OLED_Showtime, RT_NULL, 1024, 3, 10);
  if (showTimeThread != RT_NULL)
  {
    is_Change[0] = 1;
    is_Change[1] = 1;
    is_Change[2] = 1;
    is_Change[3] = 1;
    is_Change[4] = 1;
    is_Change[5] = 1;
    rt_thread_startup(showTimeThread);
  }
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
    rt_kprintf("创建jsonMessage\n");
    jsonMessage = (char *)malloc(strlen(command) + 1);
    rt_kprintf("创建OK\n");
    rt_kprintf("赋值jsonMessage\n");
    if (jsonMessage != NULL && command != NULL)
    {
      strncpy(jsonMessage, command, strlen(command) + 1);
    }
    rt_kprintf("复制成功\n");
    rt_kprintf("jsonString:%s\n", jsonMessage);
    if (jsonMessage != NULL)
    {
      rt_kprintf("进入处理线程\n");

      // 创建线程并传递 command 参数
      rt_thread_t processJsonThread = rt_thread_create("processJson", processJson, RT_NULL, 3096, 3, 10);
      if (processJsonThread != RT_NULL)
      {
        // 启动线程
        rt_thread_startup(processJsonThread);
      }
    }
  }
}

void timerThreadEntry(void *pmt)
{
  int result;
  while (1)
  {
    result = timerRun();
    if (result == 1)
    {
      getCurrentTime(0);
    }
    rt_thread_mdelay(1000);
  }
}
void processJson(void *parameter)
{
  int status;
  int type;
  int device;
  int h, m;
  int duration;
  int beginMonth, beginDay;
  int endMonth, endDay;
  // 使用sscanf进行简单的解析

  // 使用sscanf进行简单的解析
  int parsedItems = sscanf(jsonMessage, "{status:%d,", &status);
  rt_kprintf("status:%d\n", status);
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
    rt_thread_mdelay(20);
    char *dataString;
    parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{type:%d}", &type);
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
      deviceToString(deviceList, &dataString);
      espSend(dataString, 1);
      free(dataString);
      break;
    }
    case 1:
    {
      sendPlan(planList);
      break;
    }
    default:
      break;
    }

    break;
  }
  case 1:
  {
    rt_kprintf("进入status:1\n");
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

      insertDevice(deviceList, deviceName);
      // 释放内存
      free(deviceName);
      break;
    }
    case 1:
    {
      rt_kprintf("type:1\n");
      parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{type:%*d,{%d,:%d,%d.%d,:%d,%d.:%d,%d}}}",
                           &device, &h, &m, &duration, &beginMonth, &beginDay, &endMonth, &endDay);
      // 检查解析是否成功
      if (parsedItems != 8)
      {
        rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
        // 释放内存
        free(jsonMessage);
        return;
      }
      rt_kprintf("获取完plan数据:start:%d:%d,during:%d月%d日-%d月%d日\n", h, m, beginMonth, beginDay, endMonth, endDay);
      // 在这里执行新建计划的操作，你可以将解析得到的数据传递给相应的函数
      insertPlan(planList, device, *newTime(h, m, 0), duration, *newDate(0, beginMonth, beginDay), *newDate(0, endMonth, endDay));
      espSend("OK\n", 1);
      break;
    }
    default:
      break;
    }

    break;
  }
  case 2:
  {

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
  case 3:
  {
    int id;
    parsedItems = sscanf(jsonMessage, "{status:%*d,detail:{type:%d,id:%d}}", &type, &id);
    if (parsedItems != 2)
    {
      rt_kprintf("Error parsing JSON: %s\n", jsonMessage);
      // 释放内存
      free(jsonMessage);
      return;
    }
    if (type == 0)
    {
      deleteDeviceById(deviceList, id);
    }
    else if (type == 1)
    {
      deletePlanById(planList, id);
    }
    break;
  }
  }
  // 处理完毕后，释放内存
  free(jsonMessage);
}
void clearUsart()
{
  memset(usart1_rx_buffer, 0, sizeof(usart1_rx_buffer));
  usart1_rx_index = 0;
}
void handleStateZero(void *pmt)
{
  while (checkReciveFlag == 1)
  {
    if (strstr((const char *)usart1_rx_buffer, "WIFI GOT IP") != NULL)
    {
      AT_Connect_Net_Flag = 1;
      clearUsart();
    }

    if (strstr((const char *)usart1_rx_buffer, "OK") != NULL)
    {
      AT_OK_Flag = 1;
      clearUsart();
    }

    if (strstr((const char *)usart1_rx_buffer, "ready") != NULL)
    {
      AT_Ready_Flag = 1;
      clearUsart();
    }
    rt_thread_mdelay(20);
  }
}
void reciveData()
{
  if (usart1_rx_index == -1)
    usart1_rx_index++;
  else
  {
    switch (esp_command_flag)
    {
    case 1:

      if (usart1_rx_index < dataMaxLen)
      {
        dataLenStr[usart1_rx_index++] = usart1_c;
      }
      else
      {
        rt_kprintf("data too long!\n");
      }
      break;
    case 2:

      if (usart1_rx_index < dataLen)
      {
        realCommand[usart1_rx_index++] = usart1_c;
      }
      else
      {
        rt_kprintf("data error!\n");
      }
      break;
    default:
      usart1_rx_buffer[usart1_rx_index++] = usart1_c;
      break;
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
      clearUsart();
    }
    if (esp_flag == 1)
    {
      if (usart1_c == '+')
      {
        clearUsart();
      }
      else if (strstr((const char *)usart1_rx_buffer, "+IPD,0,") != NULL) // 获取到报文开头，准备获取数据长度
      {
        // 收到数据段长度
        esp_command_flag = 1;
        // 清空接收缓冲区，准备接收下一条指令
        clearUsart();
      }
      else if (esp_command_flag == 1 && usart1_c == ':') // 获取真实数据
      {
        esp_command_flag = 2;
        dataLen = atoi(dataLenStr);
        usart1_rx_index = -1;
        memset(dataLenStr, 0, sizeof(dataLenStr));
        realCommand = (char *)malloc((dataLen * sizeof(char)) + 1);
        realCommand[dataLen] = '\0';
      }
      else if (usart1_rx_index == dataLen - 1 && esp_command_flag == 2) // 处理数据
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