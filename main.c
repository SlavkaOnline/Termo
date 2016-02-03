
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

  //dataFlash.SumWorkTime(SETNULLHOUR);
 	//SumWorkTime(READHOUR);
	//data.threshold = 30;
	
	while(1){
	
		led.setLed(PWR, LED_ON);
		
		//led7.setNumLed7(RTC_MIN);
	//led7.setNumLed7(data.threshold);	//1
	//led7.setNumLed7(); 		//2
  //led7.setNumLed7(data.chanelError[3]);					//3
	//led7.setNumLed7(data.temperature[3]);        			 //4
  
	//led7.setNumLed7(data.threshold);
//		led7.setNumLed7(RTC_MIN);
	}
	
	


  

 	return 0;
 }
	


