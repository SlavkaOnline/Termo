
#ifndef flashWork_H_
#define flashWork_H_

#include "includes.h"

int saveParams();
int readParams();
double updateSumWorkTime();
void setAlarmDateTime();
struct typeDateTimeSend getAlarmDateTime();

struct typeFlash{
int (*saveParams)();
int (*readParams)();
double (*updateSumWorkTime)();
void (*setAlarmDateTime)();
struct typeDateTimeSend (*getAlarmDateTime)();	
};

extern struct typeFlash dataFlash;

#endif
