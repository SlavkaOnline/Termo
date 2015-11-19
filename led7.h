/**
	\file
	\brief Заголовочный файл с константами и функциями для работы со семисегментным индикатором   
*/



#ifndef led7_H_
#define led7_H_
 
#include "includes.h"

//Константы сегментов 
#define A1  1<<6
#define B1  1<<7
#define C1  1<<8
#define D1  1<<9
#define E1  1<<10
#define F1  1<<11
#define G1  1<<12

#define A2  1<<14
#define B2  1<<15
#define C2  1<<24
#define D2  1<<25
#define E2  1<<28
#define F2  1<<29
#define G2  1<<30

/// Структура для доступа к функциям с семисегментным индикатором
struct typeLed7{
	void (*initLed7)();
	void (*resetLed7)();
	void (*setNumLed7)(int);
	
};

/**
	\brief Функция инициализации выходов для семисегментного индикатора 
*/
void initLed7();

/**
	\brief Функция обнуления для семисегментного индикатора 
*/
void resetLed7();

/**
	\brief Функция вывода числа на  семисегментный индикатор 
*/
void setNumLed7(int);


extern struct typeLed7 led7; ///< Структура с функциями управлением семисегментным индикатором 

#endif