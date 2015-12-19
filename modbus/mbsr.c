#define MBSR_local 2;
#include "mbsr.h"



extern unsigned short err_index;

void MBSRTUWork(MBSMail* ptr)
{
	//MBSMail* ptr;
	//MBSMail p;
	unsigned short crc;
	int tmp;
	
	
	//int adr,col,i;
	//while(1)
	{
		//os_mbx_wait(MBSBox,(void**)&ptr,0xffff);
		//while(MBSGetComplit() == 0);
		//ptr = &p;
		//ptr->data = MBSGetData();
		//ptr->len = MBSGetLen();
		
	 	if(ptr->len < 3)
		{
			//���� �������
			MBSResetComplit();
		}
		else
		{
			crc = (ptr->data[ptr->len-1] << 8) | ptr->data[ptr->len-2];
			if(CRC16(ptr->data,ptr->len-2) != crc)
			{
				//�������� crc
				MBSResetComplit();
			}
			else
			{
				if((SlaveAddres == ptr->data[0])/* || (ptr->data == 0)*/)
				{
					MBSWork(&ptr->data[1],ptr->len-3,&MBSTxDataBuf[1],&tmp);
					MBSTxDataBuf[0] = SlaveAddres;
					crc = CRC16(MBSTxDataBuf,tmp+1);
					MBSTxDataBuf[tmp+1] = crc;
					MBSTxDataBuf[tmp+2] = crc>>8;
					MBSTransmitData(MBSTxDataBuf,tmp+3);
					if((ptr->data[1] == 6) || (ptr->data[1] == 16))
						update_config();
				}
				else
				{
					MBSResetComplit();	
				}
			}
		}
		//_free_box(mpool,ptr);
	}
}

void MBSWork(unsigned char* data,int len,unsigned char* retdata,int *lenretdata)
{
		//unsigned short crc;
		int adr,col,i;
		switch(data[0])
					{
						case 3://������ �� ���������� ���������
							//��������� �������� ������ � ��������
							adr = (data[1]<<8) | data[2];
							col = (data[3]<<8) | data[4];
							if((adr + col < MasDataLen)&& adr >= 0)//���������� �� ������
							{
								//retdata[0] = SlaveAddres;
								retdata[0] = 3;
								retdata[1] = col*2;
								for(i = 0;i<col;i++)
								{
									retdata[2+i*2] = *MasData[i+adr]>>8;
									retdata[3+i*2] = *MasData[i+adr];
								}
								//crc = CRC16(MBSTxDataBuf,col*2+3);
								//MBSTxDataBuf[col*2+3]	= crc;
								//MBSTxDataBuf[col*2+4] = (crc>>8);
								*lenretdata = col*2+2;
								//MBSTransmitData(MBSTxDataBuf,col*2+5);
							}
							else
							{
								//MBSTransmitError(data,len,ERROR_DATA_ADDRES);
								retdata[0] = data[0]|(1<<7);
								retdata[1] = ERROR_DATA_ADDRES;
								*lenretdata = 2;
							}
							break;
						case 6://������ ������ ��������
							//��������� �������� ������ � ��������
							adr = (data[1]<<8) | data[2];
							//adr -= 40000;
							if(adr < MasDataLen)//����� �������� �������
							{
								if((ConstData[(adr/16)]&(1<<(adr%16))) == 0)//���� ��� ������ �� ��������� ��������
									*MasData[adr] = (data[3]<<8) | data[4];
								//���������� ����� ��� ����� �������
								//retdata[0] = SlaveAddres;
								retdata[0] = 6;
							  retdata[1] = data[1];
								retdata[2] = data[2];
								retdata[3] = data[3];
							  retdata[4] = data[4];
								*lenretdata = 5;
								//retdata[6] = data[5];
								//retdata[7] = data[6];
								//MBSTransmitData(MBSTxDataBuf,8);
							}
							else
							{
								//MBSTransmitError(data,len,ERROR_DATA_ADDRES);
								retdata[0] = data[0]|(1<<7);
								retdata[1] = ERROR_DATA_ADDRES;
								*lenretdata = 2;
							}
							break;
						case 16://������ ����� ��������
							adr = (data[1]<<8) | data[2];
							//adr -= 40000;
							col = (data[3]<<8) | data[4];
							if((adr+col < MasDataLen) && adr >= 0)
							{
								for(i = 0;i<col;i++)
								{
									if((ConstData[(adr+i)/16]&(1<<((adr+i)%16))) == 0)
										*MasData[adr + i] = (data[6 + i*2]<<8) | data[7 + i*2];	
								}
								//retdata[0] = SlaveAddres;
								retdata[0] = 16;
							  retdata[1] = data[1];
								retdata[2] = data[2];
								retdata[3] = data[3];
							  retdata[4] = data[4];
								*lenretdata = 5;
								//crc = CRC16(MBSTxDataBuf,6);
								//MBSTxDataBuf[6]	= crc;
								//MBSTxDataBuf[7] = (crc>>8);
 								//MBSTransmitData(MBSTxDataBuf,8);
							}
							else
							{//MBSTransmitError(data,len,ERROR_DATA_ADDRES);
								retdata[0] = data[0]|(1<<7);
								retdata[1] = ERROR_DATA_ADDRES;
								*lenretdata = 2;
							}
							break;
						default://������������ �������
							//MBSTransmitError(data,len,ERROR_FUNK);
							retdata[0] = data[0]|(1<<7);
							retdata[1] = ERROR_FUNK;
							*lenretdata = 2;
							break;	
					}
}
void SetSlaveAddres(unsigned char addr)
{
	SlaveAddres = addr;
}

void MBSTransmitError(unsigned char* data,int len,unsigned char error)
{
	unsigned short crc;
	//while(GetLineStatus() == 1){};//���� ������������ ����� �� �������� ��� ������ ����������� ������
	MBSTxDataBuf[0] = SlaveAddres;
	MBSTxDataBuf[1] = data[1] | (1<<7); //??
	MBSTxDataBuf[2] = 	error;
	crc = CRC16(MBSTxDataBuf,3);
	MBSTxDataBuf[3]	= crc;
	MBSTxDataBuf[4] = (crc>>8);
	MBSTransmitData(MBSTxDataBuf,5);
}

void MBSInit(int NumUart,unsigned int bnd_rate,unsigned char data_bit,unsigned char stop_bit,unsigned char flow_control,
				unsigned char* txd,unsigned char* rxd,
				unsigned short** mas,int len,unsigned short* constdat)
{
	MBSBndRate = bnd_rate;
	MBSDataBit = data_bit;
	MBSStopBit = stop_bit;
	MBSFlowControl = flow_control;
	
	MBSInitBuf(rxd);
	MBSTimerInit(bnd_rate);
	MBSUartInit(NumUart,bnd_rate,data_bit,stop_bit,flow_control);
	MasData = mas;
	MasDataLen = len;
	ConstData = constdat;	
	MBSTxDataBuf = txd;
}

void MBSReInit(int NumUart,unsigned int bnd_rate,unsigned char data_bit,unsigned char stop_bit,unsigned char flow_control,
				unsigned char* txd,unsigned char* rxd,
				unsigned short** mas,int len,unsigned short* constdat)
{
	MBSBndRate = bnd_rate;
	MBSDataBit = data_bit;
	MBSStopBit = stop_bit;
	MBSFlowControl = flow_control;
	
	MBSDeInitUart(MBSGetNumUart());
	MBSResetComplit();
	MBSInitBuf(rxd);
	MBSTimerInit(bnd_rate);
	MBSUartInit(NumUart,bnd_rate,data_bit,stop_bit,flow_control);
	MasData = mas;
	MasDataLen = len;
	ConstData = constdat;	
	MBSTxDataBuf = txd;
}

void MBSSetBndRate(unsigned char bnd_rate)
{
	char tmp = MBSGetNumUart();
	MBSDeInitUart(tmp);
	MBSResetComplit();
	MBSTimerInit(bnd_rate);
	MBSBndRate = bnd_rate;
	MBSUartInit(tmp,MBSBndRate,MBSDataBit,MBSStopBit,MBSFlowControl);
}

void MBSSetDataBit(unsigned char data_bit)
{
	char tmp = MBSGetNumUart();
	MBSDeInitUart(tmp);
	MBSResetComplit();
	//MBSInitBuf(rxd);
	MBSTimerInit(MBSBndRate);
	MBSDataBit = data_bit;
	MBSUartInit(tmp,MBSBndRate,data_bit,MBSStopBit,MBSFlowControl);
	/*MasData = mas;
	MasDataLen = len;
	ConstData = constdat;	
	MBSTxDataBuf = txd;*/
}

void MBSSetStopBit(unsigned char stop_bit)
{
	char tmp = MBSGetNumUart();
	MBSDeInitUart(tmp);
	MBSResetComplit();
	MBSTimerInit(MBSBndRate);
	MBSStopBit = stop_bit;
	MBSUartInit(tmp,MBSBndRate,MBSDataBit,MBSStopBit,MBSFlowControl);
}

void MBSSetFlowControl(unsigned char flow_control)
{
	char tmp = MBSGetNumUart();
	MBSDeInitUart(tmp);
	MBSResetComplit();
	MBSTimerInit(MBSBndRate);
	MBSFlowControl = flow_control;
	MBSUartInit(tmp,MBSBndRate,MBSDataBit,MBSStopBit,MBSFlowControl);
}

unsigned short CRC16(unsigned char *lMessage, unsigned int lCount)
{
	unsigned short lCRC;
	unsigned char lBits,lCnt;
	lCRC = 0xFFFF;
	for(lCnt=0;lCount>0;lCount--,lCnt++)
	{
		lCRC^=lMessage[lCnt];
		for(lBits=0;lBits<8;lBits++)
		{
			if (lCRC & 0x0001)
			{
				lCRC>>=1;
				lCRC^=0xA001;
			}
		else lCRC >>=1;
		}
	}
	return (lCRC);
}

void MBSInitConstData(unsigned short** data,unsigned short* constdat)
{
	//��������� ����������� ������ � ���� ������
	constdat[0] = (1<<5)|(1<<8)|(1<<12)|(1<<15);//|(1<<19)|(1<<24)|(1<<27)|(1<<31);
	constdat[1] = (1<<3)|(1<<8)|(1<<11)|(1<<12)|(1<<13)|(1<<14);
	constdat[2]	=	(1<<6);
	
// 	data[0]		= &MBStmp;
// 	data[1]		= &ustavki[T_min];				//������ ������� ��������� �������������� �������
// 	data[2]		= &ustavki[T_max];				//������� ������� ��������� �������������� �������
// 	data[3]		= &ustavki[T_ten_on];		//����������� ��������� ��������
// 	data[4]		= &ustavki[T_ten_off];		//����������� ���������� ��������
// 	data[5]		= &ustavki[T_avar];			//��������� ����������� �������� (��������� ��������)
// 	data[6]		= &izmer[T_tek];					//������� ����������� ��������
// 	//����������
// 	data[7]		= &ustavki[Rashod_min];	//������ ������� ��������� ������� �������
// 	data[8]		= &ustavki[Rashod_max];	//������� ������� ��������� ������� �������
// 	data[9]		= &izmer[Rashod_tek];		//������� ��������� �����������
// 	//���������
// 	data[10]	= &MBStmp;//&ustavki[Min_lavel];		//������ ������� ��������� ������� ������
// 	data[11]	= &MBStmp;//&ustavki[Max_lavel];		//������� ������� ��������� ������� ������
// 	data[12]	= &MBStmp;//&ustavki[Avar_lavel];	//������ ��������� ������� ������
// 	data[13]	= &izmer[Tek_lavel];			//������� ��������� ����������
// 	//������ pH
// 	data[14]	= &ustavki[pH_min];			//������ ������� ��������� ������� pH
// 	data[15]	= &ustavki[pH_max];			//������� ������� ��������� ������� pH
// 	data[16]	= &izmer[pH_tek];				//������� ��������� ������� pH
// 	//���������� ��������������
// 	data[17]	= &ustavki[gaz_min];			//������ ������� ��������� ������� ��������������
// 	data[18]	= &ustavki[gaz_max];			//������� ������� ��������� ������� ��������������
// 	data[19]	= &ustavki[gaz_first];		//������ ����� ���������������
// 	data[20]	= &izmer[gaz_tek];				//������� ��������� ���������������
// 	//���������� ��������
// 	data[21]	= &ustavki[P_min];				//������ ������� ��������� ������� ��������
// 	data[22]	= &ustavki[P_max];				//������� ������� ��������� ������� ��������
// 	data[23]	= &ustavki[avar_min];		//������ ��������� ������� ��������
// 	data[24]	= &ustavki[avar_max];		//������� ��������� ������� ��������
// 	data[25]	= &izmer[P_tek];					//������� ��������� ���������
// 	//�������������� ����������
// 	data[26]	= &ustavki[I_nedogr];		//��� ��������� ���������/������
// 	data[27]	= &ustavki[I_peregr];		//��� ��������� ���������/������
// 	data[28]	= &izmer[I_potr];				//������� ����������� ���� ����������/�������
// 	data[29]	= &ustavki[Rashod];			//��������� ������ ��������
// 	data[30]	= &ustavki[U_min];				//����������� �������� ���������� �������
// 	data[31]	= &ustavki[U_max];				//������������ �������� ���������� �������
// 	data[32]	= &izmer[Uab];						//��������� ���������� ab
// 	data[33]	= &izmer[Uac];						//��������� ���������� ac
// 	data[34]	= &izmer[Ubc];						//��������� ���������� bc
// 	data[35]	= &ustavki[V_pulse];			//����� ������������� �� ���� �������
// 	data[37]	= &ustavki[Proizv];			//������������������ ������ � ��� ��� 50��
// 	//���������
// 	data[38]	= &ustavki[T_wait_P_low];//����� �������� ����� ������� ������� ��������
// 	data[39]	= &ustavki[T_res_P_high];//����� ����������� ��� ������ �������� ��������
// 	data[40]	= &ustavki[Resets];			//���������� ������������ ��� ������ �������� ��������
// 	data[41]	= &ustavki[T_norm_P];		//����� ����������� �������� ��� ������ ������ �������� ��������
// 	//��������� ���������
// 	data[42]	= &err_index;						//����� ������
// 	data[43] 	= &MBStmp;
// 	data[44]	= &MBStmp;
// 	data[45]	= &MBStmp;
// 	data[46]	= &MBStmp;
// 	data[47]	= &MBStmp;
// 	data[48] 	= &MBStmp;
// 	data[49] 	= &MBStmp;
// 	data[50] 	= &MBStmp;
// 	data[51] 	= &MBStmp;
// 	data[52] 	= &MBStmp;
// 	data[53] 	= &MBStmp;
// 	data[54] 	= &MBStmp;
// 	data[55] 	= &MBStmp;
// 	data[56] 	= &MBStmp;
// 	data[57] 	= &MBStmp;
// 	data[58] 	= &MBStmp;
// 	data[59] 	= &MBStmp;
// 	data[60] 	= &MBStmp;
// 	data[61] 	= &MBStmp;
// 	data[62] 	= &MBStmp;
// 	data[63] 	= &MBStmp;
}
