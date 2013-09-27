//��ʱ��ͷ�ļ�

#ifndef __TIME_H
#define __TIME_H

//�������Ͷ���
typedef enum
{
	MANUAL_TIM,
	AUTO_TIM,
};


//��ʱ���ṹ��
typedef struct 
{
	volatile unsigned char mode; //����ģʽ
	volatile unsigned char flag; //ʱ�䵽���־
	volatile unsigned int countValue; //ʱ���ʱ
	volatile unsigned int reloadValue;//���¼���ֵ

}TIM;

//�����ʱ��
typedef enum
{
	CLEAN_TIM_ID,

};


//��������
void Tim_init (void);

void bsp_startTim(unsigned char id, unsigned int period);
void bsp_startAutoTim(unsigned char id, unsigned int period);
void bsp_stopTim(unsigned char id);
unsigned char bsp_checkTim(unsigned char id);


#endif


