#include <lpc23xx.h>

#include "includes.h"



#include <ctype.h>
#include <math.h>

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"

U8  InReport[63];                                /* HID Input Report    */
                                            /*   Bit0   : Buttons  */
                                            /*   Bit1..7: Reserved */
U8 OutReport[63]; 

void GetInReport (void) {
	
	
	U8* message = (U8*)&data;
	data.codeError = 65;
	memcpy(&InReport, message, sizeof(data));
	
}


/*------------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *------------------------------------------------------------------------------*/
void SetOutReport (void) {

	
	data = *((struct typeData *)&OutReport);
	led7.setNumLed7(data.codeError);

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
	
  
	//data.threshold = 35;
	

	
//  	dataFlash.saveParams();
//  	dataFlash.readParams();
// 	

	
	while(1){
	
	//led7.setNumLed7(data.temperature[0]);	//1
	//led7.setNumLed7(); 		//2
  //led7.setNumLed7(data.temperature[2]);					//3
	//led7.setNumLed7(data.temperature[3]);        			 //4
  
		
	}
	
	


  

 	return 0;
 }
	

