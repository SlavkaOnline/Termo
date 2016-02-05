
#ifndef flashWork_H_
#define flashWork_H_

#include "includes.h"

#define SETNULLHOUR 0
#define UPDATEHOUR  1
#define READHOUR    2

int saveParams();
int readParams();
short SumWorkTime();
void setAlarmDateTime();
struct typeDateTimeSend getAlarmDateTime();

struct typeFlash{
int (*saveParams)();
int (*readParams)();
short (*SumWorkTime)();
void (*setAlarmDateTime)();
struct typeDateTimeSend (*getAlarmDateTime)();	
};

extern struct typeFlash dataFlash;

#endif
