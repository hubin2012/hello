/*
*   uln2003/74hc595 ����MOTO
*/

#include <stc12c5a60s2.h>
#include <intrins.h>
#include "bsp_moto.h"

//���ƹܽŶ���
sbit HC595_SI = P4^6; //��������
sbit HC595_RCK = P2^0;//����
sbit HC595_SCK = P2^1;//ʱ��

//ȫ�ֱ�������
motorStruct gmotor;


//���ת����ֵ
unsigned char motoBack_Move[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};  //����
unsigned char motoForward_Move[8] = {0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01};
//unsigned char motoBack_Move[8] = {0x03,0x06, 0x0c, 0x09};  //˫������
//unsigned char motoForward_Move[8] = {0x09, 0x0c, 0x06, 0x03};

//unsigned char motoBack_Move[8] = {0x01,0x02, 0x04, 0x08};  //����
//unsigned char motoForward_Move[8] = {0x08, 0x04, 0x02, 0x01};


unsigned char motoForward_YOUR[8] = {0x10, 0x30, 0x20, 0x60, 0x40, 0xc0, 0x80, 0x90};
unsigned char motoBack_YOUR[8] = {0x90, 0x80, 0xc0, 0x40, 0x60, 0x20, 0x30, 0x10};


extern void delayMS(unsigned int tim);
/*********************************************************************************************
��������bsp_sendDate_595
��  ��: ���з������ݣ���λ����
��  ����Ҫ���͵�����
����ֵ����
**********************************************************************************************/
void bsp_sendDate_595(unsigned char date)
{
	unsigned char i;

	for(i = 8; i > 0; i--)
	{
		if((date >> (i - 1)) & 0x01)
		{
			HC595_SI = 1;
		}
		else
		{
			HC595_SI = 0;
		}

		//��������
		HC595_SCK = 0;
		delayMS(2);
		
		
		HC595_SCK = 1;
	}
}


/*********************************************************************************************
��������bsp_lockDate_595
��  ��: �������������
��  ������
����ֵ����
**********************************************************************************************/
void bsp_lockDate_595(void)
{
	HC595_RCK = 0;
	delayMS(2);
	HC595_RCK = 1;
	delayMS(2);
	HC595_RCK = 0;
}


/*********************************************************************************************
��������motoRun
��  ��: �������������
��  ����motor --- ��Ӧ��ͬ�ĵ��   dir -----��Ӧ����˶�����   step ------����˶��ľ���
����ֵ����
**********************************************************************************************/
void motorRun(motorEnum motor, STATUS dir, unsigned int step)
{
	unsigned char i;

	//��ˮ��
	if(motor == move_motor)
	{
		while(step--)
		{
			if(dir == SET)
			{
				for(i = 0; i < 8; i++)
				{
					HC595_RCK = 0;
					bsp_sendDate_595(motoForward_Move[i]);
					HC595_RCK = 1;

					//UART_Send_Char(i);
					//UART_Send_Char(motoForward_Move[i]);

					//delayMS(400);
				}
			}
			else
			{
				for(i = 0; i < 8; i++)
				{
					HC595_RCK = 0;
					bsp_sendDate_595(motoBack_Move[i]);
					HC595_RCK = 1;
				}
			}
		}
	}

	//��ˮ���ƶ����
	if(motor == your_motor)
	{
		while(step--)
		{
			for(i = 0; i < 8; i++)
			{
				HC595_RCK = 0;
				if(dir == SET)
				{
					bsp_sendDate_595(motoForward_YOUR[i]);
					//delayMS(5);
				}
				else
				{
					bsp_sendDate_595(motoBack_YOUR[i]);
					//delayMS(5);
				}
				HC595_RCK = 1;
			}
		}
	}
}

//���
void motor(void)
{
	if(gmotor.moveMotor_f == SET)
	{
		motorRun(move_motor, SET, 400);
		gmotor.moveMotor_f = RESET;
	}

	if(gmotor.moveMotor_b == SET)
	{
		motorRun(move_motor, RESET, 400);
		gmotor.moveMotor_b = RESET;
	}

	if(gmotor.yourMotor_f == SET)
	{
		motorRun(your_motor, SET, 400);
		gmotor.yourMotor_f = RESET;
	}

	if(gmotor.yourMotor_b == SET)
	{
		motorRun(your_motor, RESET, 400);
		gmotor.yourMotor_b = RESET;
	}
}

//test
void  test595(void)
{
	unsigned char i;
	
	for(i = 0; i <8; i++)
	{
		HC595_RCK = 0;
		bsp_sendDate_595(motoForward_YOUR[i]);
		HC595_RCK = 1;
	}
}


