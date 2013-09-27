/*
*  EEPROM 读、写、擦除
*/
#include <stc12c5a60s2.h>
#include <intrins.h>

#include "bsp_eeprom.h"
#include "usart.h"
#include "app.h"

/*
* disable ISP/IAP/EEPROM function 
* make mcu in a safe state
*/
void IapIdle()
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0; 
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0;
}

/*
* read one byte from ISP/IAP/EEPROM area
* input: addr 
* output: flash date
*/
unsigned char IapReadByte(unsigned int addr)
{
	unsigned char dat;

	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_READ;

	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;

	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;

	_nop_();

	dat = IAP_DATA;

	IapIdle();

	return dat;
}

/*
* program one byte to ISP/IAP/EEPROM area
* input: addr / dat
* output: none
*/
void IapProgramByte(unsigned int addr, unsigned char dat)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_PROGRAM;

	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;

	IAP_DATA = dat;

	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;

	_nop_();


	IapIdle();
	
}


/*
*  erase one sector area
* input: addr
* output: none
*/
void IapEraseSector(unsigned int addr)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_ERASE;

	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;

	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;

	_nop_();


	IapIdle();
	
}

/*********************************************************************************************
函数名：readEpdat
参  数：无
返回值：无
备  注：EEPROM  读数据
**********************************************************************************************/
unsigned char readEpdat(unsigned int addr, unsigned char* dat, unsigned char len)
{
	unsigned char i;

	for(i = 0; i < len; i++)
	{
		dat[i] = IapReadByte(addr + i);
	}

	return 1;
}

/*********************************************************************************************
函数名：writeEpdat
参  数：无
返回值：无
备  注：EEPROM  读数据
**********************************************************************************************/
void writeEpdat(unsigned int addr, unsigned char* dat, unsigned char len)
{
	unsigned char i;
	
	//清除数据
	IapEraseSector(addr);
	for(i = 0; i < len; i++)
	{
		if(IapReadByte(addr + i) != 0xff)
		{
			UART_Send_String("ERASE ERR!");
		}
	}

	//写入数据
	for(i = 0; i < len; i++)
	{
		IapProgramByte(addr + i, (unsigned char)dat[i]);
	}
}



//test_eeprom

void test_eeprom(void)
{
	unsigned int i;
	unsigned char dat;

	//清除数据
	IapEraseSector(IAP_ADDRESS);
	for(i = 0; i < 512; i++)
	{
		if(IapReadByte(IAP_ADDRESS + i) != 0xff)
		{
			goto err;
		}
	}

	_nop_();
	_nop_();
	_nop_();
	_nop_();

	//写入数据
	for(i = 0; i < 512; i++)
	{
		IapProgramByte(IAP_ADDRESS + i, (unsigned char)i);
	}

	_nop_();
	_nop_();
	_nop_();
	_nop_();

	//verify
	for(i = 0; i < 512; i++)
	{
		if((dat = IapReadByte(IAP_ADDRESS+i)) != (unsigned char)i)
		{
			goto err;
		}
		UART_Send_Char(dat);
	}

err:
	while(1)
	{
	}
}


extern void delayMS(unsigned int tim);
void test_eeprom2(void)
{
	DateStruct g;

	g.datePosition = 0xa;
	g.dateWaterpressure = 0x55;
	
	writeEpdat(IAP_ADDRESS, (unsigned char*)&g, sizeof(g));

	delayMS(1);
	
	readEpdat(IAP_ADDRESS, (unsigned char*)&gDate, sizeof(gDate));
	UART_Send_Char(gDate.datePosition);
	UART_Send_Char(gDate.dateWaterpressure);
}

