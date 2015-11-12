#ifndef flashWork_H_
#define flashWork_H_

#include "includes.h"

void saveParams();
void readParams();
unsigned char MemoryReadStatus();

struct typeFlash{
	
void (*saveParams)();
void (*readParams)();
unsigned char (*MemoryReadStatus)();	
	
};

extern struct typeFlash dataFlash;


#endif