#ifndef timer_H_
#define timer_H_

#include <lpc23xx.h>

#include "includes.h"


/**
	\file
	\brief Заголовочный файл функциями для работы с апаратными таймерами   
*/

/**
	\brief функция инициализации таймера под номером 0.
*/
void initTimer0();

/**
	\brief функция запуска таймера под номером 0.
	
	\param [in] mode  1 - запуск, 0 - остановить
*/
void startTimer0(int mode);

/** 
	\brief функция инициализации вотчдог таймера.
*/
void initWDTimer();

/** 
	\brief функция запуска и сброса вотчдог таймера.
*/
void startWDTimer();


/// Структура для работы с таймером под номером 0
struct typeTimer{
	
	void (*initTimer0)();
	void (*startTimer0)(int);
	void (*initWDTimer)();
    void (*startWDTimer)();
	
};

extern struct typeTimer timer; ///< Таймером под номером 0

#endif