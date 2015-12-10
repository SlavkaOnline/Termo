#ifndef flashWork_H_
#define flashWork_H_

#include "includes.h"

void saveParams();
void readParams();
double updateSumWorkTime();

struct typeFlash{
	
void (*saveParams)();
void (*readParams)();
double (*updateSumWorkTime)();		
};

extern struct typeFlash dataFlash;


#endif