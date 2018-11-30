#include <stdio.h>
#include "stm32f4xx.h"
#include <rtl.h>

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "SSD1963.h"
#include "SSD1963_Configuration.h"
#include "SSD1963_CMD.h"

void delay_us(unsigned int d){	/* for 168 MHz @ Level3 Opt */
	unsigned int i,j;
	i=d;while(i){i--;j=55;while(j)j--;}
}

void delay_ms(unsigned int d){	/* for 168 MHz @ Level3 Opt */
	unsigned int i;
	i=d;while(i){i--;delay_us(998);}
}

int main (void) {
	u16	i=0,j=0;
	
	delay_us(1);		 // wait a short period of time

	LCD_ResetDevice();
	delay_us(1);		 // wait a short period of time

	LCD_SetBacklight(20);
  LCD_Clear(BLUE);
	delay_ms(1000L);		 // wait a short period of time
  LCD_Clear(RED);
	delay_ms(1000L);		 // wait a short period of time
  LCD_Clear(BLUE);
	delay_ms(1000L);		 // wait a short period of time

	for(i=0;i<200;i++){
		for (j=0;j<200;j++){
			LCD_PutPixel(i,j,RED);
		}
	}


	while(1){
	  LCD_Clear(RED);
		delay_ms(1000L);		 // wait a short period of time
		LCD_Clear(BLUE);
		delay_ms(1000L);		 // wait a short period of time
	}
}
