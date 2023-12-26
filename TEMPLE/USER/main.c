#include "stm32f10x.h"
#include "define.h"
#include "USART.h"
#include "GPIO.h"
#include "Sys_Tick.h"
#include "BPC_DECODE.h"

//外部变量
extern uint8_t Buff[BPC_EFFECT_NUM];

//标志位
uint8_t  flag1=0;//帧起始标志
uint8_t  flag2=0;//有效数据已满标志

//临时变量
uint8_t j;

//解码时间缓冲区
uint8_t DATA[BPC_EFFECT_DATA];

int main(void)
{
	USART_Config();    //配置调试用的串口
	printf("USART1 ONLINE\n");
	
	NTCO_GPIO_Config();//配置NTCO输入引脚
	printf("NTCO @ GPIO_A4\n");
	
	if(Sys_Tick_Config(Sys_Tick_Reload))//配置Sys_Tick,10ms一次中断
	{
		printf("Sys_Tick Init Failed, Check Sys_Tick_Reload Parameter And Reset\n");
		while(1);
	}
	printf("Sys_Tick Init Successfully\n");
	
	while(1)//等待Sys_Tick中断
	{
		if(flag1)     //检测到了帧起始标志位
		{
			flag1=0;
			printf("Frame Head Detected\n");
		}
		else if(flag2)//有效数据缓冲区已满
		{
			flag2=0;
			for(j=0; j<BPC_EFFECT_NUM; j++)
			  printf("%d ", Buff[j]);
			printf("\n");
			
			j=BPC_DECODE(Buff, DATA);//解码原始数据
			if(j==1)     //该帧有无效数据
				printf("Contain Invaild Data\n");
			else if(j==2)//有效数据第一阶段校验错误
				printf("First Stage Verify Failed\n");
			else if(j==3)//有效数据第二阶段校验错误
				printf("Second Stage Verify Failed\n");
			else if(j==4)//解码成功
			{
				printf("%d 年 ",DATA[7]+2000);//年
				printf("%d 月", DATA[6]);     //月
				printf("%d 日", DATA[5]);     //日
				printf("%d 时", DATA[4] ? (DATA[1] + 12) : DATA[1]); //时,24小时制
				printf("%d 分", DATA[2]);     //分
				printf("%d 秒", DATA[0]);     //秒
				//printf("%d\n ", DATA[3]);         //星期
				switch(DATA[3])
				{
					case 1:
						printf(" 星期一\n ");
					break;
					case 2:
						printf(" 星期二\n ");
					break;
					case 3:
						printf(" 星期三\n ");
					break;
					case 4:
						printf(" 星期四\n ");
					break;
					case 5:
						printf(" 星期五\n ");
					break;
					case 6:
						printf(" 星期六\n ");
					break;
					case 7:
						printf(" 星期日\n ");
					break;
				}
			}
		}
	}
}
