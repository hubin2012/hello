/*
*  EEPROM 头文件
*/

#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H


//宏定义
/* define ISP/IAP/EEPROM command */
#define CMD_IDLE 0 //stand-by
#define CMD_READ 1 //byte-read
#define CMD_PROGRAM 2 //byte-program
#define CMD_ERASE  3   //Sector-erase


/* define ISP/IAP/EEPROM operation const for IAP_CONTR */
//#define ENABLE_IAP 0x80   //SYSCLK < 30Mhz
//#define ENABLE_IAP 0x81   //SYSCLK < 24Mhz
#define ENABLE_IAP 0x82   //SYSCLK < 20Mhz
//#define ENABLE_IAP 0x83   //SYSCLK < 12Mhz
//#define ENABLE_IAP 0x84   //SYSCLK < 6Mhz
//#define ENABLE_IAP 0x85   //SYSCLK < 3Mhz
//#define ENABLE_IAP 0x86   //SYSCLK < 2Mhz
//#define ENABLE_IAP 0x87   //SYSCLK < 1Mhz


/* start address for stc12c5a60s2 series eeprom */
#define IAP_ADDRESS 0x0000

//declare function 
unsigned char IapReadByte(unsigned int addr);
void IapProgramByte(unsigned int addr, unsigned char dat);
void IapEraseSector(unsigned int addr);

unsigned char readEpdat(unsigned int addr, unsigned char* dat, unsigned char len);
void writeEpdat(unsigned int addr, unsigned char* dat, unsigned char len);

#endif

