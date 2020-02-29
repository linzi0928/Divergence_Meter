/*************************************************************************************************************
�����ۺϴ������칤����        ��Ʒ
Gearing Interated Shipbuilding Studio

��λ�����߱䶯�ʲ����ǻԹ������
EL PSY CONGROO!!!

���ļ��ǻԹ����������11��74HC573�ֱ���ƹ��ӵ�11�����֣�ÿ��573��8λ��Ӧ�˸��Թ��(�����з�ʽ�볣��������ͬ��ע�⣡)

��ע����ID��tnt34 ������¹����ɹ�
��עBվUP���������ۺϴ������� ��ø�����Ƶ��Դ

2017-5-28��һ��
��Ȩ���� ��ֹ�����κ���ҵ��;��
*************************************************************************************************************/
#include "glow_tube_display.h"
uint16_t IOStat0,IOStat1,IOStat2,IOStat3,IOStat4,IOStat5,IOStat6,IOStat7,IOStat8,IOStat9,IOStat10;//�˱������ڼ�¼IO״̬��Ŀ��Ϊ��ֹ������һλʱ����һλϴ��
int td=0;//��ǰ������
const int Divergence_MEM[]={571024,571015,523307,523299,456923,456914,456903,409431,409420,337187,334581,337337,337199,328403,000000,//0-14ΪAlpha��0.
														1130426,1130238,1130205,1048596,//15-18Ϊbeta
														1048599,1049326,1048728,//19-21λX
														2615074,//22λGamma
														3019430,3030493,3182879,3130238,3372329,3386019,3406288,3600104,3667293,//23-31λDelta
														4456441,4456442,4530806,4389117,//32-35λEpsilon
														275349,195284};//36-37λOmega
void glow_tube_init()//�Թ�����ӵ�IO�ڳ�ʼ������PA0-7��573�������İ˸��������룬�����Ӧ11��573��ѡ�ź�
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ����Ϻ�ر�ȫ��������ų���ȷ����ƽ����
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//�����о�Ϊ11����ѡ�˿ڣ��Ƚ����Ǵ�
	GPIO_SetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_SetBits(GPIOC,GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);//�򿪶�ѡ������λ�ر�
	Soft_delay_ms(10);//�����������ݵķ�Ӧʱ��
	GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//�رն�ѡ�������ر�״̬
	GPIO_ResetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
}
void glow_tube_clr()//�����ʾ�����ʼ����������һ������
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
void glow_tube_drv(int b0,int b1,int b2,int b3,int b4,int b5,int b6,int b7)//8λ�Թ��������������ʾ8λ����0-9��С������10��88��case =.=
{
glow_tube_clr();//�����������ص�
IOStat0=0;IOStat1=0;IOStat2=0;IOStat3=0;IOStat4=0;IOStat5=0;IOStat6=0;IOStat7=0;IOStat8=0;IOStat9=0;IOStat10=0;
	switch(b0)//b0��Ӧ��һλ
		{
			case 0:
			IOStat0=IOStat0|GPIO_Pin_0;//����ǰ��IO״̬�뵱ǰ״̬�ϲ�
			GPIO_SetBits(GPIOB,GPIO_Pin_8);//PB8��Ӧ0��573��ѡ�˿�
			GPIO_SetBits(GPIOA,IOStat0);//��PA0-7����ϲ����IO״̬
			Soft_delay_ms(1);//������ʱ����������Ӧʱ��
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);//��0�Ķ�ѡ�ر�
			GPIO_ResetBits(GPIOA,0xff);//��PA0-7ȫ���رգ����ֻ�رոմ򿪵Ķ˿ڻ�����Ӱ��������Ϊɶ��Ҳ���������=.=
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
			IOStat6=IOStat6|GPIO_Pin_7;//���ֶ˿ں���������һ���ļ��Ƿ��ߵ�ʱ��˳����ˣ���Ҫ�ӳ�����������漸λ��Ϊ����
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

void Divergence_change(int DN)//��Ӧ�����ߵ���ʾ�������������0-37
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

void Divergence_DEMO(int DN)//��ʾ�����߱仯�Ķ�������
{
	int g;

			for(g=0;g<38;g++)
			{
			Divergence_change(g);
			Soft_delay_ms(700);
			glow_tube_drv(g/4,10,g/5,g/6,g/5,g/7,g/6,g/4);//��������ֻ��Ϊ�˿����������
			Soft_delay_ms(700);
			}
	Divergence_change(DN);
}
void Show_time_D(int hour,int min,int sec)//�������߸�ʽ��ʾʱ��
{
	hour=10*(hour/16)+(hour%16);//��BCD��ת����ʮ����
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
void Show_time_N(int hour,int min,int sec)//����ͨ��ʽ��ʾʱ��
{
	hour=10*(hour/16)+(hour%16);//��BCD��ת����ʮ����
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

void Soft_delay_ms(u16 time)//�򵥴ֱ��������ʱ����
{    
   u16 i=0;  
   while(time--)
   {
      i=150; 
      while(i--) ;    
   }
}
