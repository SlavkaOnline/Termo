#include "softTimer.h"

struct t_readPortTimer readPortTimer[10];
struct t_blinkLedTimer blinkLedTimer; // = {10,10,0,&BlinkLed};
struct t_SSP1Timer ssp1Timer = {4,4,5,5,&recSSP1OnTimer};
struct t_SPITimer spiTimer = {4,4,5,5,&recSPIOnTimer};
struct t_THdTimer THdTimer[4];



void initSoftTimers(){
	int i; 

	static const int DI[] = {0x02, 0x01, 0x08, 0x04, 0x20, 0x10, 0x80, 0x40, 0x1000000, 0x800000}; 
	
  for(i = 0; i < 3; i++){
   THdTimer[i].start = 0;
	 THdTimer[i].stop = 0;
	}		
	
	for(i = 0; i < 10; i++){
		  readPortTimer[i].multiplier = 4;
		  readPortTimer[i]._multiplier = 4;
		  readPortTimer[i].delay = 25;
		  readPortTimer[i]._delay = 25;
		  readPortTimer[i].pin = DI[i];
		  readPortTimer[i].mode = i;
			readPortTimer[i].hold = 0;
		  readPortTimer[i].TimerFunc = &portCommand;
		  
	}
	
	blinkLedTimer.multiplier = 10;
	blinkLedTimer._multiplier = 10;
	blinkLedTimer.TimerFunc = &BlinkLed;
	for(i = 0; i < 11; i++){
   blinkLedTimer.flag[i] = 0;
	}	
	
}

void startTHdTimer(int n, int sec){
	
// 	int i;
// 	for(i = 0; i < 3; i++){
// 		if(i == n ){
			THdTimer[n].multiplier = sec * 40 ;
			THdTimer[n].start = 1;
// 		}
// 		
// 	}
	
}



void portCommand(struct t_readPortTimer* timer){

		
	
if ( !(FIO3PIN & timer->pin ) ){	//	
	  
	
		if (timer->delay-- == 0)  // 100ms * 30 = 3000ms = 3s
		{
		  led7.setNumLed7(timer->mode); //удалиь 
			timer->delay = timer->_delay;
			if( timer->mode < 5){  
			    setMode(timer->mode);
			}
			else if ( timer->mode < 8){ 
				  
				timer->hold = 1;
				forcedTestChanel(timer->mode - 5, timer->hold); 
			} 
			else if (timer->mode == 8) disableAlarm(alarm_All, notFalseAlarm);
			  else if (timer->mode == 9) {upThreshold(); disableAlarm(alarm_All, notFalseAlarm ); }
		}
		
		
		
	}
	else{
		timer->delay = timer->_delay;
			if(timer->hold == 1){
				if( 4 < timer->mode && timer->mode < 8  ){
					timer->hold = 0;
					forcedTestChanel(timer->mode - 5, timer->hold);
				}
		}
	}
	
	
	
}

void BlinkLed(struct t_blinkLedTimer* timer){
	
 int i;
 for(i = 0; i < 11; i++){
   if(ledsBlink[i] == 1){
		 if(timer->flag[i] == 1){
			 led.setLed(i, LED_ON);
			 timer->flag[i] = 0;
		 }
		 else {
			 led.setLed(i, LED_OFF);
			 timer->flag[i] = 1;
		 }		 
	 }
	 
 }	 
	
}


void recSSP1OnTimer(struct t_SSP1Timer* timer){
  
	int i;
	int num;	
	int delay = 3000000;
	
 	  
	
	if( timer->time-- !=0 ){
	 
		/* Термопары */
		for(i = 0; i < 3; i++){
   		
		 SSP.setSS1(i,SSP_ON); 
		 num = SSP.recSSP1();
		 SSP.setSS1(i,SSP_OFF);
		
		 switch (testChanel(i , num) ){
       		case 2:
						while(delay--);
						delay = 3000000;
					  data.sumTemperature[i] = 0;
					break;
					case 0:
							setTemperature(i, getDec(num));
					    
					//data.sumTemperature[i] +=  getDec( num );
					break;
					
					default: 
						data.sumTemperature[i] = 0;
					break;				
	 }		 
		}
		
		 
	}
	 
	 else{
		 
     /* Термопары */
// 		 for(i = 0; i < 3; i++){
// 		 num = data.sumTemperature[i] / timer->_time;
//      setTemperature(i, num);
// 		 getAssayTemperature();
// 		 data.sumTemperature[i] = 0;	 
// 		 } 
     	timer->time = timer->_time;		 
	 }
		
	  getAssayChanel();
	  
// 	 if(data.work)
//  				return;
	 
	 getAssayTemperature();
}


void recSPIOnTimer(struct t_SPITimer* timer){
 
	int num;	
 	//  if(data.work)
 		//		return;
	
	if( timer->time-- !=0 ){
	
		
		/* Платиновый датчик */
// 		configureMAX31865(0xD3);
// 		
		
		
		SSP.setSPI(SSP_ON);
		num = SSP.recSPI();
		SSP.setSPI(SSP_OFF);
		
		if( !(testTMZChanel(num) ) ){
			data.temperature[3] = getDecTMZ( num );	
      			
		}
		else{
				data.sumTemperature[3] = 0;
			}
	
  }	 
	else{
		
 		  //data.sumTemperature[3] /= timer->_time;
     	 
		 
     timer->time = timer->_time;		 
	 }
	  getAssayChanel();
}