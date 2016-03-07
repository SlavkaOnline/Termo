#ifndef softTimer_H_
#define softTimer_H_

#include <lpc23xx.h>

#include "includes.h"


/**
	\file
	\brief Заголовочный файл со структурами программных таймеров  
*/


/**
  \brief Функция инициализации программных таймеров.
 */
void initSoftTimers();

/**
	\brief Функция запуска таймера задержки перед тревогой.
	
	\param [in] chanel - номер канала
	\param [in] sec - задержка 
	
*/
void startTHdTimer(int chanel, int sec);


/// Структура, описывающая таймер для каждого управляющего порта 
struct t_readPortTimer{
	
	int multiplier; ///< Поправка частоты от апаратного таймера (меняется)
    int _multiplier; ///< Поправка частоты от апаратного таймера (константа) 
	int delay;		 ///< Время сигнала на порту (меняется)
	int _delay;		 ///< Время сигнала на порту (константа)
	int pin;		///< Номер порта (физический)
	int mode;      	///< Номер порта (программный)
	int hold;       ///< Наличие удеражания 
	void (*TimerFunc)();  ///< Указател на функцию, которая исполняется после исхода таймера
	
};

/// Структура, описывающая таймер мигания светодиодов 
struct t_blinkLedTimer{
	
	int multiplier; ///< Поправка частоты от апаратного таймера (меняется)
    int _multiplier; ///< Поправка частоты от апаратного таймера (константа)
	int flag[11];	 ///< Разрешение на мигание по номеру 
	void (*TimerFunc)(); ///< Указател на функцию, которая исполняется после исхода таймера
	
};

/// Стуруктура, описывающая таймер чтения данных по SSP1
struct t_SSP1Timer{
	int multiplier; ///< Поправка частоты от апаратного таймера (меняется)
    int _multiplier; ///< Поправка частоты от апаратного таймера (константа)
	void (*TimerFunc)(struct t_SSP1Timer*); ///< Указател на функцию, которая исполняется после исхода таймера
	
};
/// Стуруктура, описывающая таймер чтения данных по SPI
struct t_SPITimer{
	int multiplier; ///< Поправка частоты от апаратного таймера (меняется)
    int _multiplier; ///< Поправка частоты от апаратного таймера (константа)
	void (*TimerFunc)(struct t_SPITimer*); ///< Указател на функцию, которая исполняется после исхода таймера
	
};


/// Стуруктура, описывающая таймер задержки перед подачей тревоги 
struct t_THdTimer{
	
	int multiplier; ///< Поправка частоты от апаратного таймера
	int start; ///< Сигнал о начале отсчета
	int stop; ///< Сигнал о заверщении таймера
};

/** 
	\brief Функция, которая выполняется, когда истечет таймер порта 
	
	\param [in] struct t_readPortTimer* - таймер порта
*/
void portCommand(struct t_readPortTimer*);


/** 
	\brief Функция, которая выполняется, когда истечет таймер мигания светодиодом 
	
	\param [in] struct t_blinkLedTimer* - таймер светодиодов
*/
void BlinkLed(struct t_blinkLedTimer*);

/** 
	\brief Функция, которая выполняется, когда истечет таймер считывания SSP1 
	
*/
void recSSP1OnTimer();

/** 
	\brief Функция, которая выполняется, когда истечет таймер считывания SPI
	
*/
void recSPIOnTimer();


extern struct t_readPortTimer readPortTimer[10]; ///< Массив таймеров порта 
extern struct t_blinkLedTimer blinkLedTimer;	 ///< Таймер мигания светодиодами
extern struct t_SSP1Timer ssp1Timer;    		 ///< Таймер считывания SSP1 
extern struct t_SPITimer spiTimer;				 ///< Таймер считывания SPI 
extern struct t_THdTimer THdTimer[3];			///<  Массив таймеров задержки перед подачей тревоги на канале
#endif