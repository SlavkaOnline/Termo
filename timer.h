#ifndef timer_H_
#define timer_H_

#include <lpc23xx.h>

#include "includes.h"

void initTimer0();
void startTimer0(int);

void initWDTimer();
void startWDTimer();



struct typeTimer{
	
	void (*initTimer0)();
	void (*startTimer0)(int);
	void (*initWDTimer)();
  void (*startWDTimer)();
	
};

extern struct typeTimer timer;

#endif