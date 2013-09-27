/*
*  定时器T0程序
*  用于程序定时
*/
#include <stc12c5a60s2.h>
#include <intrins.h>
#include "time.h"

#include "app.h"
#include "usart.h"
#include "bsp_key.h"
#include "bsp_buz.h"

//宏定义
#define TIM_NUM  4 //软定时器个数

//全局变量定义
TIM softTim[TIM_NUM]; //使用TIM0， 软件定时器

//函数申明
static void bsp_timDec(TIM *tim);

/*********************************************************************************************
函数名：T0_init
作  用：初始化定时器，开中断
参  数：无
返回值：无
结  果：设置SFR中T/C1和（或）T/C0相关参数
备  注：本函数控制T/C1和T/C0，不需要使用的部分可用//屏蔽
**********************************************************************************************/
void Tim_init (void)
{
	TMOD = 0x11; //高4位控制T/C1 [ GATE，C/T，M1，M0，GATE，C/T，M1，M0 ]

	TH1 = 0xFF; //16位计数寄存器T1高8位（写入初值）
	TL1 = 0xFF; //16位计数寄存器T1低8位
	ET1 = 1; //T/C1中断开关
	//TR1 = 1; //T/C1启动开关

	TH0 = 0x3C; //16位计数寄存器T0高8位
	TL0 = 0xB0; //16位计数寄存器T0低8位（0x3CB0 = 50mS延时）
	ET0 = 1; //T/C0中断开关
	//TR0 = 1; //T/C0启动开关
}


/*********************************************************************************************
函数名：bsp_startTim
作  用：启动一个定时
参  数：id 软件定时器  period  定时时间
返回值：无
结  果：
备  注：
**********************************************************************************************/
void bsp_startTim(unsigned char id, unsigned int period)
{
	//定时器有效性判断
	if(id >= TIM_NUM)
	{
		UART_Send_String("tim id error!");
		
		return;
	}
	
	EA = 0; //关中断

	softTim[id].mode = MANUAL_TIM;
	softTim[id].flag = 0;
	softTim[id].countValue = period;
	softTim[id].reloadValue = 0;
	
	EA = 1; //开中断
}

/*********************************************************************************************
函数名：bsp_startAutoTim
作  用：启动一个定时
参  数：id 软件定时器  period  定时时间
返回值：无
结  果：
备  注：
**********************************************************************************************/
void bsp_startAutoTim(unsigned char id, unsigned int period)
{
	//定时器有效性判断
	if(id >= TIM_NUM)
	{
		UART_Send_String("tim id error!");
		
		return;
	}
	
	EA = 0; //关中断

	softTim[id].mode = AUTO_TIM;
	softTim[id].flag = 0;
	softTim[id].countValue = period;
	softTim[id].reloadValue = period;
	
	EA = 1; //开中断
}



/*********************************************************************************************
函数名：bsp_stopTim
作  用：停止一个定时
参  数：id 软件定时器
返回值：无
结  果：
备  注：
**********************************************************************************************/
void bsp_stopTim(unsigned char id)
{
	//定时器有效性判断
	if(id >= TIM_NUM)
	{
		UART_Send_String("tim id error!");
		
		return;
	}
	
	EA = 0; //关中断

	softTim[id].mode = MANUAL_TIM;
	softTim[id].flag = 0;
	softTim[id].countValue = 0;
	softTim[id].reloadValue = 0;
	
	EA = 1; //开中断
}

/*********************************************************************************************
函数名：bsp_checkTim
作  用：查询一个定时
参  数：id 软件定时器
返回值：0 - 定时间未到  1 - 定时时间到
结  果：
备  注：
**********************************************************************************************/
unsigned char bsp_checkTim(const unsigned char id)
{
	//定时器有效性判断
	if(id >= TIM_NUM)
	{
		UART_Send_String("tim id error!");
		return 0;
	}

	if(softTim[id].flag == 1)
	{
		softTim[id].flag = 0;
		
		return 1;
	}
	else
	{
		return 0;
	}
	
}

/*********************************************************************************************
函数名：bsp_timDec
作  用：启动一个定时
参  数：tim 指针
返回值：无
结  果：
备  注：
**********************************************************************************************/
static void bsp_timDec(TIM *tim)
{
	if(tim->countValue > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--tim->countValue == 0)
		{
			tim->flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(tim->mode == AUTO_TIM)
			{
				tim->countValue = tim->reloadValue;
			}
		}
	}
}

/*********************************************************************************************
函数名：定时/计数器0中断处理函数
调  用：[T/C0溢出后中断处理]
参  数：无
返回值：无
结  果：重新写入16位计数寄存器初始值，处理用户程序
备  注：必须允许中断并启动T/C本函数方可有效，重新写入初值需和T_C_init函数一致
**********************************************************************************************/
void T_C0 (void) interrupt 1 
{
	static unsigned int  timCounter = 0, tim2= 0;
	unsigned char i;
	
	//切换寄存器组到1
	TH0 = 0xfc; //16位计数寄存器T0高8位（重新写入初值） fc
	TL0 = 0x17; //16位计数寄存器T0低8位（0x3CB0 = 50mS延时） 17
	
	//软件定时器时间更新
	for(i = 0; i < TIM_NUM; i++)
	{
		bsp_timDec(&softTim[i]);
	}

	//人体检测
	HumDetect();
	
	//定时15ms, 用于按键扫描
	if(++timCounter > 15)
	{
		timCounter = 0;

		keyProess();
		getKey();
	}

	//LED指示灯显示
	if(tim2++ > 1)
	{
		tim2 = 0;
		led_d();
	}

}
	

/*********************************************************************************************
函数名：定时/计数器1中断处理函数
参  数：无
返回值：无
结  果：重新写入16位计数寄存器初始值，处理用户程序
备  注：必须允许中断并启动T/C本函数方可有效，重新写入初值需和T_C_init函数一致
**********************************************************************************************/
void T_C1(void) interrupt 3 
{
	BuzProsse();
}
