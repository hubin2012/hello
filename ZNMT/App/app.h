/*
* ϵͳͷ�ļ�
*/

#ifndef __APP_H
#define __APP_H

//�ṹ�嶨��
typedef struct
{
	unsigned char waterPressure;  //ˮѹ
	unsigned char fillWater;  //��ˮ
	unsigned char seatHeat; //���¼���
	unsigned char waterLevel; //ˮλ

	
	unsigned char sensorHum; //�����Ӧ

	unsigned char buzOn; //�������� 

	unsigned char autoRun; //�Զ�ģʽ

	int wTemp;  //ˮ��
	int sTemp;  //����

}System;
extern System gCloseTool;


//EPROM�洢������
typedef struct
{
	//unsigned char dateFlag;
	unsigned char datePosition;  //��ˮͷλ��
	unsigned char dateWaterpressure; //ˮѹ
	unsigned char windTH;   //����
	int dateWaterTH; //ˮ���¶�
	int dateSeatTH; //�����¶�
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


//��������
void System_Init(void);
void System_Work(void);
void System_Alarm(void);
void System_Autorun(void);

//��������ģ�麯��
void Clean(void);
void HumDetect(void);


//ˮ������
void waterPres_on();
void waterPres_off();
void waterPres(unsigned char pr);



#endif

