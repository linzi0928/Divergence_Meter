/*************************************************************************************************************
舸轮综合船舶制造工作室        出品
Gearing Interated Shipbuilding Studio

八位世界线变动率测试仪
EL PSY CONGROO!!!

DS3231方案 11个74HC573锁存器控制八个辉光管的88个数字位 88个13001作驱动三极管 均采用最原始方案

关注贴吧ID：tnt34 获得最新工作成果
关注B站UP主：舸轮综合船舶制造 获得更多视频资源

2017-5-28第一版
版权所有 禁止用于任何商业用途！
注：本程序中ds3231相关驱动文件来自互联网，本工作室版权仅限于glow_tube_display.c & glow_tube_display.h
*************************************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "i2c.h"
#include "ds3231.h"
#include "usart1.h"
#include "glow_tube_display.h"
extern Time_Typedef TimeValue;
void glow_tube_drv(int b0,int b1,int b2,int b3,int b4,int b5,int b6,int b7);
void glow_tube_init(void);
void Divergence_DEMO(int DN);
void Show_time_D(int hour,int min,int sec);
void Show_time_N(int hour,int min,int sec);
int d=0;
void delay()
{	int i,j;
	for (i=500;i>0;i--)
		for (j=5000;j>0;j--);
}


int main()
{
	GPIO_Config();
	USART1_Config();
	glow_tube_init();
	TimeValue.year=0x2017;	
	TimeValue.month=0x05;
	TimeValue.week=0x01;
	TimeValue.date=0x29;
	TimeValue.hour=0x23;
	TimeValue.minute=0x19;
	TimeValue.second=0x0;
	//DS3231_Time_Init(&TimeValue);//第一次设置时间把此句打开
	while(1)
	{
	Time_Handle();
//	printf("%x年%x月%x日  %x:%x:%x  星期%x\n",TimeValue.year,//时钟测试，用串口发时间值
//									TimeValue.month,TimeValue.date,TimeValue.hour,
//TimeValue.minute,TimeValue.second,TimeValue.week);
	//delay();
	//glow_tube_drv(9,1,1,1,1,1,1,1);//辉光管短断路测试
		
	Show_time_N(TimeValue.hour,TimeValue.minute,TimeValue.second);//普通方式显示时间
	//Show_time_D(TimeValue.hour,TimeValue.minute,TimeValue.second);//世界线方式显示时间(好像还有些bug)
	}
}		  
