/**
	\file
	\brief Заголовочный файл с константами и функциями для работы с интерфейсами SSP1 и SPI   
*/

#ifndef SSP_H_
#define SSP_H_

#include "includes.h"

#define TFE     1 << 0
#define TNF     1 << 1
#define RNE     1 << 2
#define RFF     1 << 3
#define BSY     1 << 4

#define SSP_ON  1
#define SSP_OFF 0

/// Структура для доступа к функциям интерфейса SSP и SPI .
struct typeSSP{
	/* SPI*/
	void (*initSPI)();
	int (*recSPI)();
	void (*setSPI)(int);
	
	/*SSP1*/
	void (*initSSP1)();
	int (*recSSP1)();
	void (*setSS1)(int, int);
	
	

};



/**
	\brief Функция инициализации интерфейса SSP1. 
*/
static void initSSP1();

/** 
	\brief Функция считывание из SSP1
	
	\return 32bit'ое число с данными 
*/
static int recSSP1();

/**
	\brief Функция активации чтения по SSP1. 
	
	\param [in] s - номер канала
	\param [in] n - активация - 1 деактивация - 0 
*/	
static void setSS1(int s, int n);

/**
	\brief Функция инициализации интерфейса SSP1. 
*/
static void initSPI();

/**
	\brief Функция активации чтения по SSP1. 
	
	\param [in] n - активация - 1 деактивация - 0 
*/	
static void setSPI(int n);


/** 
	\brief Функция считывание из SPI
	
	\return  число с данными 
*/

static int recSPI();


/** 
	\brief Функция конфигурации микросхмы АЦП платиного датчика.
*/
void configureMAX31865();

extern struct typeSSP SSP; ///< Структура с функциями управлением интерфейсом SSP

#endif