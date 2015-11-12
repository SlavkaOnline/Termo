#ifndef softTimer_H_
#define softTimer_H_

#include <lpc23xx.h>

#include "includes.h"



void initSoftTimers();
void startTHdTimer(int, int);

struct t_readPortTimer{
	
	int multiplier;
  int _multiplier;
	int delay;
	int _delay;
	int pin;
	int mode;
	int hold;
	void (*TimerFunc)();
	
};

struct t_blinkLedTimer{
	
	int multiplier;
	int _multiplier;
	int flag[11];
	void (*TimerFunc)();
	
};

struct t_SSP1Timer{
	int multiplier;
	int _multiplier;
	int time;
	int _time; 
	void (*TimerFunc)(struct t_SSP1Timer*);
	
};

struct t_SPITimer{
	int multiplier;
	int _multiplier;
	int time;
	int _time; 
	void (*TimerFunc)(struct t_SPITimer*);
	
};

struct t_THdTimer{
	
	int multiplier;
	int start;
	int stop;
};

void portCommand(struct t_readPortTimer*);
void BlinkLed(struct t_blinkLedTimer*);
void recSSP1OnTimer();
void recSPIOnTimer();


extern struct t_readPortTimer readPortTimer[10]; 
extern struct t_blinkLedTimer blinkLedTimer;
extern struct t_SSP1Timer ssp1Timer;
extern struct t_SPITimer spiTimer;
extern struct t_THdTimer THdTimer[4];
#endif