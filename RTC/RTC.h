/*----------------------------------------------------------------------------
 *      PSTU Keil Libs
 *----------------------------------------------------------------------------
 *      Name:    RTC.H
 *      Purpose: Driver for Philips LPC23xx Real-time clock
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      Author:	 			A. Sharonov
 *      Created:			09.07.2012
 *			Last modifed:	04.04.2012
 *---------------------------------------------------------------------------*/

#include <time.h>
#define RTC_EN RTC_CCR |=0x01;

void RTC_init (unsigned long Pclk);
void RTC_int (void);

unsigned int get_time (void);
void correct_time (time_t new_time);

unsigned char RTC_get_status 	(void);
void get_time_struct (struct tm* time);
void correct_time_struct 			(struct tm* time);
void correct_data_struct 			(struct tm* time);

void set_allarm 							(struct tm* local_time);
void inc_allarm 							(void);
void dec_allarm 							(void);

void set_delay(unsigned delay_time);
unsigned short check_delay (void);
void delay_clear (void);

extern const unsigned long EPOCH;	