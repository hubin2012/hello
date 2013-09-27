/*
*   PWM ���
*/

#include <stc12c5a60s2.h>

/*********************************************************************************************
��������pwmInit
��  �ã�
��  ����
����ֵ��
��  ע��PWM��ʼ��
**********************************************************************************************/
void pwmInit(void)
{
	CCON = 0;

	CL = 0;
	CH = 0;
	CMOD = 0x02;  //0x02

	CCAP0H = CCAP0L = 0xf0;
	CCAPM0 = 0x42;
	CCAPM1 = 0;
/*
	CCAP1H = CCAP1L = 0x80;
	//PCA_PWM1 = 0x03;
	CCAPM1 = 0x40;  //��ʼ������PWM
*/
	//CR = 1;
}

void pwmAdd(void)
{
	CCAP0H -= 5;
	CCAP0L -= 5;
}

void pwmDec(void)
{
	CCAP0H += 5;
	CCAP0L += 5;
}


void PCA_isr() interrupt 7 
{
	
}