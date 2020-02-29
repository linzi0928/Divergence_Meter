/*************************************************************************************************************
�����ۺϴ������칤����        ��Ʒ
Gearing Interated Shipbuilding Studio

��λ�����߱䶯�ʲ�����
EL PSY CONGROO!!!

DS3231���� 11��74HC573���������ư˸��Թ�ܵ�88������λ 88��13001������������ ��������ԭʼ����

��ע����ID��tnt34 ������¹����ɹ�
��עBվUP���������ۺϴ������� ��ø�����Ƶ��Դ

2017-5-28��һ��
��Ȩ���� ��ֹ�����κ���ҵ��;��
ע����������ds3231��������ļ����Ի��������������Ұ�Ȩ������glow_tube_display.c & glow_tube_display.h
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
	//DS3231_Time_Init(&TimeValue);//��һ������ʱ��Ѵ˾��
	while(1)
	{
	Time_Handle();
//	printf("%x��%x��%x��  %x:%x:%x  ����%x\n",TimeValue.year,//ʱ�Ӳ��ԣ��ô��ڷ�ʱ��ֵ
//									TimeValue.month,TimeValue.date,TimeValue.hour,
//TimeValue.minute,TimeValue.second,TimeValue.week);
	//delay();
	//glow_tube_drv(9,1,1,1,1,1,1,1);//�Թ�̶ܶ�·����
		
	Show_time_N(TimeValue.hour,TimeValue.minute,TimeValue.second);//��ͨ��ʽ��ʾʱ��
	//Show_time_D(TimeValue.hour,TimeValue.minute,TimeValue.second);//�����߷�ʽ��ʾʱ��(������Щbug)
	}
}		  
