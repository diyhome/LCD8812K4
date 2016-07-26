#include <reg52.h>
#include "LCD8812K4.h"

#define uchar unsigned char

void main()
{
	uchar k,t;
	Ht1621_Init();
	LCD_init();
	delayms(800);
	Ht1621WrAllData(0,Ht1621Tab,16);
	while (1)
	{
		LCD_disp_string(0,0,"String");
		LCD_disp_string(0,1,"Love !");
		for (k=0;k<10;k++)
		{
			for (t=0;t<10;t++)
			{
				bignum(k,t);
				delayms(500);
				Ht1621WrAllData(0,Ht1621Tab,16);
			}
		}
	}
}
