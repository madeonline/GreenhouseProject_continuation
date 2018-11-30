#if !defined TOUCH2046_H
#define TOUCH2046_H

#include <stdio.h>
#include <stdbool.h>
#include "math.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "GUI/GUI_DRV.h"

void TouchInit ( void );
void TouchSetScreenSize ( uint16_t sizeX, uint16_t sizeY );
bool TouchReadXY ( uint16_t *px, uint16_t *py, bool isReadCorrected );
void TouchCalibrate ( void );

#endif
