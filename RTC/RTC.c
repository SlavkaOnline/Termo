/*----------------------------------------------------------------------------
 *      PSTU Keil Libs
 *----------------------------------------------------------------------------
 *      Name:    RTC.C
 *      Purpose: Purpose: Driver for Philips LPC23xx Real-time clock
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      Author:	 			A. Sharonov
 *      Created:			09.07.2012
 *			Last modifed:	23.04.2014
 *---------------------------------------------------------------------------*/

#include "LPC23xx.h"
#include <time.h>
#include "RTC.h"
#include "includes.h"


const unsigned long EPOCH = 2208988800UL;				// 1970-1900 in seconds
unsigned char RTC_status=0;
unsigned short delay=0;

#ifdef __MENU_H
extern unsigned short ustavki[];
#endif?

void RTC_Handler (void)	__irq;

void RTC_init (unsigned long Pclk)
{
 	//Подпрограмма иницtckb иализации часов реального времени
	RTC_AMR=0xFF;								//Очистить маску будильника
	if (!(Pclk))
		RTC_CCR=0x10;							//Отключить часы, настроить на тактирование от внешнего источника
	else
		RTC_CCR=0x00;							//Отключить часы, настроить на тактирование от внутреннего источника
	RTC_CIIR=0;									//Ни одно изменение не генерирует прерывания
	if (Pclk)
	{
		RTC_PREINT=(int)(Pclk/32768)-1;				//Вводим целую часть предделителя
		RTC_PREFRAC=Pclk-((RTC_PREINT+1)*32768);	//Вводим дробную часть предделителя
	}
}

void RTC_int (void)
{
 	//Подпрограмма настройки прерывания от часов рального времени
	RTC_CIIR =0x04;								//Разрешить генерацию прерываний через 1hour
	RTC_CCR  |=0x01;								//Разрешить работу часов
	RTC_CISS =0;
	VICVectAddr13=(unsigned long) RTC_Handler;	//Прерывание от часов - вектор 13
	VICVectPriority13=4;						//Приоритет прерывания 4
	VICIntEnable |=1<<13;						//Разрешение прерывания от часов
	//RTC_ILR	 =0x07;
}

unsigned int get_time (void)
{
	//Function for get time from RTC
	struct tm loc_time;
	
	loc_time.tm_sec		=	RTC_SEC;
	loc_time.tm_min		=	RTC_MIN;
	loc_time.tm_hour	=	RTC_HOUR;
	loc_time.tm_mday	=	RTC_DOM;
	loc_time.tm_mon		=	RTC_MONTH;
	loc_time.tm_year	=	RTC_YEAR;
	return (mktime(&loc_time)+EPOCH);	
}

void correct_time (time_t new_time)
{
	//Function for set time in RTC
	struct tm loc_time;
	
	new_time=new_time-EPOCH;
	loc_time=* localtime(&new_time);
	
	RTC_SEC		=loc_time.tm_sec;
	RTC_MIN		=loc_time.tm_min;
	RTC_HOUR	=loc_time.tm_hour;
	RTC_DOM		=loc_time.tm_mday;
	RTC_MONTH	=loc_time.tm_mon;
	RTC_YEAR	=loc_time.tm_year;
}

unsigned char RTC_get_status (void)
{
	unsigned char temp;
	
	temp=RTC_status;
	RTC_status=0;
	return temp;
}

void get_time_struct (struct tm* loc_time)
{
// 	loc_time->tm_year=	RTC_YEAR;
// 	loc_time->tm_mon=		RTC_MONTH;
// 	loc_time->tm_mday=	RTC_DOM;
// 	loc_time->tm_hour=	RTC_HOUR;
// 	loc_time->tm_min=		RTC_MIN;
// 	loc_time->tm_sec=		RTC_SEC;
}

void correct_time_struct (struct tm* time)
{
 	RTC_SEC		=time->tm_sec;
 	RTC_MIN		=time->tm_min;
 	RTC_HOUR	=time->tm_hour;
}

void correct_data_struct (struct tm* time)
{
	RTC_DOM		=time->tm_mday;
	RTC_MONTH	=time->tm_mon;
	RTC_YEAR	=time->tm_year;
}

void set_allarm (struct tm* local_time)
{
// 	RTC_ALSEC		=local_time->tm_sec;
// 	RTC_ALMIN		=local_time->tm_min;
// 	RTC_ALHOUR	=local_time->tm_hour;
// 	RTC_AMR			=0xF8;
}

void inc_allarm (void)
{
	if (RTC_ALHOUR==23)
	{
		RTC_ALHOUR=0;
	}
	else
	{
		RTC_ALHOUR++;
	}
}

void dec_allarm (void)
{
	if (RTC_ALHOUR==0)
	{
		RTC_ALHOUR=23;
	}
	else
	{
		RTC_ALHOUR--;
	}
}

void set_delay (unsigned delay_time)
{
	delay=delay_time;
}

unsigned short check_delay (void)
{
	return delay;
}

void delay_clear (void)
{
	delay=0;
}

void RTC_Handler (void)	__irq 
{
	if (RTC_ILR &0x01)
	{
		//led7.setNumLed7(RTC_MIN);
		//dataFlash.SumWorkTime(UPDATEHOUR);
		RTC_status |=0x01;
		RTC_ILR	= 0x01;						//Очищаем флаг прерывания
	}
	if (RTC_ILR &0x02)
	{
#ifdef __MENU_H	
		if (ustavki[Mode_ustr]==0x01)
#endif
			RTC_status |=0x02;
		RTC_ILR	= 0x02;						//Очищаем флаг прерывания
	}
	else
		RTC_ILR = 0x04;
	VICVectAddr=0;
}
