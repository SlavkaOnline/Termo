
#ifndef flashWork_H_
#define flashWork_H_


/**
	\file
	\brief Заголовочный файл с константами и функциями для работы с флеш памятью   
*/

#include "includes.h"

#define SETNULLHOUR 0
#define UPDATEHOUR  1
#define READHOUR    2

/**
	\brief Функция сохранения константант порогов и дельт повышения на флеш память.
*/
int saveParams();

/**
	\brief Функция считывания константант порогов и дельт повышения с флеш память.
*/
int readParams();

/**
	\brief Функция выдает суммароное время работы.
*/
short SumWorkTime();


/**
	\brief Функция для записи на флеш карту даты и времени последней сработаной тревоги.
	\details данные записываются в ОЗУ модуля часов реального времени.
*/
void setAlarmDateTime();

/**
	\brief Функция возвращает дату и время последней тревоги.

*/
struct typeDateTimeSend getAlarmDateTime();

/// Структура для работы с флеш памятью 
struct typeFlash{
int (*saveParams)();
int (*readParams)();
short (*SumWorkTime)();
void (*setAlarmDateTime)();
struct typeDateTimeSend (*getAlarmDateTime)();	
};

extern struct typeFlash dataFlash; ///< Структура для работы с Флеш памятью 

#endif
