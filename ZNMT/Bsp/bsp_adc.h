/*
*  ADC头文件
*/
#ifndef __BSP_ADC_H
#define __BSP_ADC_H


//函数声明
void adcInit(void);
float adcWork(unsigned char channel);
int tempCalculate(unsigned char channel);

#endif
