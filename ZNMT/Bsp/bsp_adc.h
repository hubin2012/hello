/*
*  ADCͷ�ļ�
*/
#ifndef __BSP_ADC_H
#define __BSP_ADC_H


//��������
void adcInit(void);
float adcWork(unsigned char channel);
int tempCalculate(unsigned char channel);

#endif
