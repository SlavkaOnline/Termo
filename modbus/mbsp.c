#define MBSP_local 2;
#include "mbsp.h"
/*
	Проверить что будет если во время обработки прийдет пакет 
	что будет с защитным интервалом?
	будет ли он выдержан?
	stop_bit  0 - 1 стоп бит; 1 - 2 стоп бита;
	flow_control
		000 - Отключен
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

	/*	PCONP |= (1<<3);//Включить питание уарта
		//PCLKSEL |= (1<<PCLK_UART0);//Разрешить тактирование
		PINSEL0 |= 0x50;//Подключаем к портам ввода вывода
		U0LCR = 0x83;//8-бит данных 1 стоп бит
		U0DLM = 0x00;//Настройка скорости уарта
		U0DLL = (12000000 * 5) / (bnd_rate * 16 * 4);
		U0LCR = stop_bit | data_bit | flow_control;//Запрет изменения скорости уарта	   8 bit data
		
		U0LCR &= ~0x38;
		if(flow_control != 0 )
		{
			U0LCR |= (flow_control<<3);
		}*/
		//U0FCR = 0;//Запретить FIFO
		PCONP |= (1<<3);//Включить питание уарта
		PINMODE0	|= 0x00000080;
		PINSEL0 	|= 0x00000050;
		U0LCR = 0x83;
		U0DLM = 0x00;	//устанавливае скорость последоовательного порта: ст. байт
		U0DLL   =   (12000000 * 5) / (bnd_rate * 16 * 4);
		U0LCR = 0x03; // DLAB = 0	запрещаем доступ к делителю
		U0FCR = 0x07; // регистр управления буфером FIFO - включение и  сброс TX и RX FIFO.
		U0IER = 0x01;//Разрешение прерываний
		//U0TER = 0x80;//Разрешение передачи???
	
		VICVectAddr6 = (unsigned long)MBSUartHandler; //Задаем адрес обработчика прерывания
		//VICVectCntl6 =	0x05;//Приоритет прерывания
		VICIntEnable |= (1<<6);//Разрешение прерывания
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

		//PCONP |= (1<<4);//Включить питание уарта
		PCONP |= ( 1 << 24 );
		PINSEL0 |= 0x00500000;
  		U2LCR = 0x83;//8-бит данных 1 стоп бит
		U2DLM = 0x00;//Настройка скорости уарта
		U2DLL = (12000000 * 5) / (bnd_rate * 16 * 4);
		U2LCR = stop_bit | data_bit | flow_control;//Запрет изменения скорости уарта
		/*U2LCR &= ~0x38;
		if(flow_control != 0 )
		{
			U2LCR |= (flow_control<<3);
		}*/
		U2FCR = 0x00;//Запретить FIFO
		U2IER = 0x01;//Разрешение прерываний
	 	
		VICVectAddr28 = (unsigned long)MBSUartHandler;    // UART2 Interrupt -> Vector 28
    	VICVectCntl28 = 0x05;                              // UART2 Interrupt -> Priority 2
    	VICIntEnable = 1 << 28;
	}
}

void MBSTimerInit(unsigned int bnd_rate)
{
	PCONP |= (1<<2);//Включаем питание таймера
	PCLKSEL0 |= (1<<4) | (0<<5); 	//Частота контроллера
	T1TCR = (1<<0);//Разрешиь работу счетчика
	//T2MCR = (1<<0)|(1<<3); //Разрешить прерывания по совпадению	 6 9 
	T1CCR = (0<<0);//Счетчик работает от внутреннего тактирования
	//T0PR = 0xffffffff;// Максимальное значение счета
	//T0PC = //Претделитель счетчика
	//if((sb != 1)||(sb != 0)) sb = 0;
	//if(fc != 0) fc = 1;
	if(bnd_rate < 19200)//Фиксированные интервалы
	{
		//T2MR0 = 18752;//1,5 интервал =(57600000*(1+8+1+ sb + fc)*1.5)/t	1 - старт бит 8 - бит дата 1 - стоп бит	6 бит
		//T2MR1 =	43752;//3,5 итервал	 =(57600000*(1+8+1+ sb + fc)*3.5)/t	 14 - бит всегда 
		MBS_1_5 = 108000;//18752*5;
		MBS_3_5 = 252000;//43752*5;
	}
	else
	{
		//T2MR0 = 8400;//1,5 интервал 1,75mc
		//T2MR1 =	21015;//3,5 итервал	 0,7mc
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
	//MBSLineStatus = 2; //Предача данных
	//Проверки на занятость ненадо так как мы отвечаем на принятый пакет
	//MBSCost = 0;
	int i=0;
	MBSTxData = data;
	MBSTxLen = len;
	//Если не принимали никаких данных во время обработки пакета то интервал считаем выдержанным
	//иначе ждем когда сработает таймер и передаем пакет
	if(MBSLineInterval == 0)
	{
		MBSTxI = 1;
		//Переключить на передачу rs485
		if(MBSNumUart == 0)	FIO0SET |=RS485_DIR1; else FIO0SET |=RS485_DIR2;
		for(i=0;i<1000;i++);
		*MBSTHR = data[0];//Начинаем передачу
		*MBSIER = 0x03;//Разрешение прерываний от передатчика	
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
		if(LSR & (1<<0))//Пришли данные
		{
			//if((MBSLineStatus == 3)) {MBSLineStatus = 1; }//Зашли во время защитного интервала это ошибка

			//MBSInterv = 1;

			if(MBSRxDataComplit ==0)//Пакет не принят
			{
				//Запускаем таймер 1й слот
				T1MR1 = T1TC + MBS_3_5; 
				T1MCR |= (1<<3);

				if((MBSRxI<256))//В буфере есть место
				{
					MBSRxData[MBSRxI] = *MBSRBR;
					MBSRxI++;	
				}
				else 
					*MBSRBR;//Сбрасываем флаг приема считав данные	
			}
			else
			{
				//Зашли во время не обработанного пакета нужно запустить таймер на отсчет защитного интервала
				*MBSRBR;//Сбрасываем флаг приема считав данные
				//запустить  таймер для отсчета интервала после конца передачи 0й слот
				T1MR0 = T1TC + MBS_3_5;
				T1MCR |= (1<<0);
				//Ставим флаг интервала
				MBSLineInterval = 1;
			}
		}
	}else
	if(IIR == 2)//Прерыванеие от передатчика
	{
	 	if(LSR & (1<<5))//Регистр передатчика пуст
		{
			//запустить  таймер для отсчета интервала после конца передачи 0й слот
			T1MR0 = T1TC + MBS_3_5;
			T1MCR |= (1<<0);

			if(MBSTxI < MBSTxLen)
			{
				*MBSTHR  = MBSTxData[MBSTxI];
				MBSTxI++;
			}
			else
			{
				MBSTxI = 0;//можно убрать
				*MBSIER = 0x01;//Запрещаем прерывания от передатчика
				//Переключаемся на прием !!!????
				if(MBSNumUart == 0) FIO0CLR = RS485_DIR1; else FIO0CLR = RS485_DIR2;
				//MBSLineStatus = 3; //Защитный интервал
			}
		}
	} 
	VICVectAddr = 0;
}

void MBSTimerHandler(void) __irq
{
	MBSMail* ptr;
	if(T1IR & (1<<0))//Совпадение 0 канал 
	{
		T1IR |= (1<<0);//Сброс флага прерывания
		T1MCR &= ~(1<<0);//Запрет прерывания
		if(MBSLineInterval == 0)
		{
			MBSRxDataComplit = 0;
			MBSRxI = 0;			
		}
		else
		{
			MBSLineInterval = 0;//истек защитный интервал
			if(MBSNedTx == 1)
			{
				//Нужно провкерить естьли что на передачу
				MBSTxI = 1;
				//Переключить на передачу rs485
				if(MBSNumUart == 0)	FIO0SET |=RS485_DIR1; else FIO0SET |=RS485_DIR2;
				*MBSTHR = MBSTxData[0];//Начинаем передачу
				*MBSIER = 0x03;//Разрешение прерываний от передатчика
					MBSNedTx = 0;
			}
		}
	
	} 
	if(T1IR & (1<<1))//Совпадение 1 канал 
	{
		T1IR |= (1<<1);//Сброс флага прерывания
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
			//Пакет принят
			MBSRxDataComplit = 1;//
			MBSCost = 1;
			T1MCR &= ~(1<<3);
			ptr = _alloc_box(mpool);
			if(MBSError != MBSRxI)
			{
				//Ошибки интервала
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
		//Поидее надобы обнулить все регистры уарта
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
		//Поидее надобы обнулить все регистры уарта
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

