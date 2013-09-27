/*
*  按键扫描头文件
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

//关键定义
sbit P00 = P0^0;
sbit P01 = P0^1;
sbit P02 = P0^2;
sbit P03 = P0^3;

sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26 = P2^6;
sbit P27 = P2^7;


typedef enum 
{
	KEY_NONE,
		
	KEY_SW13 = 0x7e,
	KEY_SW11 = 0x7d,
	KEY_SW12 = 0xbe,
	KEY_SW6 = 0xbd,
	KEY_SW8 = 0xbb,
	KEY_SW9 = 0xb7,
	KEY_SW4 = 0xde,
	KEY_SW3 = 0xdd,
	KEY_SW7 = 0xdb,
	KEY_SW10 = 0xd7,
	KEY_SW14 = 0xee,
	KEY_SW5 = 0xeb,
	KEY_SW2 = 0xe7,
	KEY_SW9_SW10 = 0x97, //组合按键用于消除人体感应
	KEY_SW12_SW13 = 0x3e,//组合键用于启动 自动模式
};

typedef enum
{
	LED1,
	LED2,
	LED3,
	LED5,
	LED6,
	LED7,
	LED8,
	LED9,
	LED10,
	LED11,
	LED12,
	LED13,
	LED14,
	LED15,
	LED16,
	LED17,
};

typedef enum
{
	CLEAN,
	WOMENUSE,
	DRY,
	STRESS,
	MOVE,
	ENERGYSAVE,
	SEATTEMP,
	WATERTEMP,
	DEODORIZE,
	CHIRISMUS,
	CANCEL,
	AUTOMOD
};


//按键结构体
typedef struct
{
	volatile unsigned char keyOK;
	volatile unsigned char keyValue;
}keySTRUCT;

extern keySTRUCT gKey;

extern unsigned int keyValue;

//函数声明
//key
void keyProess(void);
void getKey(void);

//led
void ledDisplay(void);
void led_d(void);
void testled(void);

#endif


