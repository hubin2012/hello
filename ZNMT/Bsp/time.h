//定时器头文件

#ifndef __TIME_H
#define __TIME_H

//数据类型定义
typedef enum
{
	MANUAL_TIM,
	AUTO_TIM,
};


//软定时器结构体
typedef struct 
{
	volatile unsigned char mode; //计数模式
	volatile unsigned char flag; //时间到达标志
	volatile unsigned int countValue; //时间计时
	volatile unsigned int reloadValue;//重新加载值

}TIM;

//软件定时器
typedef enum
{
	CLEAN_TIM_ID,

};


//函数申明
void Tim_init (void);

void bsp_startTim(unsigned char id, unsigned int period);
void bsp_startAutoTim(unsigned char id, unsigned int period);
void bsp_stopTim(unsigned char id);
unsigned char bsp_checkTim(unsigned char id);


#endif


