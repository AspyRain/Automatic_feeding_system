#include "Sys_Tick.h"

//配置系统滴答定时器
uint32_t Sys_Tick_Config(uint32_t Ticks)
{
	return (SysTick_Config(Ticks));
}
