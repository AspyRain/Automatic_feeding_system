#include "BPC_DECODE.h"
#include "rtthread.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "data_structure.h"
#include "timer.h"
#include "oled.h"
uint8_t Buff[BPC_EFFECT_NUM];
uint8_t p = 0;
uint8_t temp = 4;

uint8_t flag = 0;
uint32_t x = 0;
uint32_t y = 0;
uint32_t z = 0;
uint32_t i = 0;

uint8_t flag1 = 0; // 帧起始标志
uint8_t flag2 = 0; // 有效数据已满标志
int is_GetTime_Flag;

extern uint8_t flag1;
extern uint8_t flag2;

// 临时变量
uint8_t j;

// 解码时间缓冲区
uint8_t DATA[BPC_EFFECT_DATA];
// 解码原始的BPC数据
// 参数uint8_t *Buff:有效数据缓冲区起始
//     uint8_t *DATA:解码时间缓冲区起始
// 返回值:1 该帧有无效数据
//        2 有效数据第一阶段校验错误
//        3 有效数据第二阶段校验错误
//        4 解码成功
uint8_t BPC_DECODE(uint8_t *Buff, uint8_t *DATA) // 解码函数
{
	uint8_t i;
	uint8_t x = 0;

	/*******************清空解码时间缓冲区*******************/
	for (i = 0; i < BPC_EFFECT_DATA; i++)
	{
		*DATA = 0;
		DATA++;
	}
	DATA -= BPC_EFFECT_DATA; // 指向缓冲区起始
	/*******************清空解码时间缓冲区*******************/

	/*****************判断该帧是否有无效数据*****************/
	for (i = 0; i < BPC_EFFECT_NUM; i++) // 判断该帧是否有无效数据
	{
		if (*Buff == 4)
			return 1;
		Buff++;
	}
	/*****************判断该帧是否有无效数据*****************/

	/********************校验第一阶段数据********************/
	Buff -= BPC_EFFECT_NUM; // 指针指向第一段有效数据起始处
	for (i = 0; i < BPC_First_Stage; i++)
	{
		if (*Buff == 1 || *Buff == 2) // 一个"1"
			x += 1;
		else if (*Buff == 3) // 两个"1"
			x += 2;
		Buff++;
	}
	if (x % 2 == 0) // 偶
	{
		if (*Buff == 1 || *Buff == 3)
			return 2; // 第一段有效数据校验失败
	}
	else // 奇
	{
		if (*Buff == 0 || *Buff == 2)
			return 2; // 第一段有效数据校验失败
	}
	x = 0;
	/********************校验第一阶段数据********************/

	/********************校验第二阶段数据********************/

	Buff += 1; // 指针指向第二段有效数据起始处
	for (i = 0; i < BPC_Second_Stage; i++)
	{
		if (*Buff == 1 || *Buff == 2) // 一个"1"
			x += 1;
		else if (*Buff == 3) // 两个"1"
			x += 2;
		Buff++;
	}
	if (x % 2 == 0) // 偶
	{
		if (*Buff == 1 || *Buff == 3)
			return 3; // 第二段有效数据校验失败
	}
	else // 奇
	{
		if (*Buff == 0 || *Buff == 2)
			return 3; // 第二段有效数据校验失败
	}

	/********************校验第二阶段数据********************/

	/************************解码数据************************/
	// 秒
	Buff -= BPC_EFFECT_NUM - 1; // 指针指向"秒"数据起始处,开始解码数据
	if (*Buff == 0)				// 第1秒
		*DATA = 1;
	else if (*Buff == 1) // 第21秒
		*DATA = 21;
	else if (*Buff == 2) // 第41秒
		*DATA = 41;
	DATA++;

	// 时
	Buff += 2; // 指针指向"时"数据起始处
	*DATA += (*Buff) * 4;
	Buff++;
	*DATA += *Buff;
	DATA++;

	// 分
	Buff++; // 指针指向"分"数据起始处
	*DATA += (*Buff) * 16;
	Buff++;
	*DATA += (*Buff) * 4;
	Buff++;
	*DATA += *Buff;
	DATA++;

	// 星期
	Buff++; // 指针指向"星期"数据起始处
	*DATA += (*Buff) * 4;
	Buff++;
	*DATA += *Buff;
	DATA++;

	// 午别
	Buff++;						  // 指针指向"午别"数据起始处
	if (*Buff == 0 || *Buff == 1) // 上午
		*DATA = 0;
	else // 下午
		*DATA = 1;
	DATA++;

	// 日期
	Buff++; // 指针指向"日期"数据起始处
	*DATA += (*Buff) * 16;
	Buff++;
	*DATA += (*Buff) * 4;
	Buff++;
	*DATA += *Buff;
	DATA++;

	// 月份
	Buff++; // 指针指向"月份"数据起始处
	*DATA += (*Buff) * 4;
	Buff++;
	*DATA += *Buff;
	DATA++;

	// 年份
	Buff++; // 指针指向"年份"数据起始R处
	*DATA += (*Buff) * 16;
	Buff++;
	*DATA += (*Buff) * 4;
	Buff++;
	*DATA += *Buff;
	Buff++;
	if (*Buff == 2 || *Buff == 3)
		*DATA += 64;
	return 4;
	/************************解码数据************************/
}
void BPC_timer_callback(void *parameter)
{

	if (flag) // 获取NTCO引脚有效数据
	{
		if (p < BPC_EFFECT_NUM)
		{
			if (z < Sys_Tick_Period)
			{
				z++;
				if (GPIO_ReadInputDataBit(clock_tccn_GPIO_Port, clock_tccn_Pin))
					i++; // i取值范围:0~~Sys_Tick_Period
			}
			else
			{
				if (i >= Zero_Low_Threshold && i <= Zero_High_Threshold)
				{
					temp = 0; // 四进制"0"
				}
				else if (i >= One_Low_Threshold && i <= One_High_Threshold)
				{
					temp = 1; // 四进制"1"
				}
				else if (i >= Two_Low_Threshold && i <= Two_High_Threshold)
				{
					temp = 2; // 四进制"2"
				}
				else if (i >= Three_Low_Threshold && i <= Three_High_Threshold)
				{
					temp = 3; // 四进制"3"
				}
				else
				{
					temp = 4; // 无意义
				}
				Buff[p] = temp; // 写入有效数据缓冲区
				p++;
				z = 0;
				i = 0;
			}
		}
		else
		{
			p = 0;
			flag = 0;
			flag2 = 1;
		}
	}
	else // 等待帧起始标志位
	{
		if (x < Sys_Tick_Period) // 对NTCO引脚进行帧起始标志位采样
		{
			x++;
			if (!GPIO_ReadInputDataBit(clock_tccn_GPIO_Port, clock_tccn_Pin))
				y++; // y取值范围:0~~Sys_Tick_Period
		}
		else // 采样周期结束，判断是否为帧起始标志位
		{
			if (y >= Sys_Tick_Threshold) // 检测到帧起始标志位
			{
				flag = 1;
				flag1 = 1;
			}
			y = 0;
			x = 0;
		}
	}
}

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;

	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != GPIO_PIN_RESET)
	{
		bitstatus = (uint8_t)GPIO_PIN_SET;
	}
	else
	{
		bitstatus = (uint8_t)GPIO_PIN_RESET;
	}

	return bitstatus;
}
void startGetTime()
{
	rt_timer_t my_timer;
	int year, mouth, day, h, m, s;
	// 创建定时器
	my_timer = rt_timer_create("BPC_timer_callback", BPC_timer_callback, RT_NULL, Sys_Tick_Reload, RT_TIMER_FLAG_PERIODIC);
	rt_kprintf("timer is running!\n");
	// 启动定时器
	if (my_timer != RT_NULL)
	{
		rt_timer_start(my_timer);
	}

	while (1) // 等待Sys_Tick中断
	{
		if (flag1) // 检测到了帧起始标志位
		{
			flag1 = 0;
			rt_kprintf("Frame Head Detected\n");
		}
		else if (flag2) // 有效数据缓冲区已满
		{
			flag2 = 0;
			for (j = 0; j < BPC_EFFECT_NUM; j++)
				rt_kprintf("%d ", Buff[j]);
			rt_kprintf("\n");

			j = BPC_DECODE(Buff, DATA); // 解码原始数据
			if (j == 1)					// 该帧有无效数据
				rt_kprintf("Contain Invaild Data\n");
			else if (j == 2) // 有效数据第一阶段校验错误
				rt_kprintf("First Stage Verify Failed\n");
			else if (j == 3) // 有效数据第二阶段校验错误
				rt_kprintf("Second Stage Verify Failed\n");
			else if (j == 4) // 解码成功
			{
				year = DATA[7] + 2000;
				mouth = DATA[6];
				day = DATA[5];
				h = DATA[4] ? (DATA[1] + 12) : DATA[1];
				m = DATA[2];
				s = DATA[0];
				rt_kprintf("成功获取日期：\n%d年%d月%d日,%d:%d:%d\n", year, mouth, day, h, m, s);

				// 停止定时器
				rt_timer_stop(my_timer);

				// 删除定时器
				rt_timer_delete(my_timer);
				is_GetTime_Flag = 1;
				setTimerDate(newDate(year, mouth, day));
				setTimerTime(newTime(h, m, s));
				is_Change[0] = 1;
				is_Change[1] = 1;
				is_Change[2] = 1;
				is_Change[3] = 1;
				is_Change[4] = 1;
				is_Change[5] = 1;
				break;
			}
		}
	}
}
void timeInitEntry(void *pmt)
{
	startGetTime();
	rt_thread_delete(rt_thread_self());
}
void getCurrentTime(int isDelay)
{
	rt_thread_t timeInitThread = rt_thread_create("timeInit", timeInitEntry, RT_NULL, 2048, 4, 10);
	if (timeInitThread != RT_NULL)
	{
		is_GetTime_Flag = 0;
		rt_thread_startup(timeInitThread);
	}
	while (is_GetTime_Flag == 0 && isDelay == 1)
	{
		rt_thread_mdelay(20);
	}
}