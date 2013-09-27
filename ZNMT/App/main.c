/*
*       智能马桶 
*	单片机型号: STC12C5A60S2
*   时间:  2013-7-18
*   版本:  v0.1
*/

#include <stc12c5a60s2.h>
#include "config.h"

//宏定义
#define ClearWDG()	{WDT_CONTR |= 1 << 4;}  //清看门狗

//函数声明
static void Init_Board(void);
void delayMS(unsigned int tim);

extern void motorRun(motorEnum motor, STATUS dir, unsigned int step);

/*********************************************************************************************
函数名：main
作  用: 程序入口
参  数：无
返回值：无
**********************************************************************************************/
int main(void)
{	
	ClearWDG(); //关看门狗
	
	//资源初始化
	Init_Board();

	//系统初始化
	System_Init();
	
	#ifdef debug
		UART_Send_String("Start Ok!");
	#endif

	TR0 = 1; //开启定时器0
	EA = 1; //开总中断
	

	while(1)
	{
	}
	
	return 0;
}


//初始化外围接口
static void Init_Board(void)
{
	/* IO配置 */
	GPIO_Init();
	
	/* 串口初始化 */
	UART_init();

	/* 定时器T0 T1初始化 */
	Tim_init();

	/* PWM初始化 */
	pwmInit();

	/* ADC初始化 */
	adcInit();
}


/*********************************************************************************************
函数名：delayMS
作  用: 延时
参  数：延时时间，单位毫秒 
返回值：无
**********************************************************************************************/
void delayMS(unsigned int tim)
{
	unsigned int i;

	while(--tim != 0)
	{
		for(i = 0; i < 600; i++)
		{
		}
	}
}


void delay_us(unsigned int tim)
{
	while(--tim > 0)
	{
	}
}

