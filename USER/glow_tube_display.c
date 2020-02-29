/*************************************************************************************************************
舸轮综合船舶制造工作室        出品
Gearing Interated Shipbuilding Studio

八位世界线变动率测试仪辉光管驱动
EL PSY CONGROO!!!

本文件是辉光管驱动程序，11个74HC573分别控制管子的11个数字，每个573的8位对应八个辉光管(此排列方式与常规有所不同需注意！)

关注贴吧ID：tnt34 获得最新工作成果
关注B站UP主：舸轮综合船舶制造 获得更多视频资源

2017-5-28第一版
版权所有 禁止用于任何商业用途！
*************************************************************************************************************/
#include "glow_tube_display.h"
uint16_t IOStat0,IOStat1,IOStat2,IOStat3,IOStat4,IOStat5,IOStat6,IOStat7,IOStat8,IOStat9,IOStat10;//此变量用于记录IO状态，目的为防止更新下一位时将上一位洗掉
int td=0;//当前世界线
const int Divergence_MEM[]={571024,571015,523307,523299,456923,456914,456903,409431,409420,337187,334581,337337,337199,328403,000000,//0-14为Alpha补0.
														1130426,1130238,1130205,1048596,//15-18为beta
														1048599,1049326,1048728,//19-21位X
														2615074,//22位Gamma
														3019430,3030493,3182879,3130238,3372329,3386019,3406288,3600104,3667293,//23-31位Delta
														4456441,4456442,4530806,4389117,//32-35位Epsilon
														275349,195284};//36-37位Omega
void glow_tube_init()//辉光管连接的IO口初始化程序，PA0-7接573锁存器的八个数据输入，其余对应11个573段选信号
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化完毕后关闭全部输出，排除不确定电平波动
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//这三行均为11个段选端口，先将他们打开
	GPIO_SetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_SetBits(GPIOC,GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);//打开段选后将所有位关闭
	Soft_delay_ms(10);//给锁存器短暂的反应时间
	GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//关闭段选，锁定关闭状态
	GPIO_ResetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
}
void glow_tube_clr()//清除显示，与初始化程序的最后一段相似
{
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
	GPIO_SetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_SetBits(GPIOC,GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	Soft_delay_ms(1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
}
void glow_tube_drv(int b0,int b1,int b2,int b3,int b4,int b5,int b6,int b7)//8位辉光管驱动函数，显示8位数字0-9，小数点用10，88个case =.=
{
glow_tube_clr();//先清屏避免重叠
IOStat0=0;IOStat1=0;IOStat2=0;IOStat3=0;IOStat4=0;IOStat5=0;IOStat6=0;IOStat7=0;IOStat8=0;IOStat9=0;IOStat10=0;
	switch(b0)//b0对应第一位
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_0;//将先前的IO状态与当前状态合并
			GPIO_SetBits(GPIOB,GPIO_Pin_8);//PB8对应0的573段选端口
			GPIO_SetBits(GPIOA,IOStat0);//将PA0-7输出合并后的IO状态
			Soft_delay_ms(1);//短暂延时给锁存器反应时间
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);//将0的段选关闭
			GPIO_ResetBits(GPIOA,0xff);//将PA0-7全部关闭，如果只关闭刚打开的端口会有重影，至于是为啥我也不想分析了=.=
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_7;//这种端口号与其它不一样的即是飞线的时候顺序错了，需要从程序更正，后面几位尤为明显
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_0;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_1;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_0;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 11:
			IOStat10=IOStat10|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
		switch(b1)
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOA,IOStat0);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_4;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_7;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_1;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
				switch(b2)
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOA,IOStat0);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_6;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_6;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_2;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
		switch(b3)
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOA,IOStat0);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_3;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_3;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_3;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
				switch(b4)
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOA,IOStat0);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_3;
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_1;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_5;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_4;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
		switch(b5)
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOA,IOStat0);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_4;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_2;
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_5;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_4;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_5;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
				switch(b6)
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOA,IOStat0);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_5;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_1;
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_7;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_2;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_6;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_6;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
		switch(b7)
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_7;
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOA,IOStat0);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 1:
			IOStat1=IOStat1|GPIO_Pin_7;
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,IOStat1);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 2:
			IOStat2=IOStat2|GPIO_Pin_7;
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			GPIO_SetBits(GPIOA,IOStat2);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 3:
			IOStat3=IOStat3|GPIO_Pin_7;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,IOStat3);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 4:
			IOStat4=IOStat4|GPIO_Pin_7;
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat4);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 5:
			IOStat5=IOStat5|GPIO_Pin_7;
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat5);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 6:
			IOStat6=IOStat6|GPIO_Pin_0;
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_SetBits(GPIOA,IOStat6);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 7:
			IOStat7=IOStat7|GPIO_Pin_2;
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			GPIO_SetBits(GPIOA,IOStat7);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 8:
			IOStat8=IOStat8|GPIO_Pin_0;
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,IOStat8);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 9:
			IOStat9=IOStat9|GPIO_Pin_7;
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,IOStat9);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,0xff);
			break;
			case 10:
			IOStat10=IOStat10|GPIO_Pin_7;
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOA,IOStat10);
			Soft_delay_ms(1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_ResetBits(GPIOA,0xff);
			break;
		}
}

void Divergence_change(int DN)//对应世界线的显示函数，输入参数0-37
{
	int Bi0,Bi1,Bi2,Bi3,Bi4,Bi5,Bi6;
	if(DN<=14)
	{
	Bi0=0;
	Bi1=Divergence_MEM[DN]/100000;
	Bi2=Divergence_MEM[DN]%100000/10000;
	Bi3=Divergence_MEM[DN]%10000/1000;
	Bi4=Divergence_MEM[DN]%1000/100;
	Bi5=Divergence_MEM[DN]%100/10;
	Bi6=Divergence_MEM[DN]%10;
	}
	else if(DN<=35)
	{
	Bi0=Divergence_MEM[DN]/1000000;
	Bi1=Divergence_MEM[DN]%1000000/100000;
	Bi2=Divergence_MEM[DN]%100000/10000;
	Bi3=Divergence_MEM[DN]%10000/1000;
	Bi4=Divergence_MEM[DN]%1000/100;
	Bi5=Divergence_MEM[DN]%100/10;
	Bi6=Divergence_MEM[DN]%10;
	}
	else if(DN>35)
	{
	Bi0=10;
	Bi1=Divergence_MEM[DN]/100000;
	Bi2=Divergence_MEM[DN]%100000/10000;
	Bi3=Divergence_MEM[DN]%10000/1000;
	Bi4=Divergence_MEM[DN]%1000/100;
	Bi5=Divergence_MEM[DN]%100/10;
	Bi6=Divergence_MEM[DN]%10;
	}
	glow_tube_drv(Bi0,10,Bi1,Bi2,Bi3,Bi4,Bi5,Bi6);
}

void Divergence_DEMO(int DN)//演示世界线变化的动画函数
{
	int g;

			for(g=0;g<38;g++)
			{
			Divergence_change(g);
			Soft_delay_ms(700);
			glow_tube_drv(g/4,10,g/5,g/6,g/5,g/7,g/6,g/4);//随便除个数只是为了看起来更随机
			Soft_delay_ms(700);
			}
	Divergence_change(DN);
}
void Show_time_D(int hour,int min,int sec)//以世界线格式显示时间
{
	hour=10*(hour/16)+(hour%16);//将BCD码转换成十进制
	min=10*(min/16)+(min%16);
	sec=10*(sec/16)+(sec%16);
	if (hour>12)
	glow_tube_drv(1,10,(hour-12)/10,(hour-12)%10,min/10,min%10,sec/10,sec%10);
	else
	glow_tube_drv(0,10,hour/10,hour%10,min/10,min%10,sec/10,sec%10);
	if(sec==59)
		{
			Divergence_DEMO(td);
			td++;
			if(td==38)
				td=0;
		}
}
void Show_time_N(int hour,int min,int sec)//以普通格式显示时间
{
	hour=10*(hour/16)+(hour%16);//将BCD码转换成十进制
	min=10*(min/16)+(min%16);
	sec=10*(sec/16)+(sec%16);
	glow_tube_drv(hour/10,hour%10,10,min/10,min%10,10,sec/10,sec%10);
	if(sec==59)
		{
			Divergence_DEMO(td);
			td++;
			if(td==38)
				td=0;
		}
}

void Soft_delay_ms(u16 time)//简单粗暴的软件延时函数
{    
   u16 i=0;  
   while(time--)
   {
      i=150; 
      while(i--) ;    
   }
}
