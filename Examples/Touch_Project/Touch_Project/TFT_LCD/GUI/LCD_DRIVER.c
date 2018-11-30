#include "LCD_DRIVER.h"
#include "stm32f10x.h"


/*******************************************************************/
// Простенькая функция задержки


/*******************************************************************/
// Так мы будем писать команды в регистры LCD
void writeLCDCommand(unsigned int reg, unsigned int value) {
	*(uint16_t *) (LCD_REG) = reg;
	*(uint16_t *) (LCD_DATA) = value;
}

/*******************************************************************/
// А так данные..
void writeLCDData(unsigned int data) {
	*(uint16_t *) (LCD_DATA) = data;
}
