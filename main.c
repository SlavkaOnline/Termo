#include <lpc23xx.h>

#include "includes.h"



#include <ctype.h>
#include <math.h>

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"

U8  InReport[64];                                /* HID Input Report    */
                                            /*   Bit0   : Buttons  */
                                            /*   Bit1..7: Reserved */
U8 OutReport[63]; 


void GetInReport (void) {
	
	
   	U8* message = (U8*)&data;
   	memcpy(&InReport, message, sizeof(data));
	   
}


/*------------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *------------------------------------------------------------------------------*/
void SetOutReport (void) {

	int i;
	struct typePCtoMCnumber datasend;
	datasend = *((struct typePCtoMCnumber *)&OutReport);
	
	data.mode[0] = datasend.threshold[0];
	data.mode[1] = datasend.threshold[1];
	data.mode[2] = datasend.threshold[2];
	data.mode[3] = datasend.threshold[3];
	data.THdelay = datasend.THdelay;
	data.UTH = datasend.UTH;
	
	for (i = 0; i < 3; i++){
			if(datasend.alarm[i] == 0){
					disableAlarm(i, 1);
			}
	}
	setMode(datasend.currentMode);
	led7.setNumLed7(data.threshold);
}





int main(){
	
	int i,j;

	
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
	
	USB_Init();                               /* USB Initialization */
  USB_Connect(__TRUE); 
	
	led.setLed(PWR, LED_ON);
	
  
	data.threshold = 35;
	//data.codeError = 65;

	
//  	dataFlash.saveParams();
//  	dataFlash.readParams();
// 	

	
	while(1){
	
	//led7.setNumLed7(data.temperature[0]);	//1
	//led7.setNumLed7(); 		//2
  //led7.setNumLed7(data.temperature[2]);					//3
	//led7.setNumLed7(data.temperature[3]);        			 //4
  
	//led7.setNumLed7(data.threshold);
		data.codeError = 65;
	}
	
	


  

 	return 0;
 }
	

