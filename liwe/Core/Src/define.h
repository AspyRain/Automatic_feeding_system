#ifndef __DEFINE__H
#define __DEFINE__H


#define Bit_RESET 0
#define Bit_SET   1
/***********以下参数与BPC灵敏度相关,最好参考统计学规律***********/
// RT-Thread SysTick配置
#define RT_TICK_MS                      (1000 / RT_TICK_PER_SECOND) // 计算每个时钟节拍的毫秒数

// SysTick重装值
#define Sys_Tick_Reload                  10 // 将时钟节拍转换为10ms一次中断的 SysTick 重装值
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


//电波钟NTCO输入端口
//STM32  CME6005
//A4<----NTCO
//GND----GND
#define  NTCO_GPIO_CLK                  RCC_APB2Periph_GPIOA

#endif
