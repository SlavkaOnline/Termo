#ifndef i2c_H_
#define i2c_H_

#include "includes.h"

struct type_DS1631{
	short RW;
	char Address;
  char REG;
  char BUFF_IN[7];
	char BUFF_OUT[5];
	short inc;
  short size;	
};

extern struct type_DS1631 dataDS1631;
void initI2C();
int startI2C();
int readI2C();



#endif