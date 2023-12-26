#ifndef __DEFINE__H
#define __DEFINE__H

#include "stm32f10x.h"

/***********以下参数与BPC灵敏度相关,最好参考统计学规律***********/
//Sys_Tick重装值
//720000--->10ms一次中断
//1440000-->20ms一次中断
#define Sys_Tick_Reload                   720000
//99--->10ms一次中断(最大不超过2^32)
//49---->20ms一次中断
#define Sys_Tick_Period                   99
//判断帧起始标志位的阈值,越大表示条件越严格,取值范围:0~~Sys_Tick_Period
#define Sys_Tick_Threshold                97
//四进制"0"阈值,取值范围:0~~Sys_Tick_Period*0.15
#define Zero_Low_Threshold                5
#define Zero_High_Threshold               14
//四进制"1"阈值,取值范围:Sys_Tick_Period*0.15~~Sys_Tick_Period*0.25
#define One_Low_Threshold                 15
#define One_High_Threshold                24
//四进制"2"阈值,取值范围:Sys_Tick_Period*0.25~~Sys_Tick_Period*0.35
#define Two_Low_Threshold                 25
#define Two_High_Threshold                34
//四进制"3"阈值,取值范围:Sys_Tick_Period*0.35~~Sys_Tick_Period*0.45
#define Three_Low_Threshold               35
#define Three_High_Threshold              44
/***********以上参数与BPC灵敏度相关,最好参考统计学规律***********/

/***********以下参数与BPC帧格式相关,最好参考BPC解码标准**********/
//BPC每帧有效数据位数(一般不变)
#define BPC_EFFECT_NUM                    19
//BPC有效数据种数(一般不变)
#define BPC_EFFECT_DATA                   8
//BPC第一阶段有效数据位数(一般不变)
#define BPC_First_Stage                   9
//BPC第二阶段有效数据位数(一般不变)
#define BPC_Second_Stage                  8
/***********以上参数与BPC帧格式相关,最好参考BPC解码标准**********/

//DEBUG使用USART1,连接的APB2时钟72M,其在物理上没有连接CH340,需要USB转串口模块
// 串口1-USART1
#define  DEBUG_USART                    USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO引脚宏定义
//STM32  CH340
//A9---->RXD
//A10--->TXD
#define  DEBUG_USART_GPIO_CLK           RCC_APB2Periph_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

//电波钟NTCO输入端口
//STM32  CME6005
//A8<----NTCO
//GND----GND
#define  NTCO_GPIO_CLK                  RCC_APB2Periph_GPIOA
#define  NTCO_GPIO_PORT                 GPIOA
#define  NTCO_GPIO_PIN                  GPIO_Pin_8

#endif
