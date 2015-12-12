
#ifndef flashWork_H_
#define flashWork_H_

#include "includes.h"

int saveParams();
int readParams();
double updateSumWorkTime();

struct typeFlash{
int (*saveParams)();
int (*readParams)();
double (*updateSumWorkTime)();		
};

extern struct typeFlash dataFlash;

#endif
