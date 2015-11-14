#include <lpc23xx.h>

#include "includes.h"



#include <ctype.h>
#include <math.h>


/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 13

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];


int main(){
	
	int i,j;
	eMBErrorCode    eStatus;

	
	/* Инициализация */
	defaultData();
	initGPIO();
	setMode(0);
	initVIC();
	//testLM75();
	initSoftTimers();
	timer.initTimer0();
	timer.startTimer0(1);
	//timer.initWDTimer();
  //timer.startWDTimer();
	
	led.setLed(PWR, LED_ON);
	
  
	//data.threshold = 35;
	
// flashSPI.setSS(SPI_ON);
// flashSPI.sendSPI(0x3D);
// flashSPI.sendSPI(0x2A);
// flashSPI.sendSPI(0x7F);
// flashSPI.sendSPI(0xCF);
// flashSPI.setSS(SPI_OFF);
	
//  	dataFlash.saveParams();
//  	dataFlash.readParams();
// 	

// 	
// 	while(1){
// 	
// 	//led7.setNumLed7(data.temperature[0]);	//1
// 	//led7.setNumLed7(); 		//2
//   //led7.setNumLed7(data.temperature[2]);					//3
// 	led7.setNumLed7(data.temperature[3]);        			 //4
//   
// 		
// 	}
	
	


    eStatus = eMBInit( MB_RTU, 0x0A, 0, 19200, MB_PAR_EVEN );

    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

//======================================================//
usRegInputBuf[0] = (USHORT)pr_data->mode[0];
usRegInputBuf[1] = (USHORT)pr_data->mode[1];
usRegInputBuf[2] = (USHORT)pr_data->mode[2];
usRegInputBuf[3] = (USHORT)pr_data->mode[3];
usRegInputBuf[4] = (USHORT)pr_data->mode[4];
usRegInputBuf[5] = (USHORT)pr_data->THdelay;
usRegInputBuf[6] = (USHORT)pr_data->UTH;
usRegInputBuf[7] = (USHORT)pr_data->temperature[0];
usRegInputBuf[8] = (USHORT)pr_data->temperature[1];
usRegInputBuf[9] = (USHORT)pr_data->temperature[2];
usRegInputBuf[10] = (USHORT)pr_data->temperature[4];
usRegInputBuf[11] = (USHORT)0;
usRegInputBuf[12] = (USHORT)pr_data->codeError;

//==================================================//

    for( ;; )
    {    
			led7.setNumLed7(data.temperature[3]);
        ( void )eMBPoll(  );

        /* Here we simply count the number of poll cycles. */
        //usRegInputBuf[0]++;
    }
	

 	return 0;
 }
	

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNRegs;
    ( void )eMode;
    return MB_ENOREG;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}


