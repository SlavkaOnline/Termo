#ifndef data_H_
#define data_H_


#define alarm_F1 0
#define alarm_F2 1
#define alarm_F3 2
#define alarm_All 4
#define alarm_FA 3
#define falseAlarm 1
#define notFalseAlarm 0
#define MAXtmzTemperature 50


/**
	\file
	\brief Заголовочный файл с основными параметрами сисетмы и функциями для работы с ними
*/

#include "includes.h"
/// Структура данных для хранение измеренных параметров и параметров системы
struct typeData {

	 short threshold;   ///< Порог измерения
	 short temperature[4];	///< Массив измеренных температур
	unsigned char  chanelError[4]; ///< Массив наличия ошибок на каналах
	unsigned short mode[5]; ///< Массив режимов
	unsigned char currentMode; ///Номер текущего режима
	unsigned char THdelay; ///< Задержка перед подачей тревоги
	unsigned short UTH; ///< Дельта повышения температуры
	unsigned char alarm[3]; ///< Наличие тревоги
	unsigned char codeError; ///< Код ошибки
	unsigned char work;  ///< Разрешение работы

	char Thyst;
	char Tos;
};

/// Структура для отправки по USB данных о константных значениях
struct typePCtoMCnumber{
    unsigned short threshold[5];  ///< Пороги
    unsigned short THdelay;       ///< Задержка перед подачей тревоги
    unsigned short UTH;           ///< Дельта повышения температуры
    unsigned char alarm[3];       ///< Наличие тревоги
    unsigned char currentMode;    ///< Номер текущего режима
};

/// Cтруктура для отправки времени по USB
struct typeDateTimeSend{

	unsigned char hh;   ///< Часы
	unsigned char mm;   ///< Минуты
	unsigned char ss;   ///< Секунды
	unsigned char dm;   ///< Число
	unsigned char mh;   ///< Месяц
	unsigned char yy;   ///< Год

};

/// Структура для отправки по USB данных о переменых значениях
struct typeInfo{

    struct typeDateTimeSend dt;
    struct typeDateTimeSend alarm;
    char dd;
    char mm;
    char yyyy;
    short workTime;


};


/**
	\brief Функция для настройки параметров по умолчанию.
*/
void defaultData();

/**
	\brief Функция для учения температуры из данных от термопары.

	\param [in] buf - 32bit'ое число от термопары

	\return Температура на канале
*/
int getDec(int buf);

int getCold(int buf);

/**
	\brief Функция записи температуры на канале в структуру данных.

	\param [in] chanel - номер канала
	\param [in] temperature - значение температуры
*/
void setTemperature(int chanel, int temperature);

/**
    \brief Функция инициализации порогов стандартными значениями.

*/
void initMode();

/**
	\brief Функция установки режима.

	\param [in] m - номер режима 0..4
*/
void setMode(int m);

/**
	\brief Функция проверки выхода за диапозон температуры на канале .

	\details Функция проверяет температуру на всех каналах, если находит выход,
			 за диапозн, запускает таймер задержки перед подачей тревоги.
*/
void getAssayTemperature();

/**
	\brief Функция вывода инфрмации на прибор о проблемах на канале.

	\details Функция проверяет в структуре данных наличия ощибки на канале,
			 в случае ошибки устанавливает и  выводит код ошибки, включает светодиод.
*/
void getAssayChanel();

/**
	\brief Функция проверяет наличие ошибки в данных с канала.

	\details Фукнция проверяет в данных с термопары наличия обрыва и КЗ,
			 устанавливает ошибку на канале  в структуре данных, если
             есть ошибка, возвращает 1, елси нет то 0.

	\param [in] chanel - номер канала
	\param [in] buf - данные с канала

	\return наличие ошибки - 1,2  отсутствие ошибки - 0

*/
int testChanel(int chanel, int buf);

/**
	\brief Функция инициализации портов ввода/вывода.
*/
void initGPIO();

/**
	\brief Функция посылает сигнал тревоги с канала.

	\param [in] chanel - номер канала
*/
void sendAlarm(int chanel);

/**
	\brief Функция снятия тревоги
*/
void disableAlarm(int chanel, int mode);

/**
	\brief Функция повышения порога на дельту UTH.
*/
void upThreshold();

/**
	\brief Функция тестирования канала по сигналу с DI

	\param [in] chanel - номер канала
	\param [in] flagFTC - наличие того что сигнал о тревоге был подан и удерживается  
*/
void forcedTestChanel(int chanel, int flagFTC);

/**
	\brief Функция проверки основных каналов на короткое замыкание. 
	
	\param [in] chanel - номер канала 
	\param [in] temperature - температура на канале 
	
	\details Функция проверяет измеренную температуру и сравнивает с прошлой,
			 если текущее значение меньше более чем на 30% прошлого, то на канале 
			 возникло короткое замыкане.
	
*/
int testSC(int chanel, int temperature);


/**
	\brief Функция получения значения температуры с ТМЗ канала.
	
	\param [in] buf - данные с микросхемы АЦП 
	
	\return [in] десятичное значение температуры 
*/
int getDecTMZ(int buf);

/**
	\brief Функция проверки ТМЗ канала на наличие ошибок.
	
	\param [in]  buf - данные с микросхемы АЦП
	
	\return наличие ошибки - 1, отсутствие ошибки - 0

*/
int testTMZChanel(int buf);

/**
	\brief Функция проверки канала ТМЗ на обрыв.

	\param [in] temperature - температура на канале 	
	
	\return наличие обрыва - 1, отсутствие обрыва - 0
	
	\details платиновый датчик имеет 4 контакта и при обрыве одного + или одного - может возникнуть ситуация,
			 что датчик сильно повысит температуру, но не сигнализирует об обрыве, максимальной возможной
			 температурой принято значение MAXtmzTemperature (50), если температура больше, то был оборван контакт. 
	
*/
int testBreakTMZ(int temperature);

void setConfigureDS1631();

void getConfigureDS1631();

int testLM75();

extern struct typeData data; ///< Структура данных
extern struct typeData * pr_data;

#endif
