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
			//Мало приняли
			MBSResetComplit();
		}
		else
		{
			crc = (ptr->data[ptr->len-1] << 8) | ptr->data[ptr->len-2];
			if(CRC16(ptr->data,ptr->len-2) != crc)
			{
				//Неверное crc
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
						case 3://Чтение из нескольких регистров
							//Проверить диапазон данных и ответить
							adr = (data[1]<<8) | data[2];
							col = (data[3]<<8) | data[4];
							if((adr + col < MasDataLen)&& adr >= 0)//Отправляем им данные
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
						case 6://Запись одного значения
							//Проверить диапазон данных и ответить
							adr = (data[1]<<8) | data[2];
							//adr -= 40000;
							if(adr < MasDataLen)//Пишем принятый регистр
							{
								if((ConstData[(adr/16)]&(1<<(adr%16))) == 0)//Если нет защиты то сохраняем значение
									*MasData[adr] = (data[3]<<8) | data[4];
								//Нормальный ответ это копия запроса
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
						case 16://Запись много значения
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
						default://Недопустимая команда
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
	//while(GetLineStatus() == 1){};//Ждем освобождения линии от передачи для защиты передающего буфера
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
	//Настройка константных данных в сети модбас
	constdat[0] = (1<<5)|(1<<8)|(1<<12)|(1<<15);//|(1<<19)|(1<<24)|(1<<27)|(1<<31);
	constdat[1] = (1<<3)|(1<<8)|(1<<11)|(1<<12)|(1<<13)|(1<<14);
	constdat[2]	=	(1<<6);
	
// 	data[0]		= &MBStmp;
// 	data[1]		= &ustavki[T_min];				//Нижняя граница показаний температурного датчика
// 	data[2]		= &ustavki[T_max];				//Верхняя граница показаний температурного датчика
// 	data[3]		= &ustavki[T_ten_on];		//Температура включения обогрева
// 	data[4]		= &ustavki[T_ten_off];		//Температура выключения обогрева
// 	data[5]		= &ustavki[T_avar];			//Аварийная температура реагента (повышение вязкости)
// 	data[6]		= &izmer[T_tek];					//Текущая температура реагента
// 	//Расходомер
// 	data[7]		= &ustavki[Rashod_min];	//Нижняя граница показаний датчика расхода
// 	data[8]		= &ustavki[Rashod_max];	//Верхняя граница показаний датчика расхода
// 	data[9]		= &izmer[Rashod_tek];		//Текущие показания расходомера
// 	//Уровнемер
// 	data[10]	= &MBStmp;//&ustavki[Min_lavel];		//Нижняя граница показаний датчика уровня
// 	data[11]	= &MBStmp;//&ustavki[Max_lavel];		//Верхняя граница показаний датчика уровня
// 	data[12]	= &MBStmp;//&ustavki[Avar_lavel];	//Нижняя аварийная граница уровня
// 	data[13]	= &izmer[Tek_lavel];			//Текущие показания уровнемера
// 	//Датчик pH
// 	data[14]	= &ustavki[pH_min];			//Нижняя граница показаний датчика pH
// 	data[15]	= &ustavki[pH_max];			//Верхняя граница показаний датчика pH
// 	data[16]	= &izmer[pH_tek];				//Текущие показания датчика pH
// 	//Аналоговый газоанализатор
// 	data[17]	= &ustavki[gaz_min];			//Нижняя граница показаний датчика загазованности
// 	data[18]	= &ustavki[gaz_max];			//Верхняя граница показаний датчика загазованности
// 	data[19]	= &ustavki[gaz_first];		//Первый порог газоанализатора
// 	data[20]	= &izmer[gaz_tek];				//Текущие показания газоанализатора
// 	//Аналоговый манометр
// 	data[21]	= &ustavki[P_min];				//Нижняя граница показаний датчика давления
// 	data[22]	= &ustavki[P_max];				//Верхняя граница показаний датчика давления
// 	data[23]	= &ustavki[avar_min];		//Нижняя аварийная граница давления
// 	data[24]	= &ustavki[avar_max];		//Верхняя аварийная граница давления
// 	data[25]	= &izmer[P_tek];					//Текущие показания манометра
// 	//Исполнительное устройство
// 	data[26]	= &ustavki[I_nedogr];		//Ток недогруза толкателя/насоса
// 	data[27]	= &ustavki[I_peregr];		//Ток недогруза толкателя/насоса
// 	data[28]	= &izmer[I_potr];				//Текущее потребление тока толкателем/насосом
// 	data[29]	= &ustavki[Rashod];			//Требуемый расход реагента
// 	data[30]	= &ustavki[U_min];				//Минимальное значение напряжения питания
// 	data[31]	= &ustavki[U_max];				//Максимальное значение напряжения питания
// 	data[32]	= &izmer[Uab];						//Межфазное напряжение ab
// 	data[33]	= &izmer[Uac];						//Межфазное напряжение ac
// 	data[34]	= &izmer[Ubc];						//Межфазное напряжение bc
// 	data[35]	= &ustavki[V_pulse];			//Объем перекачивания за один импульс
// 	data[37]	= &ustavki[Proizv];			//Производительность насоса в час при 50Гц
// 	//Алгоритмы
// 	data[38]	= &ustavki[T_wait_P_low];//Время ожидания перед аварией низкого давления
// 	data[39]	= &ustavki[T_res_P_high];//Время перезапуска при аварии высокого давления
// 	data[40]	= &ustavki[Resets];			//Количество перезапусков при аварии высокого давления
// 	data[41]	= &ustavki[T_norm_P];		//Время нормального давления для сброса аварии высокого давления
// 	//Системные параметры
// 	data[42]	= &err_index;						//Флаги ошибок
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
