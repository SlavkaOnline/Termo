/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SPI_LPC23XX.C
 *      Purpose: Serial Peripheral Interface Driver for NXP LPC214x
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/
 

#include "includes.h"                 /* LPC23xx definitions                  */

#define __DRV_ID  spi0_drv
#define __PCLK    48000000

/* SPI Driver Interface functions */
static BOOL Init (void);
static BOOL UnInit (void);
static U8   Send (U8 outb);
static BOOL SendBuf (U8 *buf, U32 sz);
static BOOL RecBuf (U8 *buf, U32 sz);
static BOOL BusSpeed (U32 kbaud);
static BOOL SetSS (U32 ss);
static U32  CheckMedia (void);        /* Optional function for SD card check */

/* SPI Device Driver Control Block */
SPI_DRV __DRV_ID = {
  Init,
  UnInit,
  Send,
  SendBuf,
  RecBuf,
  BusSpeed,
  SetSS,
  CheckMedia                          /* Can be NULL if not existing         */
};




/*--------------------------- Init ------------------------------------------*/

static BOOL Init (void) 
{
	
	
	PINSEL3	&= ~(0x3C300‬);
	PINSEL3	|= (0x‭3C300);
	
	FIO1DIR |= 1<<21;
	FIO1SET	|= 1<<21;
	
	
	S0SPCR = (0x08);
	
	return (__TRUE);
}

/*--------------------------- UnInit ----------------------------------------*/

static BOOL UnInit (void) 
{
// 	FIO1DIR &=~ (1<<21);

// 	PINSEL3	&= ~(0x00000‬);
// 	
//   S0SPCR = (0x00);
	return (__TRUE);
}

/*--------------------------- Send ------------------------------------------*/

static U8 Send (U8 outb) 
{
  S0SPDR = outb;
  /* Wait if RNE cleared, Rx FIFO is empty. */
  while (!(S0SPSR & 0x80));
  return (S0SPDR);
}

/*--------------------------- SendBuf ---------------------------------------*/

static BOOL SendBuf (U8 *buf, U32 sz) 
{
  U32 i;

  for (i = 0; i < sz; i++) 
	{
      S0SPDR = buf[i];
    /* Wait if Tx FIFO is full. */
    while (!(S0SPSR & 0x80));
      S0SPDR;
  }
  /* Wait until Tx finished, drain Rx FIFO. */
  while (!(S0SPSR & 0x80)); 
	{
    S0SPDR;
  }
  return (__TRUE);
}

/*--------------------------- RecBuf ----------------------------------------*/

static BOOL RecBuf (U8 *buf, U32 sz) 
{
  U32 i;

  for (i = 0; i < sz; i++) 
	{
    S0SPDR = 0xFF;
    /* Wait while Rx FIFO is empty. */
    while (!(S0SPSR & 0x80));
    buf[i] = S0SPDR;
  }
  return (__TRUE);
}

/*--------------------------- BusSpeed --------------------------------------*/

static BOOL BusSpeed (U32 kbaud) 
{
  U32 div;
	
  div = (__PCLK/1000 + kbaud - 1) / kbaud;
  if (div == 0)   div = 0x02;
  if (div & 1)    div++;
  if (div > 0xFE) div = 0xFE;
  SSP0CPSR = div;
  return (__TRUE);
}

/*--------------------------- SetSS -----------------------------------------*/

static BOOL SetSS (U32 ss) 
{
	if (ss)
	{
		FIO1SET	|=1<<21;
	}
	else
	{
		FIO1CLR	|=1<<21;
	}
	return (__TRUE);
}

/*--------------------------- CheckMedia ------------------------------------*/

static U32 CheckMedia (void) 
{
  //U32 stat = 0;
	
	return 0;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
