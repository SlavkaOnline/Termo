
/**
	\file
	\brief Файл с описанием функций из led7.h   
*/
 
#include "led7.h"

struct typeLed7 led7 = {&initLed7, &resetLed7, &setNumLed7};

void initLed7(){
	
	PINSEL8 &= ~(0xFFFFF000);
	PINMODE8 &= ~(0xFFFFF000);
	PINSEL9 &= ~(0xFF0F0000);
	PINMODE9 &= ~(0xFF0F0000);

	FIO4DIR |= 0xF300FFC0 ; 
	
}

void resetLed7(){
	
	FIO4SET |= 0xF300FFC0 ;
	
}

void setNumLed7(int num){

	int segNum = 0x00;
	int n = num / 10;
	int m = num % 10;
	
	if(num / 100 != 0)
		return ;
	
	switch (n){
		
		case 0 : 
			segNum |= A1 | B1 | C1 | D1 | E1 | F1 ;
		break;
		
		case 1 : 
			segNum |= B1 | C1  ;
		break;
		
		case 2 : 
			segNum |= A1 | B1 | G1 | E1 | D1 ;
		break;
		
		case 3 : 
			segNum |= A1 | B1 | C1 | D1 | G1 ;
		break;
		
		case 4 : 
			segNum |=  B1 | C1 | F1 | G1 ;
		break;
		
		case 5 : 
			segNum |= A1 | C1 | D1 | F1 | G1 ;
		break;
		
		case 6 : 
			segNum |= A1 | C1 | D1 | E1 | F1 | G1 ;
		break;
		
		case 7 : 
			segNum |= A1 | B1 | C1 ;
		break;
		
		case 8 : 
			segNum |= A1 | B1 | C1 | D1 | E1 | F1 | G1 ;
		break;
		
		case 9 : 
			segNum |= A1 | B1 | C1 | D1 | F1 | G1 ;
		break;
			
	}
	
	switch (m){
		
		case 0 : 
			segNum |= A2 | B2 | C2 | D2 | E2 | F2 ;
		break;
		
		case 1 : 
			segNum |= B2 | C2  ;
		break;
		
		case 2 : 
			segNum |= A2 | B2 | G2 | E2 | D2 ;
		break;
		
		case 3 : 
			segNum |= A2 | B2 | C2 | D2 | G2 ;
		break;
		
		case 4 : 
			segNum |=  B2 | C2 | F2 | G2 ;
		break;
		
		case 5 : 
			segNum |= A2 | C2 | D2 | F2 | G2 ;
		break;
		
		case 6 : 
			segNum |= A2 | C2 | D2 | E2 | F2 | G2 ;
		break;
		
		case 7 : 
			segNum |= A2 | B2 | C2 ;
		break;
		
		case 8 : 
			segNum |= A2 | B2 | C2 | D2 | E2 | F2 | G2 ;
		break;
		
		case 9 : 
			segNum |= A2 | B2 | C2 | D2 | F2 | G2 ;
		break;
			
	}
	resetLed7();
	FIO4CLR |= segNum ;
	
}