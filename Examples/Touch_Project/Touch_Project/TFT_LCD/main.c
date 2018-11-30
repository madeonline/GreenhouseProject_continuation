#include "stm32f10x_gpio.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "GUI/LCD_DRIVER.h"
#include "GUI/GUI_DRV.h"
#include "init.h"
#include "touch_screen/touch2046.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_spi.h"
#include "stdbool.h"
#include "misc.h"

uint16_t X=0,Y=0;

int main(void) {


	initPeriph();
	initFSMC();
   	initLCD();
	delay(10000);
	LCD_FillScr(0xFFFF);
	delay(100);
	LCD_SetOrient(Orientation_Portrait);
	delay(100);
//	LCD_DrawString("LGDP4532",30,30,0x888F,0x0000,0);
//	LCD_DrawRect(100,100,200,200,0x0000,0);
//	LCD_DrawRect(120,120,180,180,0xFF00,1);
	//LCD_DrawEllipse(150,150,50,0xF000);
	TouchInit();
	TouchSetScreenSize(240,320);
	TouchCalibrate();
	LCD_FillScr(0xFFFF);

	while (1) {
TouchReadXY(&X,&Y,true);
		   	LCD_PutPixel(X,Y,0x0000);
				}
}



