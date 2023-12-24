/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

//有效数据缓冲区
uint8_t Buff[BPC_EFFECT_NUM];
uint8_t p=0;
uint8_t temp=4;

//临时变量
uint8_t  flag=0;
uint32_t x   =0;
uint32_t y   =0;
uint32_t z   =0;
uint32_t i   =0;

//外部变量
extern uint8_t  flag1;
extern uint8_t  flag2;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if(flag)                     //获取NTCO引脚有效数据
	{
		if(p<BPC_EFFECT_NUM)
		{
			if(z<Sys_Tick_Period)
		  {
			  z++;
			  if(GPIO_ReadInputDataBit(NTCO_GPIO_PORT, NTCO_GPIO_PIN))
				  i++;                 //i取值范围:0~~Sys_Tick_Period
		  }
		  else
		  {
			  if(i>=Zero_Low_Threshold && i<=Zero_High_Threshold)
			  {
				  temp=0;              //四进制"0"
			  }
			  else if(i>=One_Low_Threshold && i<=One_High_Threshold)
			  {
				  temp=1;              //四进制"1"
			  }
			  else if(i>=Two_Low_Threshold && i<=Two_High_Threshold)
			  {
				  temp=2;              //四进制"2"
			  }
			  else if(i>=Three_Low_Threshold && i<=Three_High_Threshold)
			  {
				  temp=3;              //四进制"3"
			  }
			  else
			  {
				  temp=4;              //无意义
			  }
			  Buff[p]=temp;          //写入有效数据缓冲区
			  p++;
			  z=0;
			  i=0;
		  }
		}
		else
		{
			p=0;
			flag=0;
			flag2=1;
		}
	}
	else                         //等待帧起始标志位
	{
		if(x<Sys_Tick_Period)      //对NTCO引脚进行帧起始标志位采样
	  {
		  x++;
		  if(!GPIO_ReadInputDataBit(NTCO_GPIO_PORT, NTCO_GPIO_PIN))
			  y++;                   //y取值范围:0~~Sys_Tick_Period
	  }
	  else                       //采样周期结束，判断是否为帧起始标志位
	  {
		  if(y>=Sys_Tick_Threshold)//检测到帧起始标志位
			{
			  flag=1;
				flag1=1;
			}
			y=0;
		  x=0;
	  }
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
