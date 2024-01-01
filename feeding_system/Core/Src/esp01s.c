#include "esp01s.h"
#include "usart.h"
#include "rtthread.h"
#include "stm32f1xx_hal.h"
#include "stdlib.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "feeding.h"
#define WAIT_TIMEOUT_MS 5000 
void Esp01s_Init(char* ip, char* password, char* port) {

  char command[50];
  // 发送初始化指令到ESP01S
	sendData(&huart1,"AT+RST\r\n");
  wait_for_esp("OK");  // 等待一段时间
  rt_thread_mdelay(3000);

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
void wait_for_esp(char* key) {
  rt_kprintf("key:%s\n", key);
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
  timeout_flag = 1;
  rt_kprintf("Timeout waiting for response: %s\n", key);
}

void espSend(char* message) {
  char command[50];
  addNewlineIfNeeded(message);
   rt_kprintf("message:%s\n",message);
  sprintf(command, "AT+CIPSEND=0,%d\r\n", strlen(message));
  rt_kprintf("esp函数执行send\n");
  sendData(&huart1, command);
  wait_for_esp("OK");
  rt_kprintf("发送数据\n");
  sendData(&huart1, message);
  wait_for_esp("OK");
  rt_kprintf("发送完成\n");
}

void addNewlineIfNeeded(char* message) {
    // 获取字符串长度
    size_t len = strlen(message);

    // 判断末尾是否为 '\n'
    if (len > 0 && message[len - 1] != '\n') {
        // 在末尾添加 '\n'
        strcat(message, "\n");
    }
}


