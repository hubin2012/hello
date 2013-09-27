/*
*串口头文件
*/

#ifndef __USART_H
#define __USART_H


//函数声明
void UART_init (void);
void UART_Send_Char (unsigned char UART_data);
void UART_Send_String (unsigned char *str);

#endif

