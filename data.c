#include "data.h"

int sumTemperature[3];

/**
	\file
	\brief Файл с описанием функций из data.h   
*/

struct typeData data; ///< Структура данных
struct typeData * pr_data = &data;

void defaultData(){
	
	int i;
	for(i = 0; i < 4; i++){
		data.temperature[i] = -100;
		data.chanelError[i] = 0;
		sumTemperature[i] = 0;
		if(i < 3) data.alarm[i] = 0;
	}
	initMode();
	data.currentMode = 0;
	data.threshold = data.mode[ data.currentMode ];
	data.THdelay = 1;
	data.work = 0;
	 
	data.UTH = _UTH;
  data.codeError = 0;
	
	data.Thyst = _Thyst;
	data.Tos = _Tos;
	
}

int getDec(int buf){

	
int temperature = 0;
int i;
int mask32 = 0x80000000;
int sign  = 1;

	
for ( i = 0; i < 11; i++){

   	if ( buf & mask32){
            if ( i == 0)
            {
                sign*= -1;
                buf <<= 1;
                continue;
            }
			temperature |= 0x01;
			temperature <<= 1;
		}
		else{
           if ( i == 0)
                {
                    buf <<= 1;
                    continue;
                }
			temperature <<= 1;
		}
       buf <<= 1;

}

temperature *= sign;

return temperature;

}

int getCold(int buf){

int temperature = 0;
int i;
int mask32 = 0x8000;
int sign  = 1;
	
for ( i = 0; i < 7; i++){

   	if ( buf & mask32){
            if ( i == 0)
            {
                sign*= -1;
                buf <<= 1;
                continue;
            }
			temperature |= 0x01;
			temperature <<= 1;
		}
		else{
           if ( i == 0)
                {
                    buf <<= 1;
                    continue;
                }
			temperature <<= 1;
		}
       buf <<= 1;

}

temperature *= sign;

return temperature;

}

void setTemperature(int chanel, int temperature){
		
	
data.temperature[chanel] = temperature;

	/* Установка 5го режима по температуре машинного зала */
if(chanel == 3){

data.mode[4] = data.temperature[3] + 30;

}	
	
}

void initMode(){
	
	int i;
	for(i = 0; i < 5; i ++){
	data.mode[i] = MODE[i];
	}
	
}

void setMode(int m){
	
	
	if( (m == 4) && (data.chanelError[3] != 0) ) //отказ 5го режима если ТМЗ вышел из строя 
		return ;
	
	data.currentMode = m;
	data.threshold = data.mode[ data.currentMode ];
		
}



void getAssayTemperature(){
	
 static int leds[] = {FIRE1r, FIRE2r, FIRE3r};
 int i;	
   
 for(i = 0; i < 3; i++){ 
	 
	if ( (data.chanelError[i] == 0) && (data.temperature[i] > data.threshold) && (data.alarm[i] == 0) ){
		   
			if(THdTimer[i].start == 1) continue; //???
		  startTHdTimer(i, data.THdelay);
		  if(THdTimer[i].stop == 1){
		      //led.setLed(leds[i], LED_ON);
				sendAlarm(i);
				THdTimer[i].stop = 0;
				THdTimer[i].start = 0;
				dataFlash.setAlarmDateTime();
			}				
	}

}
}
 


void getAssayChanel(){
	
	int sumError = 0;
	static int leds[] = {FIRE1y,FIRE2y,FIRE3y, MHF};
  static int noConnect7[] = {11, 21, 31, 41};
  static int blink7[] = {12, 22, 32, 42};
  static int blink[] = {3, 4, 5, 6};
	int i;
	
for(i = 0; i < 4; i++){
	switch (data.chanelError[i]){
		    
				case 0:
					
				   led.setLed(leds[i], LED_OFF);
				   ledsBlink[ blink[i] ] = 0;
				 break;		 
		    case 1:
					 data.codeError = noConnect7[i];
					 led7.setNumLed7( noConnect7[i] );
				   led.setLed(leds[i], LED_ON);
				   ledsBlink[ blink[i] ] = 0;
				   sendAlarm(alarm_FA);
				   if ( i != 3) sumError++;
				 break;
				 case 2:
					 data.codeError = blink7[i];
					 led7.setNumLed7( blink7[i] );
				   ledsBlink[ blink[i] ] = 1;
				   sendAlarm(alarm_FA);
				   if ( i != 3) sumError++;
				 break;
				 
				 case 3:
					 data.codeError = 51;
				   led7.setNumLed7( 51 );
				   ledsBlink[ blink[i] ] = 0;
				   sendAlarm(alarm_FA);
				   if ( i != 3) sumError++;
				 break;
				 
	 } 
 }
	/* проверка выхода из строя более чем двух каналов */	
   
	 if( sumError > 1){ 
		 data.work = 1;
		 ledsBlink[1] = 1;
		 
	 }
	 // Блок восстановления устройства после решения всех аппаратных проблем 
	 
	 else{ 
		 
		 data.work = 0;
		 ledsBlink[1] = 0;
		 led.setLed(RUN, LED_ON);
	 }
	 
	 
	 if ( (sumError == 0) && (data.chanelError[3] == 0) ){
		 led.setLed(RUN, LED_ON);
		 ledsBlink[1] = 0;
		 led7.resetLed7();
		 FIO1CLR |= (0x20000000); //сброс FA
		 data.codeError = 0;
	 }
	 
	 
	 //выход из 5го реджема если он отказал
	 if (( data.chanelError[3] != 0 ) && ( data.currentMode == 4 ) ){
       data.work = 1;
		   ledsBlink[1] = 1;
		   sendAlarm(alarm_FA);
		 //disableAlarm();
		 
	 }		 
	 
}


int testChanel(int chanel, int buf){
	
	//обрыв 
	if( buf & 0x01 ){
		data.chanelError[chanel] = 1;
		return 1;
	}
	//КЗ на землю 
	if( buf & 0x2 || buf & 0x4 ){
		data.chanelError[chanel] = 2;
		return 1;
	}
		
  //отказ микросхемы
	if( buf == 0)
	{
		data.chanelError[chanel] = 3;
		return 3;
	}
	
 	
 		if( testSC( chanel,  getDec(buf) ) ){
 		data.chanelError[chanel] = 2;
 		return 1;
 	}	
	
	//нет ошибок, но раньше она была
	if( data.chanelError[chanel] != 0 ){
		data.chanelError[chanel] = 0;
		return 2;
	}
	//нет ошибок
	data.chanelError[chanel] = 0;
		return 0;
}


void initGPIO(){
	
	SCS |= 1;
	
	led.initLed();
	led.resetLed();
	led7.initLed7();
	led7.resetLed7();
	SSP.initSSP1();

	SSP.initSPI();
	initI2C();
	
	//порты установки режима
	PINSEL6 &= ~(0xFFFF);
	PINMODE6 &= ~(0xFFFF);
	PINSEL7 &=  ~(0x3C000);
	PINMODE7 &=  ~(0x3C000);
	FIO3DIR &= ~(0x18000FF);

	
	//порты подачи тревоги 
	PINSEL3 &= ~(0x03F00000);
	PINMODE3 &= ~(0x03F00000);
	FIO1DIR |= (0x3C000000);
  FIO1CLR |= (0x3C000000);
	
	PINSEL4 &= ~(0xC000000);
	FIO2DIR |= (1<<13);
	FIO2SET |= (1<<13);
	
}

void sendAlarm(int chanel){
	
	if (chanel != alarm_FA)
		data.alarm[chanel] = 1;
	switch(chanel){
		case 0:
			FIO1SET = (1<<26);
		  led.setLed(FIRE1r, LED_ON);
		break;
		case 1:
			FIO1SET = (1<<27);
		  led.setLed(FIRE2r, LED_ON);
		break;
		case 2:
			FIO1SET = (1<<28);
		  led.setLed(FIRE3r, LED_ON);
		break;
		case 3:
			FIO1SET = (1<<29);
		break;
			
	}
	
}


void disableAlarm(int chanel, int mode){
	
	switch (chanel){
		
			case 4:
				if( data.temperature[0] < data.threshold && data.temperature[1] < data.threshold  && data.temperature[2] < data.threshold ){
				data.alarm[0] = 0;
				data.alarm[1] = 0;
				data.alarm[2] = 0;
				FIO1CLR |= (0x1C000000);
				led.setLed(FIRE1r, LED_OFF);
				led.setLed(FIRE2r, LED_OFF);
				led.setLed(FIRE3r, LED_OFF);
					
				}
			break;
		
		
			case 0:
				if( (data.temperature[0] < data.threshold )&& (mode == 1) ){
					data.alarm[0] = 0;
					FIO1CLR |= (1<<26);
					led.setLed(FIRE1r, LED_OFF);
				}
			break;
			
			case 1:
				if(  (data.temperature[1] < data.threshold )&& (mode == 1)){
					data.alarm[1] = 0;
					FIO1CLR |= (1<<27);
					led.setLed(FIRE2r, LED_OFF);
				}
			break;

			case 2:
				if(  (data.temperature[2] < data.threshold )&& (mode == 1)){
					data.alarm[2] = 0;
					FIO1CLR |= (1<<28);
					led.setLed(FIRE3r, LED_OFF);
				}
			break;					
	
	
		}
	
}

void upThreshold(){
	
	data.mode[ data.currentMode ] += data.UTH;
	data.threshold = data.mode[ data.currentMode ];
	
}

void forcedTestChanel(int chanel, int flagFTC){
	
	if (flagFTC && (data.chanelError[chanel] == 0) ){
		if ( chanel < 3 ){
			sendAlarm(chanel);
		}
	}
	else{
		disableAlarm(chanel, falseAlarm);
		
	}
}


int getDecTMZ(int buf){
	
	buf = buf >> 1;
	
	buf = ( (double)buf / 32) - 256;
	
	return buf;
}

int testTMZChanel(int buf){
	
	
	if(  ( buf & 0xFFFF )== 0xFFFF ){ // || ( (buf & 0x00000001 ) == 0x00000001) ){
		data.chanelError[3] = 1;
		return 1;
	}
	
	if(testBreakTMZ( getDecTMZ( buf ) ) ){
		  data.chanelError[3] = 1;
			return 1;
	}
	
 	if(data.chanelError[3] != 0){
 		data.chanelError[3] = 0;
 		configureMAX31865(0xD3);
		return 0;
 	}
	
	data.chanelError[3] = 0;
	return 0;
}


int testSC(int chanel, int temperature){
	
	if (data.temperature[chanel] == -100){
       return 0;
	}
	else{
	float tmp = ((data.temperature[chanel]) - (short)(temperature) )/ (float)(data.temperature[chanel]);
	
	if (tmp >= 0.3 ){
		
		return 1;
		
	}
}
	return 0;
}

int testBreakTMZ(int temperature){
	
		
	if (temperature >= MAXtmzTemperature)
			return 1;

	return 0;
	
}


void setConfigureDS1631(){
	
	dataDS1631.RW = 0;
	
	dataDS1631.BUFF_OUT[0] = (0x8C);
	dataDS1631.BUFF_OUT[1] = (data.Tos);
	dataDS1631.BUFF_OUT[2] = (0x00);
	dataDS1631.BUFF_OUT[3] = (data.Thyst);
	dataDS1631.BUFF_OUT[4] = (0x00); 
	//dataDS1631.size = 5;
	
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0xAC);
	dataDS1631.inc = 0;
	dataDS1631.size = 1;
	startI2C();
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	
	
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0xA1);
	dataDS1631.size = 3;
	startI2C();
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0xA2);
	dataDS1631.size = 5;
	startI2C();
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	


	
}

void getConfigureDS1631(){
	
	dataDS1631.RW = 0;
	
	
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0x51);
	dataDS1631.inc = 6;
	dataDS1631.size = 6;
	startI2C();
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	
	
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0x22);
	dataDS1631.inc = 6;
	dataDS1631.size = 6;
	startI2C();
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	//---------------------------------------//
	
	dataDS1631.RW = 1;
	
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0xAA);
	dataDS1631.inc = 0;
	dataDS1631.size = 2;
	startI2C();
	
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0xA1);
	//dataDS1631.inc = 0;
	dataDS1631.size = 4;
	startI2C();
	
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0xA2);
	//dataDS1631.inc = 0;
	dataDS1631.size = 6;
	startI2C();
	
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	dataDS1631.Address = (0x90);
	dataDS1631.REG = (0xAC);
	//dataDS1631.inc = 0;
	dataDS1631.size = 7;
	startI2C();
	
	while( (I21CONSET & 0x10) != 0x10);
	I21CONCLR = 0x10;
	

	
	
	
}

// int testLM75(){
// 	
// 	short i;
// 	short flag = 0;
// 	getConfigureLM75();

// 	
// 	for(i = 2; i < 7; i++){
// 		if(dataDS1631.BUFF_IN[i] != dataDS1631.BUFF_OUT[i-2] ){
// 			flag =  1;
// 			continue;
// 		}
// 	}
// 	
// 	if (flag){
// 		setConfigureLM75();
// 	}
// 	
// }
