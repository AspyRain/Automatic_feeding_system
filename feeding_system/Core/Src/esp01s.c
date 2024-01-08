#include "esp01s.h"
#include "usart.h"
#include "rtthread.h"
#include "stm32f1xx_hal.h"
#include "stdlib.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "feeding.h"
#include "data_structure.h"

#define WAIT_TIMEOUT_MS 5500
void Esp01s_Init(char *ip, char *password, char *port)
{
  char command[50];
  // // 发送初始化指令到ESP01S
  // status = "AT+RST";
  // sendData(&huart1, "AT+RST\r\n");
  // while (AT_OK_Flag == 0)
  // {
  //   rt_thread_mdelay(50);
  // }
  // AT_OK_Flag = 0;
  // rt_thread_mdelay(1000);
  status = "AT+CWMODE=1";
  sendData(&huart1, "AT+CWMODE=1\r\n");
  while (AT_OK_Flag == 0)
  {
    rt_thread_mdelay(50);
  }
  AT_OK_Flag = 0;
  status = "AT+CWJAP";
  sprintf(command, "AT+CWJAP=\"%s\",\"%s\"\r\n", ip, password);
  sendData(&huart1, command);
  while (AT_OK_Flag == 0)
  {
    rt_thread_mdelay(50);
  }
  AT_OK_Flag = 0;
  while (AT_Connect_Net_Flag == 0)
  {
    rt_thread_mdelay(50);
  }
  AT_Connect_Net_Flag = 0;
  status = "AT+CIPMUX=1";
  sprintf(command, "AT+CIPMUX=1\r\n");
  sendData(&huart1, command);
  while (AT_OK_Flag == 0)
  {
    rt_thread_mdelay(50);
  }
  AT_OK_Flag = 0;
  status = "AT+CIPSERVER=1";
  sprintf(command, "AT+CIPSERVER=1,%s\r\n", port);
  sendData(&huart1, command);
  while (AT_OK_Flag == 0)
  {
    rt_thread_mdelay(50);
  }
  AT_OK_Flag = 0;
  esp_flag = 1;
  HAL_GPIO_WritePin(esp_enable_flag_GPIO_Port, esp_enable_flag_Pin, GPIO_PIN_SET);
  // 清空接收缓冲区
  huart1.Instance->DR; // 读取数据寄存器，将接收缓冲区中的数据清空
  // 重新启用USART1接收中断
  rt_kprintf("初始化完成\n");
}

void espSend(char *message, int enterFlag)
{
  char command[50];
  if (enterFlag == 1)
  {
    addNewlineIfNeeded(message);
  }
  rt_kprintf("message:%s\n", message);
  sprintf(command, "AT+CIPSEND=0,%d\r\n", strlen(message));
  rt_kprintf("esp函数执行send\n");
  sendData(&huart1, command);
  while (AT_OK_Flag == 0)
    rt_thread_mdelay(50);
  AT_OK_Flag = 0;
  rt_kprintf("发送数据\n");
  sendData(&huart1, message);
  while (AT_OK_Flag == 0)
    rt_thread_mdelay(50);
  AT_OK_Flag = 0;
  rt_kprintf("发送完成\n");
}
void sendPlan(PlanList *planList)
{
  rt_kprintf("3.5\n");
  char command[50];
  // 计算 JSON 字符串所需的大小
  size_t requiredSize = strlen("["); // 用于初始 '['
  rt_kprintf("计算 JSON 字符串所需的大小\n");
  for (size_t i = 0; i < planList->length; ++i)
  {
    Plan *plan = &(planList->plans[i]);
    // 添加每个计划所需的大小
    requiredSize += snprintf(NULL, 0, "{\"id\": %d,\"device\": %d,\"time\": {\"h\": %d,\"m\": %d,\"s\": %d},\"duration\": %d,\"beginDate\": {\"year\": %d,\"month\": %d,\"day\": %d},\"endDate\": {\"year\": %d,\"month\": %d,\"day\": %d}}%s",
                             plan->id, plan->device, plan->time.h, plan->time.m, plan->time.s,
                             plan->duration, plan->beginDate.year, plan->beginDate.month, plan->beginDate.day,
                             plan->endDate.year, plan->endDate.month, plan->endDate.day,
                             (i < planList->length - 1) ? "," : "");
  }
  requiredSize += strlen("]\n");
  sprintf(command, "AT+CIPSEND=0,%d\r\n", requiredSize);
  sendData(&huart1, command);
  rt_kprintf(command);
  while (AT_OK_Flag == 0)
    rt_thread_mdelay(50);
  AT_OK_Flag = 0;
  // 发送 '['
  char *openingBracket = "[";
  sendData(&huart1, openingBracket);
  rt_kprintf(openingBracket);
  // 逐个发送每个计划
  for (size_t i = 0; i < planList->length; ++i)
  {
    char planString[200];
    planToString(&(planList->plans[i]), planString);

    // 发送计划字符串
    sendData(&huart1, planString);
    rt_kprintf(planString);
    // 释放内存

    // 如果不是最后一个计划，发送逗号和空格
    if (i < planList->length - 1)
    {
      char *commaSpace = ",";
      sendData(&huart1, commaSpace);
      rt_kprintf(commaSpace);
    }
  }

  // 发送 ']'
  char *closingBracket = "]\n";
  sendData(&huart1, closingBracket);
  rt_kprintf(closingBracket);
  while (AT_OK_Flag == 0)
    rt_thread_mdelay(50);
  AT_OK_Flag = 0;
  rt_kprintf("发送完成\n");
}

void addNewlineIfNeeded(char *message)
{
  // 获取字符串长度
  size_t len = strlen(message);

  // 判断末尾是否为 '\n'
  if (len > 0 && message[len - 1] != '\n')
  {
    // 在末尾添加 '\n'
    strcat(message, "\n");
  }
}
