#include <lpc23xx.h>
#include "timer.h"



struct typeTimer timer = {&initTimer0, &startTimer0, &initWDTimer, &startWDTimer};


void initTimer0(){
	
	PCONP |= 0x2; //включение
	T0TCR = 0x00000002;
	T0MCR = 0x3;  // при совпадение сброс таймера и перрывание
	T0PR = 0; // перделитель 
  T0MR0 = 300000;  // совпадение 
	
	
}

void startTimer0(int n){
	
	if(n){
		//Запустить таймер
		T0TC = 0x00000000;
    T0TCR = 0x00000001;
		
	}
	else{
		
		T0TCR = 0x00;
	}

}

void initWDTimer(){
	
	WDCLKSEL |= (0x01);
	WDMOD = (0x01);
	WDTV = (0x00);
	WDTC = (0x2000000);  //~11sec
	
	
}


void startWDTimer(){
	
	WDFEED = 0xAA;
	WDFEED = 0x55;
	
}

