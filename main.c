
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
	
	//fformat ("S:");
	 
RTC_init(0);
RTC_int();	
	
	
  
	//dataFlash.saveParams();
 	dataFlash.readParams();

// loc_time.tm_sec		=	0;
// loc_time.tm_min		=	35;
// loc_time.tm_hour	=	22;
// loc_time.tm_mday	=	18;
// loc_time.tm_mon		=	12;
// loc_time.tm_year	=	2015;
//  
// correct_time_struct(&loc_time);
// correct_data_struct(&loc_time);


  //dataFlash.SumWorkTime(SETNULLHOUR);
 	//SumWorkTime(READHOUR);
	
	
	while(1){
	
		led.setLed(PWR, LED_ON);
		
		//led7.setNumLed7(RTC_MIN);
	//led7.setNumLed7(data.threshold);	//1
	//led7.setNumLed7(); 		//2
  //led7.setNumLed7(data.chanelError[3]);					//3
	led7.setNumLed7(data.temperature[3]);        			 //4
  
	//led7.setNumLed7(data.threshold);
		
	}
	
	


  

 	return 0;
 }
	


