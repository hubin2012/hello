/*
*  按键扫描
*/

#include <stc12c5a60s2.h>
#include <intrins.h>

#include "Config.h"

keySTRUCT gKey; //全局变量，按键键值

//用来记录哪一个LED需要被点亮   
//LED1 LED2 LED3 LED5     LED 6 LED7 LED8 LED9     LED10 LED11 LED12 LED13    LED14 LED15 LED16 LED17
unsigned int Led = 0;  //0

//每位对应哪一个按键被按下
//sw12+sw13 sw9+sw10 sw14 sw13 sw12 sw11 sw10 sw9 sw8 sw7 sw6 sw5 sw4 sw3 sw2 sw1
unsigned int keyValue = 0; 
unsigned char keyPress_num[12] = {0};//记录对应按键按下次数
//LED灯
unsigned char ledArray[16] = {
								0xee, 0xdd, 0x7d, 0xed, 
								0xbd, 0x7e, 0xde, 0xbe,
								0xbb, 0x7b, 0xdb, 0xeb,
								0xb7, 0x77, 0xe7, 0xd7,
							 };

//函数申明
static void ledOn(unsigned char led);
static void ledScan(unsigned char led);


/*********************************************************************************************
函数名：KeyScan
作  用: 按键扫描，要定时调用
参  数：无
返回值：不稳地的键值，要进行滤波
**********************************************************************************************/
static unsigned char KeyScan(void)
{
	unsigned char i, tmp, readDate, key = 0;
	
	for(i = 0; i < 4; i++)
	{	
		if(i == 0)
		{	
			tmp = 0xfe;
		}
		else if(i == 1)
		{
			tmp = 0xfd;
		}
		else if(i == 2)
		{
			tmp = 0xfb;
		}
		else 
		{
			tmp = 0xf7;
		}
		
		P0 = tmp;

		_nop_();
		_nop_();
		_nop_();
		
		readDate = P0;

		if(readDate != tmp)
		{	
			UART_Send_String("");
			//有按键按下
			key = readDate;
		}
	}

	if(key > 0)
	{
		return key;
	}
	else
	{
		return 0;
	}
}

/*********************************************************************************************
函数名：keyProess
作  用: 按键去抖动，定时器中周期调用
参  数：无
返回值：无
**********************************************************************************************/
void keyProess(void)
{
	unsigned char key, pinValue;
	static unsigned char key_old = 0, tim = 0;

	pinValue = P0;  //读取
	
	key = KeyScan();
	//UART_Send_Char(key_old);
	
	if(key != KEY_NONE)
	{	
		//一直按着按键，计时
		if((key == key_old) && (gKey.keyOK == 0))
		{

			tim++;
		}
		else
		{
			//tim = 0;		
		}
	}
	else
	{
		//松手或者没有按键按下
		if(tim > 7)
		{
		  	tim = 0;

			gKey.keyOK = 1;
			gKey.keyValue = key_old;
		}
		else
		{
			tim = 0;
		}
	}
	
	key_old = key;

	P0 = pinValue;  //退出按键扫描，P0口恢复

}


/*********************************************************************************************
函数名：getKey
作  用: 获得稳定的按键值,并记录LED状态 Led
参  数：无
返回值：无
**********************************************************************************************/
void getKey(void)
{
	if(gKey.keyOK)
	{	
		switch(gKey.keyValue)
		{		
			//停止按键,所有LED关闭
			case KEY_SW13:
				Led = 0;
				keyValue = 0x1000;
				P2 |= 0xf0;
				break;

			//清洗/移动
			case KEY_SW11:
				if(keyPress_num[CLEAN])
				{
					//熄灭LED13
					Led &= 0xf7ff;   // 1111 0111 1111 1111
					keyPress_num[CLEAN] = 0; 

					keyValue &= 0xfbff;
				}
				else
				{	
					//点亮LED13
					Led &= 0xf1ff;
					Led |= 0x0800; //  0000 1000 0000 0000	

					keyValue |= 0x400;
					
					keyPress_num[CLEAN] = 1;
					keyPress_num[DRY] = 0;
					keyPress_num[WOMENUSE] = 0;
				}
				
				break;

			//强度  加
			case KEY_SW12:
				if(keyPress_num[STRESS] < 4)
				{
					keyPress_num[STRESS]++;
				}

				Led &= 0x0fff; //强度几个指示灯清0
				
				switch(keyPress_num[STRESS])
				{
					case 1:
						Led |= 0x1000;  //  0001 0000 0000 0000  LED14
						break;
					case 2:
						Led |= 0x2000;  //  0010 0000 0000 0000  LED15
						break;
					case 3:
						Led |= 0x4000;  //  0100 0000 0000 0000  LED16
						break;
					case 4:
						Led |= 0x8000;  //  1000 0000 0000 0000  LED17
						break;
				}
				break;

			case KEY_SW6:
				break;
				
			case KEY_SW8:

				break;

			//强度  减
			case KEY_SW9:
				if(keyPress_num[STRESS] > 0)
				{
					keyPress_num[STRESS]--;
				}
				
				Led &= 0x0fff; //强度几个指示灯清0
			
				switch(keyPress_num[STRESS])
				{
					case 0:
						//Led |= 0x0000;  //  0000 0000 0000 0000
						P2 |= 0xf0; //灯熄灭
						break;
					case 1:
						Led |= 0x1000;  //  0001 0000 0000 0000
						break;
					case 2:
						Led |= 0x2000;  //  0010 0000 0000 0000
						break;
					case 3:
						Led |= 0x4000;  //  0100 0000 0000 0000
						break;
				}
				break;

			//水温
			case KEY_SW4:
				if(keyPress_num[WATERTEMP]++ >= 3)
				{
					keyPress_num[WATERTEMP] = 0;
				}

				switch(keyPress_num[WATERTEMP])
				{
					case 1:
						//弱
						Led &= 0xfff7;  //LED5 = 0      1111 1111 1111 0111
						Led |= 0x02;    //LED2 = 1      0000 0000 0000 0010  
						break;

					case 2:
						//中
						Led &= 0xfffd;  //LED2 = 0      1111 1111 1111 1101
						Led |= 0x08;    //LED5 = 1      0000 0000 0000 1000
						break;

					case 3:
						// 强
						Led |= 0x0a;   //LED5 = LED2 = 1  0000 0000 0000 1010
						break;

					case 0:
						Led &= 0xfff5;  //LED5 = LED2 = 0  1111 1111 1111 0101
						P2 |= 0xf0;
						break;
				}	
				break;

			//座温
			case KEY_SW3:
				if(keyPress_num[SEATTEMP]++ >= 3)
				{
					keyPress_num[SEATTEMP] = 0;
				}

				switch(keyPress_num[SEATTEMP])
				{
					case 1:
						//弱
						Led &= 0xfffb;  //LED3 = 0      1111 1111 1111 1011
						Led |= 0x10;    //LED6 = 1      0000 0000 0001 0000  
						break;

					case 2:
						//中
						Led &= 0xffef;  //LED6 = 0      1111 1111 1110 1111
						Led |= 0x04;    //LED3 = 1      0000 0000 0000 0100
						break;

					case 3:
						// 强
						Led |= 0x14;   //LED6 = LED3 = 1  0000 0000 0001 0100
						break;

					case 0:
						Led &= 0xffeb;  //LED10 = LED9 = 0  1111 1111 1110 1011
						P2 |= 0xf0;
						break;
				}						
				break;

			// 烘干
			case KEY_SW7:
				if(keyPress_num[DRY])
				{
					//熄灭LED11
					Led &= 0xfdff;   // 1111 1101 1111 1111
					keyPress_num[DRY] = 0; 
					
				}
				else
				{	
					//点亮LED11
					Led &= 0xf1ff;
					Led |= 0x0200; //  0000 0010 0000 0000	
					
					keyPress_num[DRY] = 1;
					keyPress_num[CLEAN] = 0;
					keyPress_num[WOMENUSE] = 0;
				}
				break;

			// 女用
			case KEY_SW10:
				if(keyPress_num[WOMENUSE])
				{
					//熄灭LED12
					Led &= 0xfbff;   // 1111 1011 1111 1111
					keyPress_num[WOMENUSE] = 0; 
					
				}
				else
				{	
					//点亮LED12
					Led &= 0xf1ff;
					Led |= 0x0400; //  0000 0100 0000 0000	

					keyPress_num[WOMENUSE] = 1;
					keyPress_num[CLEAN] = 0;
					keyPress_num[DRY] = 0;
				}
				break;
		
			case KEY_SW14:

				break;

			case KEY_SW5:
				if(keyPress_num[ENERGYSAVE]++ >= 3)
				{
					keyPress_num[ENERGYSAVE] = 0;
				}

				switch(keyPress_num[ENERGYSAVE])
				{
					case 1:
						//冬
						Led &= 0xfeff;  //LED10 = 0     1111 1110 1111 1111
						Led |= 0x80;    //LED9 = 1      0000 0000 1000 0000  
						break;

					case 2:
						//春秋
						Led &= 0xff7f;  //LED9 = 0      1111 1111 0111 1111
						Led |= 0x100;    //LED10 = 1    0000 0001 0000 0000
						break;

					case 3:
						//夏
						Led |= 0x180;   //LED10 = LED9 = 1  0000 0001 1000 0000
						break;

					case 0:
						Led &= 0xfe7f;  //LED10 = LED9 = 0  1111 1110 0111 1111
						P2 |= 0xf0;
						break;
				}						
				break;
				
			case KEY_SW2:
			
				
				break;

			case KEY_SW9_SW10:

				break;
				
			case KEY_SW12_SW13:
				break;
				
		}
		UART_Send_Char(gKey.keyValue);
		
		gKey.keyOK = 0;
	}
}


/*********************************************************************************************
函数名：ledDisplay
作  用: led灯显示，需要在定时器中周期调用
参  数：无
返回值：无
**********************************************************************************************/
void ledDisplay(void)
{	
	unsigned char i;
	unsigned int tmp = 0;

	for(i = 0; i <= 15; i++)
	{
		tmp = (Led >> i) & 0x01;

		if(tmp)
		{
			ledScan(i);
		}
	}
	
}

/*********************************************************************************************
函数名：ledScan
作  用: 扫描点亮对应的灯
参  数：应该点亮的灯
返回值：无
**********************************************************************************************/
static void ledScan(unsigned char led)
{
	ledOn(ledArray[led]);
}


/*********************************************************************************************
函数名：ledOn
作  用: 点亮LED
参  数：LED灯 值
返回值：无
**********************************************************************************************/
static void ledOn(unsigned char led)
{
	unsigned char dat1 = 0, dat2 = 0;
	
	dat1 = P0; // 低四位
	dat2 = P2; // 高四位

	dat1 = (dat1 & 0xf0) | (led & 0x0f); //
	dat2 = (dat2 & 0x0f) | (led & 0xf0);  //
	
	P0 = dat1;
	P2 = dat2;
}

void led_d(void)
{
	static unsigned char ledStatus = 0;

	P0 |= 0x0f;
	P2 |= 0xf0;

	//UART_Send_Char(Led >> 8);
	//UART_Send_Char((unsigned char)Led);

	switch(ledStatus)
	{
		case 0:
			//LED1 LED8 LED9 LED7
			P0 &= 0xfe;

			if((Led & 0x01) == 0x01)
			{
				P2 &= 0xef;
			}

			if((Led & 0x40) == 0x40)
			{
				P2 &= 0xdf;
			}
			
			if((Led & 0x80) == 0x80)
			{
				P2 &= 0xbf;
			}
			
			if((Led & 0x20) == 0x20)
			{
				P2 &= 0x7f;
			}

			ledStatus = 1;
			
			break;

		case 1:
			//LED5 LED2 LED6 LED3
			P0 &= 0xfd;

			if((Led & 0x08) == 0x08)
			{
				P2 &= 0xef;
			}

			if((Led & 0x02) == 0x02)
			{
				P2 &= 0xdf;
			}
			
			if((Led & 0x10) == 0x10)
			{
				P2 &= 0xbf;
			}
			
			if((Led & 0x04) == 0x04)
			{
				P2 &= 0x7f;
			}

			ledStatus = 2;
			
			break;

		case 2:
			//LED13 LED12 LED10 LED11
			P0 &= 0xfb;

			if((Led & 0x800) == 0x800)
			{
				//UART_Send_Char(Led >> 8);
				//UART_Send_Char(Led);
				P2 &= 0xef;
			}

			if((Led & 0x400) == 0x400)
			{
				P2 &= 0xdf;
			}
			
			if((Led & 0x100) == 0x100)
			{
				P2 &= 0xbf;
			}
			
			if((Led & 0x200) == 0x200)
			{
				P2 &= 0x7f;
			}

			ledStatus = 3;
			
			break;

		case 3:
			//LED16 LED17 LED14 LED15
			P0 &= 0xf7;

			if((Led & 0x4000) == 0x4000)
			{
				P2 &= 0xef;
			}

			if((Led & 0x8000) == 0x8000)
			{
				P2 &= 0xdf;
			}
			
			if((Led & 0x1000) == 0x1000)
			{
				P2 &= 0xbf;
			}
			
			if((Led & 0x2000) == 0x2000)
			{
				P2 &= 0x7f;
			}

			ledStatus = 0;
			
			break;

	}
}


//test
extern void delayMS(unsigned int tim);
void testled(void)
{
	unsigned char i;
	
	for(i = 0; i < 16; i++)
	{
		ledOn(ledArray[i]); 
		delayMS(1000);
	}
}

