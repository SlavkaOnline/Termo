
#include <lpc23xx.h>

#include "includes.h"



#include <ctype.h>
#include <math.h>


int main(){
	
	int i,j, retv;
  struct tm loc_time;
	
	while ((retv = finit (NULL)) != 0) ;
	USB_Init();                               /* USB Initialization */
  USB_Connect(__TRUE);
	
	/* Инициализация */
	defaultData();
	initGPIO();
	setMode(0);
	initVIC();
	//testLM75();
	initSoftTimers();
	timer.initTimer0();
	timer.startTimer0(1);
	//timer.initWDTimer();
  //timer.startWDTimer();
	
	 
	
	
	led.setLed(PWR, LED_ON);

  RTC_init(0);
	
  	//dataFlash.saveParams();
  	dataFlash.readParams();

// 	loc_time.tm_sec		=	0;
// 	loc_time.tm_min		=	0;
// 	loc_time.tm_hour	=	0;
// 	loc_time.tm_mday	=	1;
// 	loc_time.tm_mon		=	1;
// 	loc_time.tm_year	=	2015;
//   
// 	correct_time_struct(&loc_time);
	RTC_int();
	
	while(1){
	
		//led7.setNumLed7(RTC_MIN);
	//led7.setNumLed7(data.threshold);	//1
	//led7.setNumLed7(); 		//2
  //led7.setNumLed7(data.temperature[2]);					//3
	//led7.setNumLed7(data.temperature[3]);        			 //4
  
	//led7.setNumLed7(data.threshold);
		
	}
	
	


  

 	return 0;
 }
	


