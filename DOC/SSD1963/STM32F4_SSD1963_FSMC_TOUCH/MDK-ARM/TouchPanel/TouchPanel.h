#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "tft_lcd.h"

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;


typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

/* Private define ------------------------------------------------------------*/

#define	CHX 	0xD0 	/* ??µ?Y+µ??????????? */	
#define	CHY 	0x90	/* ??µ?X+µ??????????? */


//#define TP_CS(a)	if (a)	\
	\\				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);\
		\\			else		\
			\\		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); 

#define TP_INT_IN  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7 )

/* Private function prototypes -----------------------------------------------*/				
void TP_Init(void);	
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
void Touch_Cal_Read (Matrix * matrixPtr);					
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );

extern void GetPoint_TS  (uint16_t *x, uint16_t *y);
					
					
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


