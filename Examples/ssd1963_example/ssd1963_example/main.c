#include "ssd1963_fsmc.h"


#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

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


int main(void)
{
	Init_Periph();
	Init_SSD1963();
	
	
	TFT_Clear_Screen(BLUE);
	TFT_Clear_Screen(BLACK);

	TFT_Draw_Char(500,100,RED,BLACK,(const uint8_t*) font8x8,'5',3);
	TFT_Draw_Char(500,200,GREEN,BLACK,(const uint8_t*) font8x8,'b',3);
	TFT_Draw_Char(200,300,GREEN,BLACK,(const uint8_t*) font8x8,'5',3);
	
	TFT_Draw_String(300, 300, RED, BLACK,(const uint8_t*) font8x8, "hub",5);
	TFT_Draw_String(421, 300, BLUE, BLACK,(const uint8_t*) font8x8, "stub.ru",5);
	TFT_Draw_String(380, 350, WHITE, BLACK,(const uint8_t*) font8x8, "SSD1963",5);
	
	TFT_Draw_Line(120,120, 280,280,2,RED);
	TFT_Draw_Triangle(100,100,100,200,300,300,2,DARK_BLUE);
	TFT_Draw_Circle(70,350,50,0,2,BLUE);
    
    TFT_Draw_Line ( 10, 35, 60, 35, 2,RED);
	TFT_Draw_Line ( 35, 10, 35, 60, 2,RED );
	TFT_Draw_Circle(270,350,50,1,2,BLUE);
	TFT_Draw_Circle(650,150,100,0,2,GREEN);
	
	//TFT_Draw_Rectangle(700,350,50,20,4,WHITE);
	TFT_Draw_Round_Rect(150,350,100,100,10,2,WHITE);
	TFT_Draw_Fill_Round_Rect(400,150,100,100,10,WHITE);
	TFT_Draw_Fill_Round_Rect(150,50,170,50,10,WHITE);
	TFT_Draw_String(170, 65, BLUE, WHITE,(const uint8_t*) font8x8, "STM32",3);
	
	while(1)
	{

	
	}
}

