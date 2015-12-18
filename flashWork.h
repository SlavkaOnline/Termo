
#ifndef flashWork_H_
#define flashWork_H_

#include "includes.h"

#define SETNULLHOUR
#define UPDATEHOUR
#define READHOUR

int saveParams();
int readParams();
double SumWorkTime();
void setAlarmDateTime();
struct typeDateTimeSend getAlarmDateTime();

struct typeFlash{
int (*saveParams)();
int (*readParams)();
double (*SumWorkTime)();
void (*setAlarmDateTime)();
struct typeDateTimeSend (*getAlarmDateTime)();	
};

extern struct typeFlash dataFlash;

#endif
