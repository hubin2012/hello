/*
* app ϵͳ�߼���
*/

#include <stc12c5a60s2.h>
#include <intrins.h>
#include <string.h>

#include "config.h"

//�˿ڶ���
sbit waterSwitch = P3^7; //��ˮ��ŷ�
sbit waterHeat = P3^4; //ˮ�����
sbit dryHeat = P3^5;  //�ȷ��
sbit seatHeat = P3^6; //�������
sbit hDet = P3^2; //������
sbit waterLevel = P1^4; //Һ��

//�궨��
#define CLEANORWOMEN_TIM   55 * 1000   //ϴ������Ů�� ��ϴʱ��


//ȫ�ֱ���
System gCloseTool;
cleanEnum cleanStatus = clean_Idle;

DateStruct gDate, g_init = {0xaa, 0x01, 0x01, 0x0022, 0x0022};

//��������
extern void delayMS(unsigned int tim);

static void InputUpdate(void);
static void openWater(void);
static void closeWater(void);
static void openSeatHeat(void);
static void closeSeatHeat(void);


static void Gvariable_Init(void)
{
	// ������ʼ��
	memset(&gCloseTool, 0, sizeof(gCloseTool));
	memset(&gmotor, 0, sizeof(gmotor));
	gmotor.moveMotor_step = 400;
}


/*********************************************************************************************
��������System_Init
��  ������
����ֵ����
��  ע������ϵͳ��ʼ��
**********************************************************************************************/
void System_Init(void)
{
	//ȫ�ֱ�����ʼ��
	Gvariable_Init();
	
	//������ ��һ��
	//BuzOn();

	//LED����3s
	ledStart();
#if 0
	InputUpdate(); //״̬����

	//��EEPROM
	readEpdat(IAP_ADDRESS, (unsigned char*)&gDate, sizeof(gDate));
	if(gDate.datePosition != 0xaa)
	{
		writeEpdat(IAP_ADDRESS, (unsigned char *)&g_init, sizeof(g_init));
	}
	
	
	//ˮ��ˮλ���
	if(gCloseTool.fillWater == 0x01)
	{
		//ˮλ������������ŷ���������ˮ P37
		openWater();
	}
	else
	{
		closeWater();
	}

	//ˮ���¶�
	if(gCloseTool.wTemp < gDate.dateWaterTH)
	{
		//��������
	}

	//���¼��
	if(gCloseTool.sTemp < gDate.dateSeatTH)
	{
		//��������
	}
#endif
}


/*********************************************************************************************
��������System_Work
��  �ã�
��  ������
����ֵ����
��  ע��ϵͳ��������
**********************************************************************************************/
void System_Work(void)
{
	//�����Ӧ���
	if(gCloseTool.sensorHum == 0x01)
	{
		//ϴ��

		//Ů��
		
		//���
		
	}

	//��Ħ

	//���µ���

	//����

	//����

	//ˮѹ����

	//ˮ�µ���

	//ˮλ���

	//������

	//ǿ��ˮ����
	
}

/*********************************************************************************************
��������System_Alarm
��  �ã�
��  ������
����ֵ����
��  ע��ϵͳ����
**********************************************************************************************/
void System_Alarm(void)
{
	//ˮ�±���

	//���±���

	//ˮ����ȳ�ʱ����
}

/*********************************************************************************************
��������System_Autorun
��  �ã�
��  ������
����ֵ����
��  ע���Զ�����
**********************************************************************************************/
void System_Autorun(void)
{
	
}





/*********************************************************************************************
��������InputUpdate
��  �ã�
��  ����
����ֵ��
��  ע���������ⲿ״̬����
**********************************************************************************************/
static void InputUpdate(void)
{	
	// ˮ��ˮλ���
	if(waterLevel) //0001 0000
	{
		//������ˮ������ˮ
		gCloseTool.fillWater = 0x01;
	}
	else
	{
		gCloseTool.fillWater = 0x0;
	}
	
	//ˮ���¶�
	gCloseTool.wTemp =  tempCalculate(waterTHchanne);

	//�����¶�
	gCloseTool.sTemp = tempCalculate(seatTHchanne);
}

/*********************************************************************************************
��������Watersurface
��  �ã�
��  ������
����ֵ����
��  ע��Һ����
**********************************************************************************************/
void Watersurface(void)
{
	
}


/*********************************************************************************************
��������HumDetect
��  �ã�
��  ������
����ֵ����
��  ע��������,�ڶ�ʱ�������ڵ���
**********************************************************************************************/
void HumDetect(void)
{
	unsigned char temp;
	static unsigned int humTim = 0, humTim_end = 0;

	temp = P3;

	if((temp & 0x04) == 0)
	{
		//�Ӵ����
		//UART_Send_Char(gCloseTool.sensorHum);
		
		if(gCloseTool.sensorHum == 0x0)
		{
			if(humTim++ > 1000)
			{
				UART_Send_String("detect");
				humTim = 0;
				gCloseTool.sensorHum = 0x01;	
				
			}
		}
	}
	else if((temp & 0x04) == 0x04)
	{
		//�뿪���
		if(gCloseTool.sensorHum == 0x01)
		{
			if(humTim_end++ > 2000)
			{
				UART_Send_String("likai!");
				humTim_end = 0;
				gCloseTool.sensorHum = 0x0;		
			}
		}
	}
	
}


/*********************************************************************************************
��������Clean
��  �ã�
��  ������
����ֵ����
��  ע��ϴ������
**********************************************************************************************/
void Clean(void)
{
	switch(cleanStatus)
	{
		case clean_Idle:
			if((keyValue & 0x400) == 0x400)
			{
				// ��ϴ��������
				gmotor.yourMotor_f = SET;
				gmotor.moveMotor_f = SET;
				
				cleanStatus = clean_Work;
			}
			break;

		case clean_Work:
			motor();

			waterPres_on();
			waterPres(20);

			bsp_startTim(CLEAN_TIM_ID, 5000);
			cleanStatus = clean_Wait;
			
			break;

		case clean_Wait:
			if(bsp_checkTim(CLEAN_TIM_ID) == 1)
			{
				//ʱ�䵽
				waterPres(30);
				bsp_startTim(CLEAN_TIM_ID, CLEANORWOMEN_TIM);
				cleanStatus = clean_Exit;
			}
			
			break;

		case clean_Exit:
			if((bsp_checkTim(CLEAN_TIM_ID) == 1) || ((keyValue & 0x400) == 0))
			{
				waterPres_off();
				
				gmotor.moveMotor_b = SET;
				gmotor.yourMotor_b = SET;

				motor();

				cleanStatus = clean_Idle;
			}
			
			break;

		default:
			break;
	}
}

/*********************************************************************************************
��������addWater
��  ������
����ֵ����
��  ע������ˮ��
**********************************************************************************************/
static void openWater(void)
{
	waterSwitch = 0;

}

/*********************************************************************************************
��������closeWater
��  ������
����ֵ����
��  ע���ر�ˮ��
**********************************************************************************************/
static void closeWater(void)
{
	waterSwitch = 1;
}

/*********************************************************************************************
��������openWaterHeat
��  ������
����ֵ����
��  ע������ˮ�����
**********************************************************************************************/
static void openWaterHeat(void)
{

}

/*********************************************************************************************
��������openWaterHeat
��  ������
����ֵ����
��  ע���ر�ˮ�����
**********************************************************************************************/
static void closeWaterHeat(void)
{
	
}

/*********************************************************************************************
��������openSeatHeat
��  ������
����ֵ����
��  ע�������������
**********************************************************************************************/
static void openSeatHeat(void)
{
	seatHeat = 0;
}

/*********************************************************************************************
��������closeSeatHeat
��  ������
����ֵ����
��  ע���ر��������
**********************************************************************************************/
static void closeSeatHeat(void)
{
	seatHeat = 1;
}

/*********************************************************************************************
��������watePres_on
��  ������
����ֵ����
��  ע��ˮ�� PWM���ƿ���
**********************************************************************************************/
void waterPres_on()
{
	CR = 1;	
}

/*********************************************************************************************
��������waterPres_off
��  ������
����ֵ����
��  ע��ˮ�� PWM���� �ر�
**********************************************************************************************/
void waterPres_off()
{
	CR = 0;
}


/*********************************************************************************************
��������waterPreFun
��  ����ˮѹ��ֵ
����ֵ����
��  ע������ˮѹ
**********************************************************************************************/
extern void pwmAdd(void);
extern void pwmDec(void);
void waterPres(unsigned char pr)
{
	unsigned char Voltage = 0;
	float tmp;

	while(1)
	{
		//P16 ADC����
		tmp = adcWork(6);
		Voltage = (unsigned char)(10 * tmp);

		//����PWM P13
		if(Voltage > (pr + 1))
		{
			pwmDec();
			
		}
		else if(Voltage < (pr - 1))
		{
			pwmAdd(); //�ı�ռ�ձ�
		}

		//���������ʵ�ѹ��������
		if((Voltage > (pr + 1)) && (Voltage < (pr - 1)))
		{
			break;
		}
	}
	
	//UART_Send_Char(Voltage);

}


void waterPrestest(void)
{
	unsigned char Voltage = 0;
	float tmp;
	
	gCloseTool.waterPressure = 1;

	while(1)
	{
		//P16 ADC����
		tmp = adcWork(6);
		Voltage = (unsigned char)(10 * tmp);

	
		bsp_startTim(0, 1000);

		while(bsp_checkTim(0) != 1)
		{
			
		}
	
		//delayMS(1000);
		
		//����PWM P13
		if(Voltage > 24)
		{
			pwmAdd();
			
		}
		else if(Voltage < 22)
		{
			pwmDec(); //�ı�ռ�ձ�
		}
		
		
		UART_Send_Char(Voltage);
	}

}

