#include "GPIO.h"

//设置NTCO输入引脚为浮空输入
void NTCO_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开时钟
  RCC_APB2PeriphClockCmd(NTCO_GPIO_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin  = NTCO_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	
	//初始化该GPIO
	GPIO_Init(NTCO_GPIO_PORT, &GPIO_InitStructure);
}
