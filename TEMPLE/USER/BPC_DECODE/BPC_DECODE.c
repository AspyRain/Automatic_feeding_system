#include "BPC_DECODE.h"

//解码原始的BPC数据
//参数uint8_t *Buff:有效数据缓冲区起始
//    uint8_t *DATA:解码时间缓冲区起始
//返回值:1 该帧有无效数据
//       2 有效数据第一阶段校验错误
//       3 有效数据第二阶段校验错误
//       4 解码成功
uint8_t BPC_DECODE(uint8_t *Buff, uint8_t *DATA)
{
	uint8_t i;
	uint8_t x=0;
	
	/*******************清空解码时间缓冲区*******************/
	for(i=0; i<BPC_EFFECT_DATA; i++)
	{
		*DATA=0;
		DATA++;
	}
	DATA-=BPC_EFFECT_DATA;//指向缓冲区起始
	/*******************清空解码时间缓冲区*******************/
	
	/*****************判断该帧是否有无效数据*****************/
	for(i=0; i<BPC_EFFECT_NUM; i++)//判断该帧是否有无效数据
	{
		if(*Buff==4)
			return 1;
		Buff++;
	}
	/*****************判断该帧是否有无效数据*****************/
	
	/********************校验第一阶段数据********************/
     Buff-=BPC_EFFECT_NUM;          //指针指向第一段有效数据起始处
	for(i=0; i<BPC_First_Stage; i++)
	{
		if(*Buff==1 || *Buff==2)//一个"1"
			x+=1;
		else if(*Buff==3)       //两个"1"
			x+=2;
		Buff++;
	}
	if(x%2==0)//偶
	{
		if(*Buff==1 || *Buff==3)
			return 2;//第一段有效数据校验失败
	}
	else      //奇
	{
		if(*Buff==0 || *Buff==2)
			return 2;//第一段有效数据校验失败
	}
	x=0;
	/********************校验第一阶段数据********************/
	
	/********************校验第二阶段数据********************/
	
	Buff+=1;                       //指针指向第二段有效数据起始处
	for(i=0; i<BPC_Second_Stage; i++)
	{
		if(*Buff==1 || *Buff==2)//一个"1"
			x+=1;
		else if(*Buff==3)       //两个"1"
			x+=2;
		Buff++;
	}
	if(x%2==0)//偶
	{
		if(*Buff==1 || *Buff==3)
			return 3;//第二段有效数据校验失败
	}
	else      //奇
	{
		if(*Buff==0 || *Buff==2)
			return 3;//第二段有效数据校验失败
	}
	
	/********************校验第二阶段数据********************/
	
	/************************解码数据************************/
	//秒
	Buff-=BPC_EFFECT_NUM-1;        //指针指向"秒"数据起始处,开始解码数据
	if(*Buff==0)                   //第1秒
		*DATA=1;
	else if(*Buff==1)              //第21秒
		*DATA=21;
	else if(*Buff==2)              //第41秒
		*DATA=41;
	DATA++;
	
	//时
	Buff+=2;                       //指针指向"时"数据起始处
	*DATA+=(*Buff)*4;
	Buff++;
	*DATA+=*Buff;
	DATA++;
	
	//分
	Buff++;                        //指针指向"分"数据起始处
	*DATA+=(*Buff)*16;
	Buff++;
	*DATA+=(*Buff)*4;
	Buff++;
	*DATA+=*Buff;
	DATA++;
	
	//星期
	Buff++;                        //指针指向"星期"数据起始处
	*DATA+=(*Buff)*4;
	Buff++;
	*DATA+=*Buff;
	DATA++;
	
	//午别
	Buff++;                       //指针指向"午别"数据起始处
	if(*Buff==0 || *Buff==1)      //上午
		*DATA=0;
	else                          //下午
		*DATA=1;
	DATA++;
	
	//日期
	Buff++;                       //指针指向"日期"数据起始处
	*DATA+=(*Buff)*16;
	Buff++;
	*DATA+=(*Buff)*4;
	Buff++;
	*DATA+=*Buff;
	DATA++;
	
	//月份
	Buff++;                        //指针指向"月份"数据起始处
	*DATA+=(*Buff)*4;
	Buff++;
	*DATA+=*Buff;
	DATA++;
	
	//年份
	Buff++;                        //指针指向"年份"数据起始处
	*DATA+=(*Buff)*16;
	Buff++;
	*DATA+=(*Buff)*4;
	Buff++;
	*DATA+=*Buff;
	Buff++;
	if(*Buff==2 || *Buff==3)
		*DATA+=64;
	return 4;
	/************************解码数据************************/
}
