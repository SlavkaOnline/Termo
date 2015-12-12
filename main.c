
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
void ChangeConstParam (void) {

	int i;
	struct typePCtoMCnumber datasend;
	datasend = *((struct typePCtoMCnumber *)&OutReport[1]);
	
	data.mode[0] = datasend.threshold[0];
	data.mode[1] = datasend.threshold[1];
	data.mode[2] = datasend.threshold[2];
	data.mode[3] = datasend.threshold[3];
	data.mode[4] = datasend.threshold[4];
	data.THdelay = datasend.THdelay;
	data.UTH = datasend.UTH;
	
	setMode(datasend.currentMode);
	
	while(dataFlash.saveParams());
	
// 	for (i = 0; i < 3; i++){
// 			if(datasend.alarm[i] == 0){
// 					disableAlarm(i, 1);
// 			}
// 	}
	
	
	
	
}

void SetOutReport (void) {
			

	
		switch(OutReport[0]){
			
			case 0x01 : ChangeConstParam (); break;
			case 0xd0 : disableAlarm(0,1); break;
			case 0xd1 : disableAlarm(1,1); break;
			case 0xd2 : disableAlarm(2,1); break;
		}
	
}




int main(){
	
	int i,j, retv;
  struct tm loc_time;
	
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
	while ((retv = finit (NULL)) != 0) ;
	
	led.setLed(PWR, LED_ON);
	
  
	


  RTC_init(0);
	
  	//dataFlash.saveParams();
  	dataFlash.readParams();
// 	

// 	
// 	
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
	
	//led7.setNumLed7(data.threshold);	//1
	//led7.setNumLed7(); 		//2
  //led7.setNumLed7(data.temperature[2]);					//3
	//led7.setNumLed7(data.temperature[3]);        			 //4
  
	//led7.setNumLed7(data.threshold);
		
	}
	
	


  

 	return 0;
 }
	


