#include "Sys_Tick.h"

//����ϵͳ�δ�ʱ��
uint32_t Sys_Tick_Config(uint32_t Ticks)
{
	return (SysTick_Config(Ticks));
}
