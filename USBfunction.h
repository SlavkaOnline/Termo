#ifndef USBFUNCTION_H
#define USBFUNCTION_H


#include "includes.h"

/**
	\file
	\brief Заголовочный файл функциями для работы с USB 
*/


/* Push Button Definitions */
#define PB_INT0         0x00000400  /* P2.10 */

/* LED Definitions */
#define LED_MSK         0x000000FF  /* P2.0..7 */

/* HID Demo Variables */
extern U8 InReport[64]; ///< Буфер с данными принятыми по USB 
extern U8 OutReport[63];	///< Буфер с данными для отправки по USB 

/**
	\brief Функция, записывающая данные для тправки по USB.
*/
 void GetInReport  (void);
 
 /**
	\brief Функция для разбора данных принятых  по USB.
*/
 void SetOutReport (void);
 
 /**
	\brief Функиция, изменяющая пороги и константы на принятые по USB.
*/
 void ChangeConstParam(void);
 
 /**
	\brief Фукнция, устанавливающая время на принятое по USB.
*/
 void setDateTime(void);
 
 /**
	\brief Функция отправляющая данные по USB (пороги, константы, температуру).
*/
 void getConstParam(void);
 
  /**
	\brief Функция отправляющая данные по USB (время и информацию о приборе).
*/
 void sendDateTime(void);

#endif