/*
*       ������Ͱ 
*	��Ƭ���ͺ�: STC12C5A60S2
*   ʱ��:  2013-7-18
*   �汾:  v0.1
*/

#include <stc12c5a60s2.h>
#include "config.h"

//�궨��
#define ClearWDG()	{WDT_CONTR |= 1 << 4;}  //�忴�Ź�

//��������
static void Init_Board(void);
void delayMS(unsigned int tim);

extern void motorRun(motorEnum motor, STATUS dir, unsigned int step);

/*********************************************************************************************
��������main
��  ��: �������
��  ������
����ֵ����
**********************************************************************************************/
int main(void)
{	
	ClearWDG(); //�ؿ��Ź�
	
	//��Դ��ʼ��
	Init_Board();

	//ϵͳ��ʼ��
	System_Init();
	
	#ifdef debug
		UART_Send_String("Start Ok!");
	#endif

	TR0 = 1; //������ʱ��0
	EA = 1; //�����ж�
	

	while(1)
	{
	}
	
	return 0;
}


//��ʼ����Χ�ӿ�
static void Init_Board(void)
{
	/* IO���� */
	GPIO_Init();
	
	/* ���ڳ�ʼ�� */
	UART_init();

	/* ��ʱ��T0 T1��ʼ�� */
	Tim_init();

	/* PWM��ʼ�� */
	pwmInit();

	/* ADC��ʼ�� */
	adcInit();
}


/*********************************************************************************************
��������delayMS
��  ��: ��ʱ
��  ������ʱʱ�䣬��λ���� 
����ֵ����
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

