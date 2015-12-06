#include <lpc23xx.h>

#include "includes.h"



#include <ctype.h>
#include <math.h>




int main(){
	
	int i,j, retv;

	
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
	
  
	//data.threshold = 35;
	
while ((retv = finit (NULL)) != 0) ;


	
	dataFlash.saveParams();
 	dataFlash.readParams();
	

	
	while(1){
	
	//led7.setNumLed7(data.temperature[0]);	//1
	//led7.setNumLed7(); 		//2
  //led7.setNumLed7(data.temperature[2]);					//3
	led7.setNumLed7(data.temperature[3]);        			 //4
  
		
	}
	
	

}



