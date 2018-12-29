#include "ssd1963_fsmc.h"
#include "xpt2046_spi.h"
#include <stdio.h>



#define 	 TFT_PIXEL_WIDTH  800
#define 	 TFT_PIXEL_HEIGHT 480

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC



#define 	 ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define 	 ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define 	 ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define  	 DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define 	 TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}

static const int16_t xCenter[] = { 35, TFT_PIXEL_WIDTH -35, 35, TFT_PIXEL_WIDTH -35 };
static const int16_t yCenter[] = { 35, 35, TFT_PIXEL_HEIGHT-35, TFT_PIXEL_HEIGHT-35 };
static int16_t xPos[5], yPos[5];

static float axc[2],  ayc[2];
static int16_t bxc[2], byc[2];

float ax, ay;
int16_t bx,  by;


volatile uint16_t touch_kor_x= 0;
volatile uint16_t touch_kor_y= 0;





void delay_us(uint32_t us)
{
	int32_t us_count_tick =  us * (SystemCoreClock/1000000);
	//разрешаем использовать счётчик
	SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        //обнуляем значение счётного регистра
	DWT_CYCCNT  = 0;
        //запускаем счётчик
	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk; 
	while(DWT_CYCCNT < us_count_tick);
        //останавливаем счётчик
	DWT_CONTROL &= ~DWT_CTRL_CYCCNTENA_Msk;
	
}
//////////////////////////////
void delay_ms(uint32_t ms)
{
	int32_t ms_count_tick =  ms * (SystemCoreClock/1000);
	//разрешаем использовать счётчик
	SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
         //обнуляем значение счётного регистра
	DWT_CYCCNT  = 0;
        //запускаем счётчик
	DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; 
	while(DWT_CYCCNT < ms_count_tick);
        //останавливаем счётчик
	DWT_CONTROL &= ~DWT_CTRL_CYCCNTENA_Msk;
	
}

////////////////////////////

//ф-ция фиксирует касание(TS_IRQ -  PB6)
uint8_t isTouch ( void )
{
   	if(!(GPIOB->IDR & GPIO_IDR_IDR6))
    	return 1;	// касание было

	return 0;
} 
/////////////////////////////

//ф-ция устанавливает калибровочные коэффициенты
void Touch_Set_Coef ( float _ax, int16_t _bx, float _ay, int16_t _by )
{
	ax = _ax;
	bx = _bx;
	ay = _ay;
	by = _by;
} 
/////////////////////////////
//ф-ция калибровки
void Touch_Calibrate ( void )
{
	uint16_t x, y;

	//рисуем крестик в левом верхнем углу
	TFT_Clear_Screen (WHITE);
	TFT_Draw_String( 50, 100, 0xFFE0, 0x0000, *font8x8, "Callibration", 3  );
 	TFT_Draw_Line ( 10, 10+25, 10+50, 10+25, 2,1);
	TFT_Draw_Line ( 10+25, 10, 10+25, 10+50, 2,1 );

	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Press",  3 );
	while (1)
	{
		//ждать нажатия
		while ( !isTouch ( ) );
		Get_Touch_XY ( &x, &y, 100, 1);//производим измерения 100 раз
		if (x < 4090 && y < 4090)
		{
			xPos[0] = x;
			yPos[0] = y;
			break;
		} 
	} 

	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Off   ",  3 );
 //   TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, String*(x),  3 );
	//ждать отпускания
	while ( isTouch ( ) );
	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8,  "     ", 3 );


	//рисуем крестик в правом верхнем углу
	TFT_Clear_Screen (WHITE);
	TFT_Draw_String( 50, 100, 0xFFE0, 0x0000, *font8x8, "Callibration", 3 );
	TFT_Draw_Line  ( TFT_PIXEL_WIDTH -10-50, 10+25, TFT_PIXEL_WIDTH -10-50+50, 10+25, 2,1 );
	TFT_Draw_Line  ( TFT_PIXEL_WIDTH -10-25, 10, TFT_PIXEL_WIDTH -10-25, 10+50, 2,1 );

	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Press",  3 );
	while (1)
	{
		//ждать нажатия
		while ( !isTouch ( ) );
		Get_Touch_XY ( &x, &y, 100, 1);//производим измерения 100 раз
		if (x < 4090 && y < 4090)
		{
			xPos[1] = x;
			yPos[1] = y;
			break;
		} 
	} 
	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Off   ",  3 );

	//ждать отпускания
	while ( isTouch ( ) );
	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8,  "     ", 3 );
	
	
	//рисуем крестик в левом нижнем углу
	TFT_Clear_Screen (WHITE);
	TFT_Draw_String( 50, 100, 0xFFE0, 0x0000, *font8x8, "Callibration", 3 );
	TFT_Draw_Line  ( 10, TFT_PIXEL_HEIGHT-10-25, 10+50, TFT_PIXEL_HEIGHT-10-25, 2,1 );	// hor
	TFT_Draw_Line  ( 10+25, TFT_PIXEL_HEIGHT-10-50, 10+25, TFT_PIXEL_HEIGHT-10-50+50, 2,1 );	// vert

	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Press",  3 );
	while (1)
	{
		// ждать нажатия
		while ( !isTouch ( ) );
		Get_Touch_XY ( &x, &y, 100, 1);//производим измерения 100 раз
		if (x < 4090 && y < 4090)
		{
			xPos[2] = x;
			yPos[2] = y;
			break;
		} 
	} 
	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Off   ",  3 );

	// ждать отпускания
	while ( isTouch ( ) );
	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8,  "     ", 3 );
	
	
	//рисуем крестик в правом нижнем углу
	TFT_Clear_Screen (WHITE);
	TFT_Draw_String( 50, 100, 0xFFE0, 0x0000, *font8x8, "Callibration",  3 );
	TFT_Draw_Line  ( TFT_PIXEL_WIDTH -10-50, TFT_PIXEL_HEIGHT-10-25, TFT_PIXEL_WIDTH -10-50+50, TFT_PIXEL_HEIGHT-10-25, 2,1 );	// hor
	TFT_Draw_Line  ( TFT_PIXEL_WIDTH -10-25, TFT_PIXEL_HEIGHT-10-50, TFT_PIXEL_WIDTH -10-25, TFT_PIXEL_HEIGHT-10-50+50, 2,1 );	// vert

	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Press",  3 );
	while (1)
	{
		// ждать нажатия
		while ( !isTouch ( ) );
		Get_Touch_XY ( &x, &y, 100, 1);
		if (x < 4090 && y < 4090)
		{
			xPos[3] = x;
			yPos[3] = y;
			break;
		}
	}
	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8, "Off   ",  3 );

	//ждать отпускания
	while ( isTouch ( ) );
	TFT_Draw_String( 50, 120, 0xFFE0, 0x0000, *font8x8,  "     ", 3 );

	
	//Расчёт коэффициентов. 
	//xPos - значения полученные с помощью тачскринаx, xCenter - реальные координаты точек на дисплее
	axc[0] = (float)(xPos[3] - xPos[0])/(xCenter[3] - xCenter[0]);
	bxc[0] = xCenter[0] - xPos[0]/axc[0];
	ayc[0] = (float)(yPos[3] - yPos[0])/(yCenter[3] - yCenter[0]);
	byc[0] = yCenter[0] - yPos[0]/ayc[0];

	/*axc[1] = (float)(xPos[2] - xPos[1])/(xCenter[2] - xCenter[1]);
	bxc[1] = xCenter[1] - xPos[1]/axc[1];
	ayc[1] = (float)(yPos[2] - yPos[1])/(yCenter[2] - yCenter[1]);
	byc[1] = yCenter[1] - yPos[1]/ayc[1];*/

	// Сохранить коэффициенты
	Touch_Set_Coef ( axc[0], bxc[0], ayc[0], byc[0] );

	TFT_Clear_Screen (WHITE);
	TFT_Draw_String( 50, 100, 0xFFE0, 0x0000, *font8x8, "Callibration_End",  3 );
	delay_ms (1000);	// 1 sec
}


int main(void)
{
	Init_Periph();
	Init_SSD1963();
	
	SPI_Touch_Config();
	Init_XPT2046();
    
		
	TFT_Clear_Screen(BLUE);
	Touch_Calibrate();
	TFT_Clear_Screen(BLUE);
    
	TFT_Draw_Char(500,100,RED,BLACK,(const uint8_t*) font8x8,'5',3);
	TFT_Draw_Char(500,200,GREEN,BLACK,(const uint8_t*) font8x8,'b',3);
	TFT_Draw_Char(200,300,GREEN,BLACK,(const uint8_t*) font8x8,'6',3);
	
	TFT_Draw_String(300, 300, RED, BLACK,(const uint8_t*) font8x8, "hub",5);
	TFT_Draw_String(421, 300, BLUE, BLACK,(const uint8_t*) font8x8, "stub.ru",5);
	TFT_Draw_String(380, 350, WHITE, BLACK,(const uint8_t*) font8x8, "SSD1963",5);
	
	TFT_Draw_Line(120,120, 280,280,BLACK, RED);
	TFT_Draw_Triangle(200,100,100,200,300,300,2,RED);
	TFT_Draw_Circle(70,350,50,0,2,RED);
	TFT_Draw_Circle(270,350,50,1,2,RED);
	TFT_Draw_Circle(650,150,100,0,2,GREEN);
	
	//TFT_Draw_Rectangle(700,350,50,20,4,WHITE);
	TFT_Draw_Round_Rect(150,350,100,100,10,2,WHITE);
	TFT_Draw_Fill_Round_Rect(400,150,100,100,10,WHITE);
	TFT_Draw_Fill_Round_Rect(150,50,170,50,10,WHITE);
	TFT_Draw_String(170, 65, BLUE, WHITE,(const uint8_t*) font8x8, "STM32",3);
	
	while(1)
	{
		if(isTouch())
		{
			
			Get_Touch_XY(&touch_kor_x, &touch_kor_y,10,0);
			printf(" ax = %f  bx = %d  x = %d\r\n",ax,bx,touch_kor_x); 
			TFT_Draw_Fill_Rectangle(touch_kor_x,touch_kor_y,2,2,2);
          //  TFT_Draw_Line(touch_kor_x,touch_kor_y,touch_kor_x+2,touch_kor_y+2,2, RED);
            

		}
	
	}
}

