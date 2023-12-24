#include "GPIO.h"

//����NTCO��������Ϊ��������
void NTCO_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��ʱ��
  RCC_APB2PeriphClockCmd(NTCO_GPIO_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin  = NTCO_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	
	//��ʼ����GPIO
	GPIO_Init(NTCO_GPIO_PORT, &GPIO_InitStructure);
}
