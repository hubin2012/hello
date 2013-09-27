/*
* 蜂鸣器
*/
#include <stc12c5a60s2.h>
#include "bsp_pwm.h"
#include "time.h"

//管脚定义
sbit P44 = P4^4; //低频控制端	   NA
sbit P45 = P4^5; //高频控制端   ALE

//变量定义
volatile unsigned char buzfrz_h, buzfrz_l; //定时器1 定时时间

code unsigned char FREQH[] = {
                        0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,    //低音1234567
                        0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC,// 
                        0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,            //高音 234567
                        0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF   //超高音 1234567 
                        };
code unsigned char FREQL[] = {
	                    0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,//低音1234567
	                    0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B, 0x8F, //
	                    0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,//高音 234567
	                    0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16   //超高音 1234567
	                    };

/*code unsigned char sszymmh[] = {
	6, 2, 3,  5, 2, 1,  3, 2, 2,  
	5, 2, 2,  1, 3, 2,  6, 2, 1,
	5, 2, 1,  6, 2, 4,  3, 2, 2,
	5, 2, 1,  6, 2, 1,  5, 2, 2,
	3, 2, 2,  1, 2, 1,  6, 1, 1,
	5, 2, 1,  3, 2, 1,  2, 2, 4,
	2, 2, 3,  3, 2, 1,  5, 2, 2,
	5, 2, 1,  6, 2, 1,  3, 2, 2,
	2, 2, 2,  1, 2, 4,  5, 2, 3,
	3, 2, 1,  2, 2, 1,  1, 2, 1,
	6, 1, 1,  1, 2, 1,  5, 1, 6, 
	0, 0, 0
	}; */
code unsigned char sszymmh[] = {
	1, 1, 3,  2, 1, 3,  3, 1, 3,  
	4, 1, 3,  5, 1, 3,  6, 1, 3,
	7, 1, 3,  1, 1, 3,  2, 1, 3,
	3, 1, 3,  4, 1, 3,  5, 1, 3,
	6, 1, 3,  7, 1, 3,  6, 1, 1,
	5, 1, 1,  3, 1, 1,  2, 1, 4,
	2, 2, 3,  3, 2, 1,  5, 2, 2,
	5, 2, 1,  6, 2, 1,  3, 2, 2,
	2, 2, 2,  1, 2, 4,  5, 2, 3,
	3, 2, 1,  2, 2, 1,  1, 2, 1,
	6, 1, 1,  1, 2, 1,  5, 1, 6, 
	0, 0, 0
	};
	

//函数声明
extern void delayMS(unsigned int tim);

/*
*  蜂鸣高频端控制，定时器中周期调用
*/
void BuzProsse(void)
{
	P45 = !P45;
	
	//TH1 = 0x1e;
	//TL1 = 0x1e;

	TH1 = buzfrz_h;
	TL1 = buzfrz_l;
}


/*
* 开蜂鸣器
*/
void BuzOn(void)
{
	TH1 = 0x1e;  
	TL1 = 0x1e;
	
	P44 = 1;  //开低频
	delayMS(400);
	P44 = 0;//关低频

	//开高频
	TR1 = 1;
	delayMS(400);
	TR0 = 0;

	TH1 = 0xfd;
	TL1 = 0xfd;
}

//
void Buz(void)
{
	P44 = 1;  //开低频
}


//延时程序，控制发音的时间长度 
void delay(unsigned char t)
{
	unsigned char t1;
	unsigned long t2;
	
	//双重循环, 共延时t个半拍 
	for(t1 = 0; t1 < t; t1++) 
	{
		//延时期间, 可进入T0中断去发音 
      	for(t2 = 0; t2 < 8000; t2++)
		{
		}
	}
}

void BuzOn_1(void)
{
	unsigned char i, time, tim0, tim1, k;

	i = 0;
	time = 1;

	while(time)
	{
		
		k = sszymmh[i] + 7*sszymmh[i+1] - 1;

		tim0 = FREQH[k];
		tim1 = FREQL[k];
		

		time = sszymmh[i+2];
		i += 3;

		//if(i > 9)
		{
		//	break;	
		}
		
		buzfrz_h = tim0;
		buzfrz_l = tim1;

		TR1 = 1; //开定时器1中断
		delay(time);
		TR1 = 0; //关定时器1中断
	}	
}