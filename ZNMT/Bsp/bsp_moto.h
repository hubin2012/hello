/*
*  MOTO Ctr
*/
#ifndef __BSP_MOTO_H
#define __BSP_MOTO_H

#include "Config.h"

//
typedef struct
{
	STATUS moveMotor_f;
	STATUS moveMotor_b;
	STATUS yourMotor_f;
	STATUS yourMotor_b;

	unsigned int moveMotor_step;
}motorStruct;
extern motorStruct gmotor;

typedef enum
{
	move_motor,
	your_motor,
}motorEnum;


//º¯ÊýÉùÃ÷
void motor(void);
void  test595(void);

#endif

