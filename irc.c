#include "irc.h"


void Timer0_Int (void) __irq
{
int i; 
T0IR = 0x00000001; /*Сбросить флаг прерывания в Timer0*/

for(i = 0; i < 10; i++){	
	if ( readPortTimer[i].multiplier-- == 0)
	{
		readPortTimer[i].multiplier = readPortTimer[i]._multiplier; // 25ms * 4 = 100ms
		readPortTimer[i].TimerFunc(&readPortTimer[i]);
		
	}
}
  if ( blinkLedTimer.multiplier-- == 0){
		  blinkLedTimer.multiplier = blinkLedTimer._multiplier; 
		  blinkLedTimer.TimerFunc(&blinkLedTimer);
	}
	
	/*Отключаем считывание с каналов если вышло из строя более 2х каналов */
	//if ( !data.work ){
			if( ssp1Timer.multiplier-- ==0){ 
				ssp1Timer.multiplier = ssp1Timer._multiplier;
				ssp1Timer.TimerFunc(&ssp1Timer);
			}
			
			if( spiTimer.multiplier-- ==0){ 
				spiTimer.multiplier = spiTimer._multiplier;
				spiTimer.TimerFunc(&spiTimer);
			}
			
			for(i = 0; i < 3; i++){
				if(THdTimer[i].start == 1){
					if(THdTimer[i].multiplier-- == 0)
					{
						THdTimer[i].start = 0;
						THdTimer[i].stop = 1;
					}
				}
				
			}
//}


	
 timer.startWDTimer();
 VICVectAddr = 0; /*Перевести VIC в исходное состояние*/
}

void WDTimer_Int (void) __irq{
	
// 	int i;
// 	for(i = 0; i < 100000; i++)
// 	led.setLed(RUN, LED_ON);
// 	for(i = 0; i < 100000; i++)
// 	led.setLed(RUN, LED_OFF);
	
	led.setLed(HFAULT, LED_ON);
	
	sendAlarm(alarm_FA);
	
	VICVectAddr = 0; /*Перевести VIC в исходное состояние*/
}

void I2C_Int() __irq {
	
	switch(I21STAT){
		
		case (0x08):
			I21CONCLR = (0x20);
		  I21DAT = dataDS1631.Address;
		break;
		
		case (0x10):
  	I21CONCLR = (0x20); 
		I21DAT = (dataDS1631.Address);		
		break;
		
		case (0x18):
		I21DAT = (dataDS1631.REG);
    	
		break;
		
		case (0x20):
			
		break;
		
		case (0x28):
			if(dataDS1631.RW){
				  dataDS1631.Address |= 0x01;
					I21CONSET = (0x20);	
			}  
     else	{		
      if (dataDS1631.inc != dataDS1631.size){
				I21DAT = dataDS1631.BUFF_OUT[dataDS1631.inc++];  
			}	
			 else{
				 I21CONSET = 0x10;
			 }
		}			
		break;
		
		case (0x40):
		I21CONSET = 0x04;	
		break;
		
		case (0x48):
		I21CONSET = (0x20);	
		break;
	
		case (0x50):	
		  if(	dataDS1631.inc == dataDS1631.size){
        I21STAT = 0x58; 
        I21CONSET = 0x10;				
			}
			else{
				dataDS1631.BUFF_IN[dataDS1631.inc++] = I21DAT;
			}	
		break;
	
		case (0x58):
			I21CONSET = 0x10;
		break;
		
	}

I21CONCLR = (0x08);
VICVectAddr = 0;
}




void initVIC(){
	
	 
	 //Записать адрес обработчика прерывания в таблицу векторов
   VICVectAddr19 = (unsigned)I2C_Int;
	 VICVectAddr4 = (unsigned)Timer0_Int;
	 VICVectAddr0 = (unsigned)WDTimer_Int;
	
	 VICIntEnable |= 0x00080011;
	 
}

