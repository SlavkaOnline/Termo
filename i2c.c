#include "i2c.h"

struct type_DS1631 dataDS1631;

void initI2C()
{
	PCONP |= ( 1 << 19 );
	
	//инициализация пинов 
	PINSEL1 &= ~(0x3C0);
	PINSEL1 |=  (0x3C0);
	I21CONCLR = (0x6C);
	I21SCLH = (0x3C);
	I21SCLL = (0x3C);

	I21CONSET = (0x40);
}


int startI2C(){

	I21CONSET = (0x20);
	
	
	return 0;	
}


