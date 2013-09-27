/*
* app 系统逻辑层
*/

#include <stc12c5a60s2.h>
#include <intrins.h>
#include <string.h>

#include "config.h"

//端口定义
sbit waterSwitch = P3^7; //上水电磁阀
sbit waterHeat = P3^4; //水箱加热
sbit dryHeat = P3^5;  //热风机
sbit seatHeat = P3^6; //坐垫加热
sbit hDet = P3^2; //人体检测
sbit waterLevel = P1^4; //液面

//宏定义
#define CLEANORWOMEN_TIM   55 * 1000   //洗净或者女用 冲洗时间


//全局变量
System gCloseTool;
cleanEnum cleanStatus = clean_Idle;

DateStruct gDate, g_init = {0xaa, 0x01, 0x01, 0x0022, 0x0022};

//函数声明
extern void delayMS(unsigned int tim);

static void InputUpdate(void);
static void openWater(void);
static void closeWater(void);
static void openSeatHeat(void);
static void closeSeatHeat(void);


static void Gvariable_Init(void)
{
	// 变量初始化
	memset(&gCloseTool, 0, sizeof(gCloseTool));
	memset(&gmotor, 0, sizeof(gmotor));
	gmotor.moveMotor_step = 400;
}


/*********************************************************************************************
函数名：System_Init
参  数：无
返回值：无
备  注：开机系统初始化
**********************************************************************************************/
void System_Init(void)
{
	//全局变量初始化
	Gvariable_Init();
	
	//蜂鸣器 响一声
	//BuzOn();

	//LED点亮3s
	ledStart();
#if 0
	InputUpdate(); //状态输入

	//读EEPROM
	readEpdat(IAP_ADDRESS, (unsigned char*)&gDate, sizeof(gDate));
	if(gDate.datePosition != 0xaa)
	{
		writeEpdat(IAP_ADDRESS, (unsigned char *)&g_init, sizeof(g_init));
	}
	
	
	//水箱水位检查
	if(gCloseTool.fillWater == 0x01)
	{
		//水位不够，开启电磁阀，进行上水 P37
		openWater();
	}
	else
	{
		closeWater();
	}

	//水箱温度
	if(gCloseTool.wTemp < gDate.dateWaterTH)
	{
		//启动加热
	}

	//座温检查
	if(gCloseTool.sTemp < gDate.dateSeatTH)
	{
		//启动加热
	}
#endif
}


/*********************************************************************************************
函数名：System_Work
调  用：
参  数：无
返回值：无
备  注：系统正常运行
**********************************************************************************************/
void System_Work(void)
{
	//人体感应检查
	if(gCloseTool.sensorHum == 0x01)
	{
		//洗净

		//女用
		
		//烘干
		
	}

	//按摩

	//座温调节

	//节能

	//除臭

	//水压调节

	//水温调节

	//水位检测

	//防冻结

	//强排水功能
	
}

/*********************************************************************************************
函数名：System_Alarm
调  用：
参  数：无
返回值：无
备  注：系统报警
**********************************************************************************************/
void System_Alarm(void)
{
	//水温报警

	//座温报警

	//水箱加热超时报警
}

/*********************************************************************************************
函数名：System_Autorun
调  用：
参  数：无
返回值：无
备  注：自动运行
**********************************************************************************************/
void System_Autorun(void)
{
	
}





/*********************************************************************************************
函数名：InputUpdate
调  用：
参  数：
返回值：
备  注：开机，外部状态读入
**********************************************************************************************/
static void InputUpdate(void)
{	
	// 水箱水位检测
	if(waterLevel) //0001 0000
	{
		//开启进水阀，上水
		gCloseTool.fillWater = 0x01;
	}
	else
	{
		gCloseTool.fillWater = 0x0;
	}
	
	//水箱温度
	gCloseTool.wTemp =  tempCalculate(waterTHchanne);

	//座温温度
	gCloseTool.sTemp = tempCalculate(seatTHchanne);
}

/*********************************************************************************************
函数名：Watersurface
调  用：
参  数：无
返回值：无
备  注：液面检测
**********************************************************************************************/
void Watersurface(void)
{
	
}


/*********************************************************************************************
函数名：HumDetect
调  用：
参  数：无
返回值：无
备  注：人体检测,在定时器中周期调用
**********************************************************************************************/
void HumDetect(void)
{
	unsigned char temp;
	static unsigned int humTim = 0, humTim_end = 0;

	temp = P3;

	if((temp & 0x04) == 0)
	{
		//接触检测
		//UART_Send_Char(gCloseTool.sensorHum);
		
		if(gCloseTool.sensorHum == 0x0)
		{
			if(humTim++ > 1000)
			{
				UART_Send_String("detect");
				humTim = 0;
				gCloseTool.sensorHum = 0x01;	
				
			}
		}
	}
	else if((temp & 0x04) == 0x04)
	{
		//离开检测
		if(gCloseTool.sensorHum == 0x01)
		{
			if(humTim_end++ > 2000)
			{
				UART_Send_String("likai!");
				humTim_end = 0;
				gCloseTool.sensorHum = 0x0;		
			}
		}
	}
	
}


/*********************************************************************************************
函数名：Clean
调  用：
参  数：无
返回值：无
备  注：洗净功能
**********************************************************************************************/
void Clean(void)
{
	switch(cleanStatus)
	{
		case clean_Idle:
			if((keyValue & 0x400) == 0x400)
			{
				// 清洗按键按下
				gmotor.yourMotor_f = SET;
				gmotor.moveMotor_f = SET;
				
				cleanStatus = clean_Work;
			}
			break;

		case clean_Work:
			motor();

			waterPres_on();
			waterPres(20);

			bsp_startTim(CLEAN_TIM_ID, 5000);
			cleanStatus = clean_Wait;
			
			break;

		case clean_Wait:
			if(bsp_checkTim(CLEAN_TIM_ID) == 1)
			{
				//时间到
				waterPres(30);
				bsp_startTim(CLEAN_TIM_ID, CLEANORWOMEN_TIM);
				cleanStatus = clean_Exit;
			}
			
			break;

		case clean_Exit:
			if((bsp_checkTim(CLEAN_TIM_ID) == 1) || ((keyValue & 0x400) == 0))
			{
				waterPres_off();
				
				gmotor.moveMotor_b = SET;
				gmotor.yourMotor_b = SET;

				motor();

				cleanStatus = clean_Idle;
			}
			
			break;

		default:
			break;
	}
}

/*********************************************************************************************
函数名：addWater
参  数：无
返回值：无
备  注：开启水阀
**********************************************************************************************/
static void openWater(void)
{
	waterSwitch = 0;

}

/*********************************************************************************************
函数名：closeWater
参  数：无
返回值：无
备  注：关闭水阀
**********************************************************************************************/
static void closeWater(void)
{
	waterSwitch = 1;
}

/*********************************************************************************************
函数名：openWaterHeat
参  数：无
返回值：无
备  注：开启水箱加热
**********************************************************************************************/
static void openWaterHeat(void)
{

}

/*********************************************************************************************
函数名：openWaterHeat
参  数：无
返回值：无
备  注：关闭水箱加热
**********************************************************************************************/
static void closeWaterHeat(void)
{
	
}

/*********************************************************************************************
函数名：openSeatHeat
参  数：无
返回值：无
备  注：开启坐垫加热
**********************************************************************************************/
static void openSeatHeat(void)
{
	seatHeat = 0;
}

/*********************************************************************************************
函数名：closeSeatHeat
参  数：无
返回值：无
备  注：关闭坐垫加热
**********************************************************************************************/
static void closeSeatHeat(void)
{
	seatHeat = 1;
}

/*********************************************************************************************
函数名：watePres_on
参  数：无
返回值：无
备  注：水泵 PWM控制开启
**********************************************************************************************/
void waterPres_on()
{
	CR = 1;	
}

/*********************************************************************************************
函数名：waterPres_off
参  数：无
返回值：无
备  注：水泵 PWM控制 关闭
**********************************************************************************************/
void waterPres_off()
{
	CR = 0;
}


/*********************************************************************************************
函数名：waterPreFun
参  数：水压的值
返回值：无
备  注：调整水压
**********************************************************************************************/
extern void pwmAdd(void);
extern void pwmDec(void);
void waterPres(unsigned char pr)
{
	unsigned char Voltage = 0;
	float tmp;

	while(1)
	{
		//P16 ADC采样
		tmp = adcWork(6);
		Voltage = (unsigned char)(10 * tmp);

		//启动PWM P13
		if(Voltage > (pr + 1))
		{
			pwmDec();
			
		}
		else if(Voltage < (pr - 1))
		{
			pwmAdd(); //改变占空比
		}

		//调整到合适的压力。跳出
		if((Voltage > (pr + 1)) && (Voltage < (pr - 1)))
		{
			break;
		}
	}
	
	//UART_Send_Char(Voltage);

}


void waterPrestest(void)
{
	unsigned char Voltage = 0;
	float tmp;
	
	gCloseTool.waterPressure = 1;

	while(1)
	{
		//P16 ADC采样
		tmp = adcWork(6);
		Voltage = (unsigned char)(10 * tmp);

	
		bsp_startTim(0, 1000);

		while(bsp_checkTim(0) != 1)
		{
			
		}
	
		//delayMS(1000);
		
		//启动PWM P13
		if(Voltage > 24)
		{
			pwmAdd();
			
		}
		else if(Voltage < 22)
		{
			pwmDec(); //改变占空比
		}
		
		
		UART_Send_Char(Voltage);
	}

}

