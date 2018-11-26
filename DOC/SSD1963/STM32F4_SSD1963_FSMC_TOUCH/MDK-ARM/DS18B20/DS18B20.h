#ifndef _DS1820_H
#define _DS1820_H 
//#include "Init_Perif.h"
#include "stm32f4xx_hal.h"
#include  <stdbool.h>

#define   SET_Write_Conf_DS       1  // 1 - вписать конфиг файл в ЕЕПРОМ DS18B20, 0 - не вписывать, пропустить этот шаг, конфиг - с десятыми (0,1гр.С) температуры
#define  _XTAL_FREQ_us  42044 

/****************************************************************************************/
#define		DS1820_PORT			GPIOC
#define		DS1820_PIN_NUM	GPIO_PIN_5
//---------------------------------------------------------//
#define		DS_GET_PORT()	HAL_GPIO_ReadPin(DS1820_PORT, DS1820_PIN_NUM)
//---------------------------------------------------------//
#define		flash_t		const  	// Спецификатор размещения данных во flash памяти, для таблицы
#define		IE(val)		;		    // Запрет/разрешение прерываний
/****************************************************************************************/

/****************************************************************************************/
extern unsigned char  zn_ds;
extern signed int     TERMO_DS;
extern unsigned char			  ds_cnt, ds_cnt_next, srch_count;			// Кол-во найденых датчиков
extern bool					end_fl;		// Флаг окончания преобразования температур

void Write_config_ds1820 (void);
extern void search_ROM (void);
bool		ds1820_presence(void);
unsigned char 	ds1820_write(unsigned char out_val);
#define			ds1820_read()	ds1820_write(0xff)
unsigned char 	ds1820_crc(char *arr, unsigned char len);
bool 			ds1820_first_search(char *ROM_BUF);
bool 			ds1820_next_search(char *ROM_BUF);
void CONV_TRMO (void);
void receiv_TERMO(void);
extern void DS1820_Task(void);
extern void next_search_ROM (unsigned char set);

void DS_PORT_1(void);
void DS_PORT_0(void);




extern signed int ds1820_trm (char x);
extern  char ds1820_zn(char x);


typedef struct{
	char           		rom[8];			// ID датчика
	signed short int	termo;			// Температура
	unsigned  char    sign;
	unsigned  char    crc_ds;
}_ds1820;

extern _ds1820 ds1820[64];

/****************************************************************************************/

#endif
