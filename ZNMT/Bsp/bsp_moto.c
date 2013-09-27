/*
*   uln2003/74hc595 驱动MOTO
*/

#include <stc12c5a60s2.h>
#include <intrins.h>
#include "bsp_moto.h"

//控制管脚定义
sbit HC595_SI = P4^6; //串行输入
sbit HC595_RCK = P2^0;//锁存
sbit HC595_SCK = P2^1;//时钟

//全局变量声明
motorStruct gmotor;


//电机转动的值
unsigned char motoBack_Move[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};  //八拍
unsigned char motoForward_Move[8] = {0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01};
//unsigned char motoBack_Move[8] = {0x03,0x06, 0x0c, 0x09};  //双向四拍
//unsigned char motoForward_Move[8] = {0x09, 0x0c, 0x06, 0x03};

//unsigned char motoBack_Move[8] = {0x01,0x02, 0x04, 0x08};  //单向
//unsigned char motoForward_Move[8] = {0x08, 0x04, 0x02, 0x01};


unsigned char motoForward_YOUR[8] = {0x10, 0x30, 0x20, 0x60, 0x40, 0xc0, 0x80, 0x90};
unsigned char motoBack_YOUR[8] = {0x90, 0x80, 0xc0, 0x40, 0x60, 0x20, 0x30, 0x10};


extern void delayMS(unsigned int tim);
/*********************************************************************************************
函数名：bsp_sendDate_595
作  用: 串行发送数据，按位发送
参  数：要发送的数据
返回值：无
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

		//上升发送
		HC595_SCK = 0;
		delayMS(2);
		
		
		HC595_SCK = 1;
	}
}


/*********************************************************************************************
函数名：bsp_lockDate_595
作  用: 输出口数据锁存
参  数：无
返回值：无
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
函数名：motoRun
作  用: 输出口数据锁存
参  数：motor --- 对应不同的电机   dir -----对应电机运动方向   step ------电机运动的距离
返回值：无
**********************************************************************************************/
void motorRun(motorEnum motor, STATUS dir, unsigned int step)
{
	unsigned char i;

	//排水阀
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

	//喷水管移动电机
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

//电机
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


