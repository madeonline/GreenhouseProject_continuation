#include "stm32f10x.h"

#if !defined GUI_DRV_H
#define GUI_DRV_H

// Для записи данных
#define  LCD_DATA    			    ((uint32_t)0x60020000)
// Для записи команд
#define  LCD_REG   		  	    ((uint32_t)0x60000000)

#define DISP_WIDTH  240
#define DISP_HEIGHT 320

typedef enum
{
	Orientation_Portrait,
	Orientation_Album		// Landscape
} TOrientation;

void LCD_PutPixel ( uint16_t x, uint16_t y, uint16_t color );						//установка пикселя по координатам, цвет
void LCD_SetOrient ( TOrientation orientation );									//ориентация экрана
uint16_t LCD_GetMaxX ( void );
uint16_t LCD_GetMaxY ( void );
uint8_t LCD_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t textColor,
		uint16_t backColor, uint8_t isTransparent);	//пишем символ (координаты начала, символ, цвет текста, цвет фона, прозрачность фона)
uint8_t LCD_GetCharHight(void);
void LCD_DrawString(char* s, uint16_t x, uint16_t y, uint16_t color,
		uint16_t backColor, uint8_t isTransparent); //пишем строку (строка, координаты начала, цвет строки, цвет фона, прозрачность фона)
void LCD_SetCursor ( uint16_t Xpos, uint16_t Ypos ); //установка курсора
void LCD_drawLine ( int x1, int y1, int x2, int y2, uint16_t color); //рисуем линию (координаты начаал, координаты конца, цвет). Алгоритм Брезенхема
void LCD_DrawRect ( int x1, int y1, int x2, int y2, uint16_t color,uint8_t filled ); //рисуем прямоугольник(координаты верхнего левого угла, координаты правого нижнего угла, цвет, заливка)
void LCD_FillScr(uint16_t color); //заливка экрана
void LCD_DrawEllipse(uint16_t X1,uint16_t Y1,uint16_t R, uint16_t color); //рисуем окружность (координаты центра, радиус,цвет)


#endif
