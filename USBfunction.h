#ifndef USBFUNCTION_H
#define USBFUNCTION_H


#include "includes.h"

/* Push Button Definitions */
#define PB_INT0         0x00000400  /* P2.10 */

/* LED Definitions */
#define LED_MSK         0x000000FF  /* P2.0..7 */

/* HID Demo Variables */
extern U8 InReport[64];
extern U8 OutReport[63];

/* HID Demo Functions */
extern void GetInReport  (void);
extern void SetOutReport (void);
extern void ChangeConstParam(void);
extern void setDateTime(void);
extern void getConstParam(void);
extern void sendDateTime(void);

#endif