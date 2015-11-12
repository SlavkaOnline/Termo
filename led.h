/**
	\file
	\brief Заголовочный файл с константами и функциями для работы со светодиодами   
*/


#ifndef led_H_
#define led_H_

#include "includes.h"


// Константы режимов работы 

#define LED_ON    1
#define LED_OFF   0


//Константы номеров  светодиодов 
	
#define PWR    0
#define RUN    1
#define HFAULT 2
#define FIRE1y 3
#define FIRE2y 4
#define FIRE3y 5
#define MHF    6
#define FIRE1r 7
#define FIRE2r 8
#define FIRE3r 9
#define LINK   10

/// Структура для доступа к функциям со светодиодами 
struct typeLed{
	
	void (*resetLed)();
	void (*initLed)();
	void (*setLed)(int, int);
	
	
};

/**
	\brief Функция инициализации выходов для светодиодов 
*/
void initLed();

/**
	\brief Функция сброса всех светодиодов 
*/
void resetLed();

/**
	\brief Функция включения\выключения светодиода. 
	
	\param [in] n - номер светодиода 
	\param [in] p - режим 1 - ВКЛ  0 - ВЫКЛ
*/
void setLed(int n, int p); 




extern struct typeLed led; ///< Структура с функциями управленем светодиодами 
extern int ledsBlink[11];  ///< Массив со светодиодами которые должны мигать 

#endif