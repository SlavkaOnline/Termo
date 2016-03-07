#include <lpc23xx.h>
#include "SSP.h"



struct typeSSP SSP = {&initSPI, &recSPI, &setSPI,&initSSP1, &recSSP1, &setSS1};


static void initSSP1(){
	
	int i;
	
	PCONP |= (1 << 10); // активация SSP1
	
	PINSEL0 &= ~(0x3C000); // p0.7 | p0.8 clear
	PINSEL0 |= (0x28000);  // p0.7 | p0.8 set
	
	
	//p0.1 SS3 p0.10 SS1  p0.11 SS2 
	PINSEL0 &= ~(0xF0000C);
	PINMODE0 &= ~(0xF0000C);
	FIO0DIR |= (0xC02);
	FIO0SET |= (0xC02);
	
	//SSP1 
	SSP1CR0 = (0x00CF); // 16bits CPOL = 0; CPHA = 1
	SSP1CPSR = (0x04);
	SSP1CR1 =(0x0002);  // SSE = 1, MS = 0, SOD = 0, LBM = 0;
	
	
  FIO0SET |= (0xC02);  //отключить все SSn
		
}



static int  recSSP1(){

	int num = 0;

	 while (SSP1SR & RNE);
		SSP1DR = 0xFFFF;
	  
	 while ( (SSP1SR & (BSY |RNE)) != RNE );

	 while ( !(SSP1SR & RNE) );
		num = SSP1DR;

	  num=num << 16;
	 
	while (SSP1SR & RNE);
		SSP1DR = 0xFFFF;
	  
	 while ( (SSP1SR & (BSY |RNE)) != RNE );

	 while ( !(SSP1SR & RNE) );
		num |= SSP1DR;
	
	return num;
}

static void setSS1(int s, int n){
	
	
	switch (s){
	 case 0: 
			if( n ){
				FIO0CLR |= (1<<10);
			}
			else{
				FIO0SET |= (1<<10);
			}
		break;
			
		case 1:
			if( n ){
				FIO0CLR |= (1<<11);
			}
			else{
				FIO0SET |= (1<<11);
			}	
		break;
		
		case 2:
			if( n ){
				FIO0CLR |= (1<<1);
			}
			else{
				FIO0SET |= (1<<1);
			}
		break;
}
}


static void initSPI(){
	
	PCONP |= (1 << 8);
	
	//конфигурация пинов  15 - SCK 16 - GPIO 17 - MISO 18 - MOSI
  PINSEL0 &= ~(0xD0000000);
	PINSEL0 |=  (0xD0000000);
	PINMODE0 &=  ~(0xD0000000);
	
	PINSEL1 &= ~(0x3F);
	PINMODE1 &= ~(0x3F);
	PINSEL1 |=  (0x3C);
	FIO0DIR |= (1<<16);
	

	   S0SPCCR = 8;
	   S0SPCR = (0x0828);
	   
  	SSP.setSPI(SSP_OFF);
 	
 	 configureMAX31865(0xC3);
}


static void setSPI(int n){
	
	if( n ){
				FIO0CLR |= (1<<16);
			}
			else{
 				FIO0SET |= (1<<16);
			}
}

static int recSPI(){
	int tmp;
	int RTD;
	int regFault; 
  
	S0SPDR;
	S0SPDR = 0x01;
  while ( !(S0SPSR & 0x80));
		tmp = S0SPDR;
	  
	
	S0SPDR = 0x00;
  while ( !(S0SPSR & 0x80));
		RTD = S0SPDR << 8;	

	S0SPDR = 0x00;
  while ( !(S0SPSR & 0x80));
		RTD |= S0SPDR;
		
 
 
	S0SPDR = 0x00;
  while ( !(S0SPSR & 0x80));
	regFault = S0SPDR;
	
	S0SPDR = 0x00;
  while ( !(S0SPSR & 0x80));
	regFault = S0SPDR;
	
	S0SPDR = 0x00;
  while ( !(S0SPSR & 0x80));
	regFault = S0SPDR;
	
	S0SPDR = 0x00;
  while ( !(S0SPSR & 0x80));
	regFault = S0SPDR;
	
	S0SPDR = 0x00;
  while ( !(S0SPSR & 0x80));
	regFault = S0SPDR;
	
	
	
	if (regFault != 0){
		RTD = 0xFFFF;
	}
	
	return RTD ;
}


void configureMAX31865(short conf){
	
	//short conf = 0xC3;
	short trash;

	setSPI(SSP_ON);
	
	
	S0SPDR = 0x80;
 	while ( !(S0SPSR & 0x80) );
  trash = S0SPDR;
	
	S0SPDR = conf;
 	while (!(S0SPSR & 0x80) );
  trash = S0SPDR;
		
	setSPI(SSP_OFF);
}