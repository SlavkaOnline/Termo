#ifndef MBSR_H
#define MBSR_H

#include "mbsp.h"

#define UART_7_BIT_DATA							0x2
#define UART_8_BIT_DATA							0x3
#define	UART_1_STOP_BIT							0x0
#define	UART_2_STOP_BIT							0x4
#define	FLOW_CONTROL_NON						0x0
#define	FLOW_CONTROL_ODD						0x8
#define	FLOW_CONTROL_EVEN_PARITY		0x18
#define	FLOW_CONTROL_FORCED_0				0x28
#define	FLOW_CONTROL_FORCED_1				0x38

#ifdef MBSR_local

#define ERROR_DATA_ADDRES	2
#define ERROR_FUNK			1




unsigned short** MasData;
int MasDataLen;
unsigned short *ConstData;	//Массив флагов защиты записи переменных

unsigned char *MBSTxDataBuf;

unsigned char SlaveAddres = 0x30;

unsigned int MBSBndRate;
unsigned char	MBSDataBit;
unsigned char MBSStopBit;
unsigned char MBSFlowControl;
unsigned short MBStmp;

extern unsigned short izmer[32];
extern unsigned short ustavki[96];

#endif

void MBSRTUWork(MBSMail* ptr);
void MBSWork(unsigned char* data,int len,unsigned char* retdata,int *lenretdata);
void SetSlaveAddres(unsigned char addr);
void MBSTransmitError(unsigned char* data,int len,unsigned char error);
void MBSInit(int NumUart,unsigned int bnd_rate,unsigned char data_bit,unsigned char stop_bit,unsigned char flow_control,
				unsigned char* txd,unsigned char* rxd,
				unsigned short** mas,int len,unsigned short* constdat);
unsigned short CRC16(unsigned char *lMessage, unsigned int lCount);
void MBSReInit(int NumUart,unsigned int bnd_rate,unsigned char data_bit,unsigned char stop_bit,unsigned char flow_control,
				unsigned char* txd,unsigned char* rxd,
				unsigned short** mas,int len,unsigned short* constdat);
void MBSInitConstData(unsigned short** data,unsigned short* constdat);

#endif
