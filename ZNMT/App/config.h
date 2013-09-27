/*
*  全局宏定义
*/
#ifndef __CONFIG_H
#define __CONFIG_H

//宏定义
#define debug 1   //控制打印信息
typedef enum {RESET = 0, SET = !RESET}STATUS;

//头文件
#include "app.h"
#include "usart.h"
#include "time.h"

#include "bsp_key.h"
#include "bsp_adc.h"
#include "bsp_moto.h"
#include "bsp_buz.h"
#include "bsp_pwm.h"
#include "bsp_gpio.h"
#include "bsp_eeprom.h"


#include <stdio.h>

#endif


