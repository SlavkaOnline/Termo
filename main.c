
#include <lpc23xx.h>

#include "includes.h"



#include <ctype.h>
#include <math.h>


int main(){

	int retv;

    /* ������������� */
	while ((retv = finit (NULL)) != 0) ; //���� �����
	USB_Init();             //USB
    USB_Connect(__TRUE);

	defaultData(); // ��������� ����������� ��������
	initGPIO();     // ������������� ������
	setMode(0);     //��������� 1�� ������
	initVIC();      // ������������� ����������

    /*��������� �������� */
	initSoftTimers();
	timer.initTimer0();
	timer.startTimer0(1);
	timer.initWDTimer();
    timer.startWDTimer();

    /*��������� ������ ����� ��������� �������*/
    RTC_init(0);
    RTC_int();

    dataFlash.readParams();
  //dataFlash.SumWorkTime(SETNULLHOUR);


	while(1){

	}

 	return 0;
 }



