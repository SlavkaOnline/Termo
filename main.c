
#include <lpc23xx.h>

#include "includes.h"



#include <ctype.h>
#include <math.h>


int main(){

	int retv;

    /* Инициализация */
	while ((retv = finit (NULL)) != 0) ; //флеш карта
	USB_Init();             //USB
    USB_Connect(__TRUE);

	defaultData(); // установка стандартных значений
	initGPIO();     // инициализация портов
	setMode(0);     //установка 1го режима
	initVIC();      // инициализвция прерываний

    /*включение таймеров */
	initSoftTimers();
	timer.initTimer0();
	timer.startTimer0(1);
	timer.initWDTimer();
    timer.startWDTimer();

    /*включение модуля часов реального времени*/
    RTC_init(0);
    RTC_int();

    dataFlash.readParams();
  //dataFlash.SumWorkTime(SETNULLHOUR);


	while(1){

	}

 	return 0;
 }



