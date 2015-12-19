#ifndef MBSP_H
#define MBSP_H

#include <LPC23xx.h>
#include <RTL.h> 

#ifdef MBSP_local

volatile unsigned long* MBSRBR;
volatile unsigned long* MBSTHR;
volatile unsigned long* MBSIER;
volatile unsigned long* MBSIIR;
volatile unsigned long* MBSLSR;

unsigned char *MBSTxData;//Указатель на передаваемые данные
unsigned char MBSTxI = 0; //Щетчик отправленных байт
unsigned char MBSTxLen = 0;//Длинна отпровляемой посылки
unsigned char MBSNedTx = 0;

unsigned char *MBSRxData;//Указатель на принимаемые данные
unsigned char MBSRxI = 0;//Щетчик принятых байт

unsigned char MBSNumUart;//Номер уарта
unsigned char MBSRxDataComplit = 0;	//0 - пакет не принят 1 - пакет принят но не обработан
//unsigned char MBSLineStatus = 0; // 0 - нече не делаем 1 - прием данных 2 - передача данных 3 - защитный интервал
unsigned char MBSLineInterval = 0; // 0 - истек 1 - не истек
unsigned char MBSError = 0;
unsigned char MBSInterv;

unsigned char MBSCost;

unsigned int	MBS_1_5;
unsigned int 	MBS_3_5;

extern OS_MBX MBSBox;
extern OS_MBX mpool;

#endif

#define RS485_DIR1		0x0040
#define RS485_DIR2		0x0100

typedef struct
{
	unsigned char *data;
	unsigned char len;
	unsigned char error;	
}MBSMail;

void MBSInitBuf(unsigned char *RxBuf);
void MBSUartInit(int NumUart,unsigned int bnd_rate,unsigned char data_bit,unsigned char stop_bit,unsigned char flow_control);
void MBSTimerInit(unsigned int bnd_rate);
void MBSUartHandler(void) __irq;
void MBSTimerHandler(void) __irq;
void MBSTransmitData(unsigned char *data,int len);
void MBSDeInitUart(int NumUart);
void MBSResetComplit(void);
int MBSGetComplit(void);
int MBSGetNumUart(void);
unsigned char* MBSGetData(void);
int MBSGetLen(void);



#endif
