/*
* 系统头文件
*/

#ifndef __APP_H
#define __APP_H

//结构体定义
typedef struct
{
	unsigned char waterPressure;  //水压
	unsigned char fillWater;  //上水
	unsigned char seatHeat; //座温加热
	unsigned char waterLevel; //水位

	
	unsigned char sensorHum; //人体感应

	unsigned char buzOn; //开蜂鸣器 

	unsigned char autoRun; //自动模式

	int wTemp;  //水温
	int sTemp;  //座温

}System;
extern System gCloseTool;


//EPROM存储的数据
typedef struct
{
	//unsigned char dateFlag;
	unsigned char datePosition;  //喷水头位置
	unsigned char dateWaterpressure; //水压
	unsigned char windTH;   //风温
	int dateWaterTH; //水箱温度
	int dateSeatTH; //座温温度
}DateStruct;
extern DateStruct gDate;

typedef enum
{
	waterTHchanne,
	seatTHchanne,
};

typedef enum
{
	clean_Idle,
	clean_Work,
	clean_Wait,
	clean_Exit,
}cleanEnum;


//函数声明
void System_Init(void);
void System_Work(void);
void System_Alarm(void);
void System_Autorun(void);

//各个功能模块函数
void Clean(void);
void HumDetect(void);


//水阀控制
void waterPres_on();
void waterPres_off();
void waterPres(unsigned char pr);



#endif

