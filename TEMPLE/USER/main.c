#include "stm32f10x.h"
#include "define.h"
#include "USART.h"
#include "GPIO.h"
#include "Sys_Tick.h"
#include "BPC_DECODE.h"

//�ⲿ����
extern uint8_t Buff[BPC_EFFECT_NUM];

//��־λ
uint8_t  flag1=0;//֡��ʼ��־
uint8_t  flag2=0;//��Ч����������־

//��ʱ����
uint8_t j;

//����ʱ�仺����
uint8_t DATA[BPC_EFFECT_DATA];

int main(void)
{
	USART_Config();    //���õ����õĴ���
	printf("USART1 ONLINE\n");
	
	NTCO_GPIO_Config();//����NTCO��������
	printf("NTCO @ GPIO_A4\n");
	
	if(Sys_Tick_Config(Sys_Tick_Reload))//����Sys_Tick,10msһ���ж�
	{
		printf("Sys_Tick Init Failed, Check Sys_Tick_Reload Parameter And Reset\n");
		while(1);
	}
	printf("Sys_Tick Init Successfully\n");
	
	while(1)//�ȴ�Sys_Tick�ж�
	{
		if(flag1)     //��⵽��֡��ʼ��־λ
		{
			flag1=0;
			printf("Frame Head Detected\n");
		}
		else if(flag2)//��Ч���ݻ���������
		{
			flag2=0;
			for(j=0; j<BPC_EFFECT_NUM; j++)
			  printf("%d ", Buff[j]);
			printf("\n");
			
			j=BPC_DECODE(Buff, DATA);//����ԭʼ����
			if(j==1)     //��֡����Ч����
				printf("Contain Invaild Data\n");
			else if(j==2)//��Ч���ݵ�һ�׶�У�����
				printf("First Stage Verify Failed\n");
			else if(j==3)//��Ч���ݵڶ��׶�У�����
				printf("Second Stage Verify Failed\n");
			else if(j==4)//����ɹ�
			{
				printf("%d �� ",DATA[7]+2000);//��
				printf("%d ��", DATA[6]);     //��
				printf("%d ��", DATA[5]);     //��
				printf("%d ʱ", DATA[4] ? (DATA[1] + 12) : DATA[1]); //ʱ,24Сʱ��
				printf("%d ��", DATA[2]);     //��
				printf("%d ��", DATA[0]);     //��
				//printf("%d\n ", DATA[3]);         //����
				switch(DATA[3])
				{
					case 1:
						printf(" ����һ\n ");
					break;
					case 2:
						printf(" ���ڶ�\n ");
					break;
					case 3:
						printf(" ������\n ");
					break;
					case 4:
						printf(" ������\n ");
					break;
					case 5:
						printf(" ������\n ");
					break;
					case 6:
						printf(" ������\n ");
					break;
					case 7:
						printf(" ������\n ");
					break;
				}
			}
		}
	}
}
