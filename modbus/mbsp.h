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

unsigned char *MBSTxData;//��������� �� ������������ ������
unsigned char MBSTxI = 0; //������ ������������ ����
unsigned char MBSTxLen = 0;//������ ������������ �������
unsigned char MBSNedTx = 0;

unsigned char *MBSRxData;//��������� �� ����������� ������
unsigned char MBSRxI = 0;//������ �������� ����

unsigned char MBSNumUart;//����� �����
unsigned char MBSRxDataComplit = 0;	//0 - ����� �� ������ 1 - ����� ������ �� �� ���������
//unsigned char MBSLineStatus = 0; // 0 - ���� �� ������ 1 - ����� ������ 2 - �������� ������ 3 - �������� ��������
unsigned char MBSLineInterval = 0; // 0 - ����� 1 - �� �����
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
