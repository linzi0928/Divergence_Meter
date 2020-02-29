#include "delay.h"

void delay_us(u16 time)
{
	u16 i=0;
	while(time--)
		{
		i=10;
		while(i--);
		}
}
