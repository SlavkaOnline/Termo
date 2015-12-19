#define MBSP_local 2;
#include "mbsp.h"
/*
	��������� ��� ����� ���� �� ����� ��������� ������� ����� 
	��� ����� � �������� ����������?
	����� �� �� ��������?
	stop_bit  0 - 1 ���� ���; 1 - 2 ���� ����;
	flow_control
		000 - ��������
		001 - Odd parity. Number of 1s in the transmitted character and the attached parity bit will be odd.
		011 - Even Parity. Number of 1s in the transmitted character and the attached parity bit will be even.
		101 - Forced "1" stick parity.
		111 - Forced "0" stick parity.
*/
void MBSUartInit(int NumUart,unsigned int bnd_rate,unsigned char data_bit,unsigned char stop_bit,unsigned char flow_control)
{
	MBSNumUart = NumUart;
	if(NumUart == 0)
	{
		FIO0DIR |= RS485_DIR1;
		FIO0CLR	= RS485_DIR1;
		MBSRBR = ((volatile unsigned long *)(UART0_BASE_ADDR + 0x00));
		MBSTHR = ((volatile unsigned long *)(UART0_BASE_ADDR + 0x00));
		MBSIER = ((volatile unsigned long *)(UART0_BASE_ADDR + 0x04));
		MBSIIR = ((volatile unsigned long *)(UART0_BASE_ADDR + 0x08));
		MBSLSR = ((volatile unsigned long *)(UART0_BASE_ADDR + 0x14));

	/*	PCONP |= (1<<3);//�������� ������� �����
		//PCLKSEL |= (1<<PCLK_UART0);//��������� ������������
		PINSEL0 |= 0x50;//���������� � ������ ����� ������
		U0LCR = 0x83;//8-��� ������ 1 ���� ���
		U0DLM = 0x00;//��������� �������� �����
		U0DLL = (12000000 * 5) / (bnd_rate * 16 * 4);
		U0LCR = stop_bit | data_bit | flow_control;//������ ��������� �������� �����	   8 bit data
		
		U0LCR &= ~0x38;
		if(flow_control != 0 )
		{
			U0LCR |= (flow_control<<3);
		}*/
		//U0FCR = 0;//��������� FIFO
		PCONP |= (1<<3);//�������� ������� �����
		PINMODE0	|= 0x00000080;
		PINSEL0 	|= 0x00000050;
		U0LCR = 0x83;
		U0DLM = 0x00;	//������������ �������� ������������������ �����: ��. ����
		U0DLL   =   (12000000 * 5) / (bnd_rate * 16 * 4);
		U0LCR = 0x03; // DLAB = 0	��������� ������ � ��������
		U0FCR = 0x07; // ������� ���������� ������� FIFO - ��������� �  ����� TX � RX FIFO.
		U0IER = 0x01;//���������� ����������
		//U0TER = 0x80;//���������� ��������???
	
		VICVectAddr6 = (unsigned long)MBSUartHandler; //������ ����� ����������� ����������
		//VICVectCntl6 =	0x05;//��������� ����������
		VICIntEnable |= (1<<6);//���������� ����������
	}
	else
	{
		FIO0DIR |=RS485_DIR2;
		FIO0CLR	=RS485_DIR2;

		MBSRBR = ((volatile unsigned long *)(UART2_BASE_ADDR + 0x00));
		MBSTHR = ((volatile unsigned long *)(UART2_BASE_ADDR + 0x00));
		MBSIER = ((volatile unsigned long *)(UART2_BASE_ADDR + 0x04));
		MBSIIR = ((volatile unsigned long *)(UART2_BASE_ADDR + 0x08));
		MBSLSR = ((volatile unsigned long *)(UART2_BASE_ADDR + 0x14));

		//PCONP |= (1<<4);//�������� ������� �����
		PCONP |= ( 1 << 24 );
		PINSEL0 |= 0x00500000;
  		U2LCR = 0x83;//8-��� ������ 1 ���� ���
		U2DLM = 0x00;//��������� �������� �����
		U2DLL = (12000000 * 5) / (bnd_rate * 16 * 4);
		U2LCR = stop_bit | data_bit | flow_control;//������ ��������� �������� �����
		/*U2LCR &= ~0x38;
		if(flow_control != 0 )
		{
			U2LCR |= (flow_control<<3);
		}*/
		U2FCR = 0x00;//��������� FIFO
		U2IER = 0x01;//���������� ����������
	 	
		VICVectAddr28 = (unsigned long)MBSUartHandler;    // UART2 Interrupt -> Vector 28
    	VICVectCntl28 = 0x05;                              // UART2 Interrupt -> Priority 2
    	VICIntEnable = 1 << 28;
	}
}

void MBSTimerInit(unsigned int bnd_rate)
{
	PCONP |= (1<<2);//�������� ������� �������
	PCLKSEL0 |= (1<<4) | (0<<5); 	//������� �����������
	T1TCR = (1<<0);//�������� ������ ��������
	//T2MCR = (1<<0)|(1<<3); //��������� ���������� �� ����������	 6 9 
	T1CCR = (0<<0);//������� �������� �� ����������� ������������
	//T0PR = 0xffffffff;// ������������ �������� �����
	//T0PC = //������������ ��������
	//if((sb != 1)||(sb != 0)) sb = 0;
	//if(fc != 0) fc = 1;
	if(bnd_rate < 19200)//������������� ���������
	{
		//T2MR0 = 18752;//1,5 �������� =(57600000*(1+8+1+ sb + fc)*1.5)/t	1 - ����� ��� 8 - ��� ���� 1 - ���� ���	6 ���
		//T2MR1 =	43752;//3,5 �������	 =(57600000*(1+8+1+ sb + fc)*3.5)/t	 14 - ��� ������ 
		MBS_1_5 = 108000;//18752*5;
		MBS_3_5 = 252000;//43752*5;
	}
	else
	{
		//T2MR0 = 8400;//1,5 �������� 1,75mc
		//T2MR1 =	21015;//3,5 �������	 0,7mc
		MBS_1_5 = 43200;//8400*5;
		MBS_3_5 = 100800;//21015*5;
	}
	VICVectAddr5 = (unsigned long)MBSTimerHandler;
	VICVectCntl5 = 0x05;
	VICIntEnable |= (1<<5);
}

void MBSInitBuf(unsigned char *RxBuf)
{
	MBSRxData = RxBuf;
}

void MBSTransmitData(unsigned char *data,int len)
{
	//MBSLineStatus = 2; //������� ������
	//�������� �� ��������� ������ ��� ��� �� �������� �� �������� �����
	//MBSCost = 0;
	int i=0;
	MBSTxData = data;
	MBSTxLen = len;
	//���� �� ��������� ������� ������ �� ����� ��������� ������ �� �������� ������� �����������
	//����� ���� ����� ��������� ������ � �������� �����
	if(MBSLineInterval == 0)
	{
		MBSTxI = 1;
		//����������� �� �������� rs485
		if(MBSNumUart == 0)	FIO0SET |=RS485_DIR1; else FIO0SET |=RS485_DIR2;
		for(i=0;i<1000;i++);
		*MBSTHR = data[0];//�������� ��������
		*MBSIER = 0x03;//���������� ���������� �� �����������	
	}
	else
		MBSNedTx = 1;
}

void MBSUartHandler(void) __irq
{
	char LSR = *MBSLSR;
	char IIR = (*MBSIIR)&0x0E;
	if(IIR == 4)
	{
		if(LSR & (1<<0))//������ ������
		{
			//if((MBSLineStatus == 3)) {MBSLineStatus = 1; }//����� �� ����� ��������� ��������� ��� ������

			//MBSInterv = 1;

			if(MBSRxDataComplit ==0)//����� �� ������
			{
				//��������� ������ 1� ����
				T1MR1 = T1TC + MBS_3_5; 
				T1MCR |= (1<<3);

				if((MBSRxI<256))//� ������ ���� �����
				{
					MBSRxData[MBSRxI] = *MBSRBR;
					MBSRxI++;	
				}
				else 
					*MBSRBR;//���������� ���� ������ ������ ������	
			}
			else
			{
				//����� �� ����� �� ������������� ������ ����� ��������� ������ �� ������ ��������� ���������
				*MBSRBR;//���������� ���� ������ ������ ������
				//���������  ������ ��� ������� ��������� ����� ����� �������� 0� ����
				T1MR0 = T1TC + MBS_3_5;
				T1MCR |= (1<<0);
				//������ ���� ���������
				MBSLineInterval = 1;
			}
		}
	}else
	if(IIR == 2)//����������� �� �����������
	{
	 	if(LSR & (1<<5))//������� ����������� ����
		{
			//���������  ������ ��� ������� ��������� ����� ����� �������� 0� ����
			T1MR0 = T1TC + MBS_3_5;
			T1MCR |= (1<<0);

			if(MBSTxI < MBSTxLen)
			{
				*MBSTHR  = MBSTxData[MBSTxI];
				MBSTxI++;
			}
			else
			{
				MBSTxI = 0;//����� ������
				*MBSIER = 0x01;//��������� ���������� �� �����������
				//������������� �� ����� !!!????
				if(MBSNumUart == 0) FIO0CLR = RS485_DIR1; else FIO0CLR = RS485_DIR2;
				//MBSLineStatus = 3; //�������� ��������
			}
		}
	} 
	VICVectAddr = 0;
}

void MBSTimerHandler(void) __irq
{
	MBSMail* ptr;
	if(T1IR & (1<<0))//���������� 0 ����� 
	{
		T1IR |= (1<<0);//����� ����� ����������
		T1MCR &= ~(1<<0);//������ ����������
		if(MBSLineInterval == 0)
		{
			MBSRxDataComplit = 0;
			MBSRxI = 0;			
		}
		else
		{
			MBSLineInterval = 0;//����� �������� ��������
			if(MBSNedTx == 1)
			{
				//����� ���������� ������ ��� �� ��������
				MBSTxI = 1;
				//����������� �� �������� rs485
				if(MBSNumUart == 0)	FIO0SET |=RS485_DIR1; else FIO0SET |=RS485_DIR2;
				*MBSTHR = MBSTxData[0];//�������� ��������
				*MBSIER = 0x03;//���������� ���������� �� �����������
					MBSNedTx = 0;
			}
		}
	
	} 
	if(T1IR & (1<<1))//���������� 1 ����� 
	{
		T1IR |= (1<<1);//����� ����� ����������
		/*T2MR1 = T2TC + (MBS_3_5 - MBS_1_5);
		if(MBSInterv == 1)
		{
			MBSInterv = 2;
			if(MBSError == 0)
			{
				MBSError = MBSRxI;
			}
		}
		else*/
		{
			//����� ������
			MBSRxDataComplit = 1;//
			MBSCost = 1;
			T1MCR &= ~(1<<3);
			ptr = _alloc_box(mpool);
			if(MBSError != MBSRxI)
			{
				//������ ���������
				ptr->error = 1;//ERROR_INTRVAL_1_5;
			}
			else
				ptr->error = 0;
			ptr->data = MBSRxData;
			ptr->len = MBSRxI;
			isr_mbx_send(MBSBox,ptr);
		
		}
	}
	VICVectAddr = 0;
}

void MBSDeInitUart(int NumUart)
{
		if(NumUart == 0)
	{
		VICIntEnClr = 1<<6;
		VICVectAddr6 = 0;

		PINSEL0 &= ~0x50;
		//PCLKSEL &= ~;
		PCONP &= ~(1<<3);
		//������ ������ �������� ��� �������� �����
		U0LCR = 0;
		U0IER = 0;
	}
	if(NumUart == 1)
	{
		//!!!!!!!!!!!!!!
		VICIntEnClr = 1<<28;
		VICVectAddr28 = 0;

		PINSEL0 &= ~0x00F00000;
		//PCLKSEL &= ~;
		PCONP &= ~(1<<24);
		//������ ������ �������� ��� �������� �����
		U2LCR = 0;
		U2IER = 0;
	}
}

void MBSResetComplit()
{
	MBSRxDataComplit = 0;
	MBSRxI = 0;
	MBSCost = 0;
}

int MBSGetComplit(void)
{
	return MBSCost;
}

unsigned char* MBSGetData(void)
{
	return MBSRxData;
}
int MBSGetLen(void)
{
	return MBSRxI;
}

int MBSGetNumUart()
{
	return MBSNumUart;
}

