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
#include "feeding.h"
#include "stdlib.h"
#include "flash.h"
#include "cJSON.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */
#define WAIT_TIMEOUT_MS 5000 
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

uint8_t usart1_c; 
uint8_t usart2_c; 

uint8_t usart2_rx_buffer[BUFFER_SIZE];
uint8_t usart2_rx_index = 0;

uint8_t usart1_rx_buffer[BUFFER_SIZE];
uint8_t usart1_rx_index = 0;

int feedNumFlag = 0;
int esp_flag=0;//判断esp是否就绪
int timeout_flag=0;

void Esp01s_Init(char* ip, char* password, char* port);
void wait_for_esp(char* key);
void parseAndProcessCommand(char *command);
char* extractData(const char* input) ;

void sendData(UART_HandleTypeDef *huart, const char *str);
void flash_test();
void parse_json(void *promt) ;

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
  feeding_init();
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
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1_c, 1);
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */
  Esp01s_Init("AspyRain","Lxr20030106","8080");
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* code */
    rt_thread_mdelay(20);
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


void wait_for_esp(char* key) {
  if (key == NULL) {
    return;
  }

  uint32_t start_time = rt_tick_get();
  while (rt_tick_get() - start_time < WAIT_TIMEOUT_MS) {
    // Check if the key is present in the received data
    if (strstr((const char *)usart1_rx_buffer, key) != NULL) {
      // Key found, reset the buffer and return
      usart1_rx_index = 0;
      memset(usart1_rx_buffer, 0, sizeof(usart1_rx_buffer));
      return;
    }
    
    // Wait for a short duration before checking again
    rt_thread_mdelay(30);
  }
  timeout_flag=1;
  rt_kprintf("Timeout waiting for response: %s\n", key);
}
void Esp01s_Init(char* ip, char* password, char* port) {

  char command[50];
  // 发送初始化指令到ESP01S
	sendData(&huart1,"AT+RST\r\n");
  wait_for_esp("OK");  // 等待一段时间
  rt_thread_mdelay(2000);

	sendData(&huart1,"AT+CWMODE=1\r\n");
	if (timeout_flag==0)wait_for_esp("OK");  // 等待一段时间

  sprintf(command, "AT+CWJAP=\"%s\",\"%s\"\r\n", ip, password);
  sendData(&huart1, command);
	if (timeout_flag==0)wait_for_esp("OK");  // 等待一段时间

  sprintf(command, "AT+CIPMUX=1\r\n");
  sendData(&huart1, command);
  if (timeout_flag==0)wait_for_esp("OK"); 

  sprintf(command, "AT+CIPSERVER=1,%s\r\n", port);
  sendData(&huart1, command);
  if (timeout_flag==0)wait_for_esp("OK"); 
  esp_flag=1;
  if (timeout_flag==0){
  HAL_GPIO_WritePin(esp_enable_flag_GPIO_Port, esp_enable_flag_Pin, GPIO_PIN_SET);
  // 清空接收缓冲区
  huart1.Instance->DR; // 读取数据寄存器，将接收缓冲区中的数据清空
  // 重新启用USART1接收中断
  rt_kprintf("初始化完成\n");
  }else {
    while (1)
    {
      /* code */
      HAL_GPIO_TogglePin(esp_enable_flag_GPIO_Port,esp_enable_flag_Pin);
      rt_thread_mdelay(500);
    }
    
  }
}
void sendData(UART_HandleTypeDef *huart, const char *str) {
  HAL_UART_Transmit(huart, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart1) {
    if (usart1_rx_index>=BUFFER_SIZE-1){
      memset(usart1_rx_buffer,0,sizeof(usart1_rx_buffer));
      usart1_rx_index=0;
    }
    usart1_rx_buffer[usart1_rx_index++] = usart1_c;
     if (esp_flag==1){
        if (usart1_c == '+') {
      // 开始接收一条新指令
      usart1_rx_index = 0;
      usart1_rx_buffer[usart1_rx_index++] = usart1_c;
    }
    
    if (strstr((const char*)usart1_rx_buffer, "CLOSED") != NULL) {
      // 收到一条完整的指令
      usart1_rx_buffer[usart1_rx_index] = '\0'; // 添加字符串结束符

      // 在这里解析指令
      parseAndProcessCommand((char*)usart1_rx_buffer);


      // 清空接收缓冲区，准备接收下一条指令
      usart1_rx_index = 0;
      memset(usart1_rx_buffer,0,sizeof(usart1_rx_buffer));
    }
      }
    // 继续启动下一次接收
      HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1_c, 1);

  }

}
void parse_json(void *promt) {
    char jsonString[200];

    // 构建 JSON 字符串
    snprintf(jsonString, sizeof(jsonString),
             "{\"msg\":\"this is successful start up\",\"result\":1,\"action\":\"req_startUp\",\"responseData\":\"trustedDevice\",\"serial_no\":\"0341\",\"timestamp\":1656489886238}");

    // 使用 JSON 字符串
  char *output = NULL;
  cJSON *jsonData = NULL;
  rt_kprintf("Original string is: %s \n", jsonString);
  jsonData = cJSON_Parse(jsonString);
while (1){
  if (jsonData != NULL) {
    output = cJSON_Print(jsonData);
      
    rt_kprintf("Parsed string is: %s \n", output);
          // 释放 cJSON 对象
    cJSON_Delete(jsonData);
    
    // 释放输出字符串
    free(output);
    rt_thread_mdelay(20);
		break;
  }
}
}

void parseAndProcessCommand(char *command) {
  // 在这里解析和处理指令
  if (command != NULL) {
    char* data=extractData(command);
    // 在这里处理提取出的值
    // 将提取出的值存储到全局变量中
    feedNumFlag =atoi(data);
    
    if (feedNumFlag>0 && feedNumFlag<4){
     toggle_feed(feedNumFlag);
    }
    else if(feedNumFlag==4){
    rt_thread_t json_thread=rt_thread_create("json_thread", parse_json, RT_NULL, 2048, 4, 10);
    rt_thread_startup(json_thread);
    }
  }
}
char* extractData(const char* input) {
    // 找到真正数据的起始位置
    const char* dataStart = strstr(input, ":");
    if (dataStart == NULL) {
        // 如果没有找到冒号，返回空指针表示失败
        return NULL;
    }

    // 获取真正数据的长度
    int dataLength = atoi(input + 7);

    // 分配足够的空间来存储真正数据
    char* extractedData = (char*)malloc((dataLength + 1) * sizeof(char));

    // 复制真正数据到提取的字符串中
    strncpy(extractedData, dataStart + 1, dataLength);
    extractedData[dataLength] = '\0'; // 添加字符串终止符

    return extractedData;
}
void flash_test(){
  char* w_data = (char*)malloc(50);
  int i=0;
  while (1){
  sprintf(w_data, "this is data: %d",i++);
  flash_write_string(w_data);
  rt_kprintf("Write\n");
  rt_thread_mdelay(1000);
  rt_kprintf("read data is : %s \n",flash_read_string());
  rt_thread_mdelay(1000);
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
