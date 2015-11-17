#ifndef flashWork_H_
#define flashWork_H_

#include "includes.h"

void saveParams();
void readParams();


struct typeFlash{
	
void (*saveParams)();
void (*readParams)();
		
};

extern struct typeFlash dataFlash;


#endif