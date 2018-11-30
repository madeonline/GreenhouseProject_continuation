#include "stm32f10x.h"
#include  "System/System.h"
#if !defined LCD_DRIVER_H
#define LCD_DRIVER_H

// Для записи данных
#define  LCD_DATA    			    ((uint32_t)0x60020000)
// Для записи команд
#define  LCD_REG   		  	    ((uint32_t)0x60000000)

void writeLCDCommand(unsigned int reg, unsigned int value);
void writeLCDData(unsigned int data);

#endif
