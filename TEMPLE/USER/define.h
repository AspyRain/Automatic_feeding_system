#ifndef __DEFINE__H
#define __DEFINE__H

#include "stm32f10x.h"

/***********���²�����BPC���������,��òο�ͳ��ѧ����***********/
//Sys_Tick��װֵ
//720000--->10msһ���ж�
//1440000-->20msһ���ж�
#define Sys_Tick_Reload                   720000
//99--->10msһ���ж�(��󲻳���2^32)
//49---->20msһ���ж�
#define Sys_Tick_Period                   99
//�ж�֡��ʼ��־λ����ֵ,Խ���ʾ����Խ�ϸ�,ȡֵ��Χ:0~~Sys_Tick_Period
#define Sys_Tick_Threshold                97
//�Ľ���"0"��ֵ,ȡֵ��Χ:0~~Sys_Tick_Period*0.15
#define Zero_Low_Threshold                5
#define Zero_High_Threshold               14
//�Ľ���"1"��ֵ,ȡֵ��Χ:Sys_Tick_Period*0.15~~Sys_Tick_Period*0.25
#define One_Low_Threshold                 15
#define One_High_Threshold                24
//�Ľ���"2"��ֵ,ȡֵ��Χ:Sys_Tick_Period*0.25~~Sys_Tick_Period*0.35
#define Two_Low_Threshold                 25
#define Two_High_Threshold                34
//�Ľ���"3"��ֵ,ȡֵ��Χ:Sys_Tick_Period*0.35~~Sys_Tick_Period*0.45
#define Three_Low_Threshold               35
#define Three_High_Threshold              44
/***********���ϲ�����BPC���������,��òο�ͳ��ѧ����***********/

/***********���²�����BPC֡��ʽ���,��òο�BPC�����׼**********/
//BPCÿ֡��Ч����λ��(һ�㲻��)
#define BPC_EFFECT_NUM                    19
//BPC��Ч��������(һ�㲻��)
#define BPC_EFFECT_DATA                   8
//BPC��һ�׶���Ч����λ��(һ�㲻��)
#define BPC_First_Stage                   9
//BPC�ڶ��׶���Ч����λ��(һ�㲻��)
#define BPC_Second_Stage                  8
/***********���ϲ�����BPC֡��ʽ���,��òο�BPC�����׼**********/

//DEBUGʹ��USART1,���ӵ�APB2ʱ��72M,����������û������CH340,��ҪUSBת����ģ��
// ����1-USART1
#define  DEBUG_USART                    USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO���ź궨��
//STM32  CH340
//A9---->RXD
//A10--->TXD
#define  DEBUG_USART_GPIO_CLK           RCC_APB2Periph_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

//�粨��NTCO����˿�
//STM32  CME6005
//A4<----NTCO
//GND----GND
#define  NTCO_GPIO_CLK                  RCC_APB2Periph_GPIOA
#define  NTCO_GPIO_PORT                 GPIOA
#define  NTCO_GPIO_PIN                  GPIO_Pin_8

#endif
