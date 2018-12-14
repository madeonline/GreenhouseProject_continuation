#ifndef _XPT2046_H_
#define _XPT2046_H_

#include "stm32f10x.h"

#define CS_TOUCH_LOW   (GPIOB->BSRR =GPIO_BSRR_BR7)    //negative logic
#define CS_TOUCH_HIGH  (GPIOB->BSRR =GPIO_BSRR_BS7)    //negative logic

#define	Y 	0x90
#define	X 	0xD0


extern float ax,  ay;
extern int16_t bx, by;

extern void delay_us(uint32_t us);
extern void delay_ms(uint32_t ms);
extern uint8_t Touch_Verify_Coef(void);


void SPI_Touch_Config (void);
void Init_XPT2046(void);
uint8_t Spi_Master_Transmit(uint8_t out_data);
void Get_Touch_XY( volatile uint16_t *x_kor,volatile uint16_t *y_kor, uint8_t count_read, uint8_t isReadCorrected);

#endif

