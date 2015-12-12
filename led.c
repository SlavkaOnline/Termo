/**
	\file
	\brief Файл с описанием функций из led.h   
*/


#include "led.h"

struct typeLed led = {&resetLed, &initLed, &setLed};
int ledsBlink[] = {0,0,0,0,0,0,0,0,0,0,0};


void resetLed(){
	
	FIO0CLR |= 0x01;  
	FIO2CLR |= 0x5FF;
	
}


void initLed(){
	
	//PWR
	PINSEL0 &= ~0x3;
	PINMODE0 &= ~0x3;
    FIO0DIR |= 0x01;
	
	//остальные
	PINSEL4 &= ~0xFFFF;
	PINMODE4 &= 0xFFFF;
    FIO2DIR |= 0x5FF;
	
	
}

 void setLed(int n, int p){
	
	 switch(n){
		
		case 0 :
			if (p) FIO0SET |= 0x01; else FIO0CLR |= 0x01; 
		break;
		
		case 1 :
			if (p) FIO2SET |= 1<<10; else FIO2CLR |= 1<<10;
		break;
		
		case 2 :
			if (p) FIO2SET |= 1; else FIO2CLR |= 1;
		break;
			
		case 3 :
			if(p) FIO2SET |= 1<<4; else FIO2CLR |= 1<<4;
		break;	

		case 4 :
			if (p) FIO2SET |= 1<<6; else FIO2CLR |= 1<<6;
		break;
			
		case 5 :
			if(p) FIO2SET |= 1<<8; else FIO2CLR |= 1<<8;
		break;
			
		case 6 :
			if(p) FIO2SET |= 2; else FIO2CLR |= 2;
		break;
			
		case 7 :
			if(p) FIO2SET |= 1<<3; else FIO2CLR |= 1<<3;
		break;
			
		case 8 :
			if (p) FIO2SET |= 1<<5; else FIO2CLR |= 1<<5;
		break;
			
		case 9 :
			if(p) FIO2SET |= 1<<7; else FIO2CLR |= 1<<7;
		break;
			
		case 10 :
			if(p) FIO2SET |= 1<<2; else FIO2CLR |= 1<<2;
		break;
	
	}	
}