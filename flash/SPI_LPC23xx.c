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
 
#include <File_Config.h>
#include <LPC23xx.H>                 /* LPC23xx definitions                  */

#define __DRV_ID  spi0_drv
#define __PCLK    48000000

/* SPI Driver Interface functions */
static fBOOL Init (void);
static fBOOL UnInit (void);
static U8   Send (U8 outb);
static fBOOL SendBuf (U8 *buf, U32 sz);
static fBOOL RecBuf (U8 *buf, U32 sz);
static fBOOL BusSpeed (U32 kbaud);
static fBOOL SetSS (U32 ss);
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


/* SSPSR - bit definitions. */
#define TFE     0x01
#define TNF     0x02
#define RNE     0x04
#define RFF     0x08
#define BSY     0x10

/*--------------------------- Init ------------------------------------------*/

static fBOOL Init (void) 
{
// 	FIO0DIR |=1<<16;
// 	FIO0SET	|=1<<16;
// 	
// 	PINSEL0	&=~0xC0000000;
// 	PINSEL1	&=~0x0000003C;
// 	PINSEL0	|=0x80000000;
// 	PINSEL1	|=0x00000028;
// 	
//   SSP0CR0  = 0x0007;
//   SSP0CR1  = 0x0002;
//   SSP0CPSR = 0xFE;
	
	
	
	PCLKSEL1 |= (1<<10);
	
	PINSEL3	&= ~(0x3C300);
	PINSEL3	|=  (0x3C300);
	
	FIO1DIR |= 1<<21;
	FIO1SET	|= 1<<21;
	
	SSP0CR0 = (0x0007); // 8bits CPOL = 0; CPHA = 0 "87"
	SSP0CPSR = (0x02);  //24mhz
	SSP0CR1 =(0x0002);  // SSE = 1, MS = 0, SOD = 0, LBM = 0;
	
	

	
	
	return (__TRUE);
}

/*--------------------------- UnInit ----------------------------------------*/

static fBOOL UnInit (void) 
{
	FIO0DIR &=~(1<<21);

	PINSEL3	&= ~(0x3C300);

  SSP0CR0  = 0;
  SSP0CR1  = 0;
  SSP0CPSR = 0;
	return (__TRUE);
}

/*--------------------------- Send ------------------------------------------*/

static U8 Send (U8 outb) 
{
  SSP0DR = outb;
  /* Wait if RNE cleared, Rx FIFO is empty. */
  while (!(SSP0SR & RNE));
  return (SSP0DR);
}

/*--------------------------- SendBuf ---------------------------------------*/

static fBOOL SendBuf (U8 *buf, U32 sz) 
{
  U32 i;

  for (i = 0; i < sz; i++) 
	{
    SSP0DR = buf[i];
    /* Wait if Tx FIFO is full. */
    while (!(SSP0SR & TNF));
    SSP0DR;
  }
  /* Wait until Tx finished, drain Rx FIFO. */
  while (SSP0SR & (BSY | RNE)) 
	{
    SSP0DR;
  }
  return (__TRUE);
}

/*--------------------------- RecBuf ----------------------------------------*/

static fBOOL RecBuf (U8 *buf, U32 sz) 
{
  U32 i;

  for (i = 0; i < sz; i++) 
	{
    SSP0DR = 0xFF;
    /* Wait while Rx FIFO is empty. */
    while (!(SSP0SR & RNE));
    buf[i] = SSP0DR;
  }
  return (__TRUE);
}

/*--------------------------- BusSpeed --------------------------------------*/

static fBOOL BusSpeed (U32 kbaud) 
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

static fBOOL SetSS (U32 ss) 
{
	if (ss)
	{
		FIO0SET	|=1<<21;
	}
	else
	{
		FIO0CLR	|=1<<21;
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
