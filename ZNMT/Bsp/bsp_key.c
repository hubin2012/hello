/*
*  ����ɨ��
*/

#include <stc12c5a60s2.h>
#include <intrins.h>

#include "Config.h"

keySTRUCT gKey; //ȫ�ֱ�����������ֵ

//������¼��һ��LED��Ҫ������   
//LED1 LED2 LED3 LED5     LED 6 LED7 LED8 LED9     LED10 LED11 LED12 LED13    LED14 LED15 LED16 LED17
unsigned int Led = 0;  //0

//ÿλ��Ӧ��һ������������
//sw12+sw13 sw9+sw10 sw14 sw13 sw12 sw11 sw10 sw9 sw8 sw7 sw6 sw5 sw4 sw3 sw2 sw1
unsigned int keyValue = 0; 
unsigned char keyPress_num[12] = {0};//��¼��Ӧ�������´���
//LED��
unsigned char ledArray[16] = {
								0xee, 0xdd, 0x7d, 0xed, 
								0xbd, 0x7e, 0xde, 0xbe,
								0xbb, 0x7b, 0xdb, 0xeb,
								0xb7, 0x77, 0xe7, 0xd7,
							 };

//��������
static void ledOn(unsigned char led);
static void ledScan(unsigned char led);


/*********************************************************************************************
��������KeyScan
��  ��: ����ɨ�裬Ҫ��ʱ����
��  ������
����ֵ�����ȵصļ�ֵ��Ҫ�����˲�
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
			//�а�������
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
��������keyProess
��  ��: ����ȥ��������ʱ�������ڵ���
��  ������
����ֵ����
**********************************************************************************************/
void keyProess(void)
{
	unsigned char key, pinValue;
	static unsigned char key_old = 0, tim = 0;

	pinValue = P0;  //��ȡ
	
	key = KeyScan();
	//UART_Send_Char(key_old);
	
	if(key != KEY_NONE)
	{	
		//һֱ���Ű�������ʱ
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
		//���ֻ���û�а�������
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

	P0 = pinValue;  //�˳�����ɨ�裬P0�ڻָ�

}


/*********************************************************************************************
��������getKey
��  ��: ����ȶ��İ���ֵ,����¼LED״̬ Led
��  ������
����ֵ����
**********************************************************************************************/
void getKey(void)
{
	if(gKey.keyOK)
	{	
		switch(gKey.keyValue)
		{		
			//ֹͣ����,����LED�ر�
			case KEY_SW13:
				Led = 0;
				keyValue = 0x1000;
				P2 |= 0xf0;
				break;

			//��ϴ/�ƶ�
			case KEY_SW11:
				if(keyPress_num[CLEAN])
				{
					//Ϩ��LED13
					Led &= 0xf7ff;   // 1111 0111 1111 1111
					keyPress_num[CLEAN] = 0; 

					keyValue &= 0xfbff;
				}
				else
				{	
					//����LED13
					Led &= 0xf1ff;
					Led |= 0x0800; //  0000 1000 0000 0000	

					keyValue |= 0x400;
					
					keyPress_num[CLEAN] = 1;
					keyPress_num[DRY] = 0;
					keyPress_num[WOMENUSE] = 0;
				}
				
				break;

			//ǿ��  ��
			case KEY_SW12:
				if(keyPress_num[STRESS] < 4)
				{
					keyPress_num[STRESS]++;
				}

				Led &= 0x0fff; //ǿ�ȼ���ָʾ����0
				
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

			//ǿ��  ��
			case KEY_SW9:
				if(keyPress_num[STRESS] > 0)
				{
					keyPress_num[STRESS]--;
				}
				
				Led &= 0x0fff; //ǿ�ȼ���ָʾ����0
			
				switch(keyPress_num[STRESS])
				{
					case 0:
						//Led |= 0x0000;  //  0000 0000 0000 0000
						P2 |= 0xf0; //��Ϩ��
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

			//ˮ��
			case KEY_SW4:
				if(keyPress_num[WATERTEMP]++ >= 3)
				{
					keyPress_num[WATERTEMP] = 0;
				}

				switch(keyPress_num[WATERTEMP])
				{
					case 1:
						//��
						Led &= 0xfff7;  //LED5 = 0      1111 1111 1111 0111
						Led |= 0x02;    //LED2 = 1      0000 0000 0000 0010  
						break;

					case 2:
						//��
						Led &= 0xfffd;  //LED2 = 0      1111 1111 1111 1101
						Led |= 0x08;    //LED5 = 1      0000 0000 0000 1000
						break;

					case 3:
						// ǿ
						Led |= 0x0a;   //LED5 = LED2 = 1  0000 0000 0000 1010
						break;

					case 0:
						Led &= 0xfff5;  //LED5 = LED2 = 0  1111 1111 1111 0101
						P2 |= 0xf0;
						break;
				}	
				break;

			//����
			case KEY_SW3:
				if(keyPress_num[SEATTEMP]++ >= 3)
				{
					keyPress_num[SEATTEMP] = 0;
				}

				switch(keyPress_num[SEATTEMP])
				{
					case 1:
						//��
						Led &= 0xfffb;  //LED3 = 0      1111 1111 1111 1011
						Led |= 0x10;    //LED6 = 1      0000 0000 0001 0000  
						break;

					case 2:
						//��
						Led &= 0xffef;  //LED6 = 0      1111 1111 1110 1111
						Led |= 0x04;    //LED3 = 1      0000 0000 0000 0100
						break;

					case 3:
						// ǿ
						Led |= 0x14;   //LED6 = LED3 = 1  0000 0000 0001 0100
						break;

					case 0:
						Led &= 0xffeb;  //LED10 = LED9 = 0  1111 1111 1110 1011
						P2 |= 0xf0;
						break;
				}						
				break;

			// ���
			case KEY_SW7:
				if(keyPress_num[DRY])
				{
					//Ϩ��LED11
					Led &= 0xfdff;   // 1111 1101 1111 1111
					keyPress_num[DRY] = 0; 
					
				}
				else
				{	
					//����LED11
					Led &= 0xf1ff;
					Led |= 0x0200; //  0000 0010 0000 0000	
					
					keyPress_num[DRY] = 1;
					keyPress_num[CLEAN] = 0;
					keyPress_num[WOMENUSE] = 0;
				}
				break;

			// Ů��
			case KEY_SW10:
				if(keyPress_num[WOMENUSE])
				{
					//Ϩ��LED12
					Led &= 0xfbff;   // 1111 1011 1111 1111
					keyPress_num[WOMENUSE] = 0; 
					
				}
				else
				{	
					//����LED12
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
						//��
						Led &= 0xfeff;  //LED10 = 0     1111 1110 1111 1111
						Led |= 0x80;    //LED9 = 1      0000 0000 1000 0000  
						break;

					case 2:
						//����
						Led &= 0xff7f;  //LED9 = 0      1111 1111 0111 1111
						Led |= 0x100;    //LED10 = 1    0000 0001 0000 0000
						break;

					case 3:
						//��
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
��������ledDisplay
��  ��: led����ʾ����Ҫ�ڶ�ʱ�������ڵ���
��  ������
����ֵ����
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
��������ledScan
��  ��: ɨ�������Ӧ�ĵ�
��  ����Ӧ�õ����ĵ�
����ֵ����
**********************************************************************************************/
static void ledScan(unsigned char led)
{
	ledOn(ledArray[led]);
}


/*********************************************************************************************
��������ledOn
��  ��: ����LED
��  ����LED�� ֵ
����ֵ����
**********************************************************************************************/
static void ledOn(unsigned char led)
{
	unsigned char dat1 = 0, dat2 = 0;
	
	dat1 = P0; // ����λ
	dat2 = P2; // ����λ

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

