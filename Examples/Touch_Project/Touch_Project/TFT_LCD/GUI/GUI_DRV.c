#include "stm32f10x.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>		// abs
#include "font.h"
#include "LCD_DRIVER.h"
#include "GUI_DRV.h"


static TOrientation orient = Orientation_Portrait;

extern const FONT_INFO lucidaConsole12ptFontInfo;
const FONT_INFO *font = &lucidaConsole12ptFontInfo;

void LCD_PutPixel ( uint16_t x, uint16_t y, uint16_t color );
void LCD_SetOrient ( TOrientation orientation );
uint16_t LCD_GetMaxX ( void );
uint16_t LCD_GetMaxY ( void );
uint8_t LCD_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t textColor,
		uint16_t backColor, uint8_t isTransparent);
uint8_t LCD_GetCharHight(void);
void LCD_DrawString(char* s, uint16_t x, uint16_t y, uint16_t color,
		uint16_t backColor, uint8_t isTransparent);
void LCD_SetCursor ( uint16_t Xpos, uint16_t Ypos );
void LCD_drawLine ( int x1, int y1, int x2, int y2, uint16_t color);
void LCD_DrawRect ( int x1, int y1, int x2, int y2, uint16_t color,uint8_t filled );

void LCD_FillScr(uint16_t color);
void LCD_DrawEllipse(uint16_t X1,uint16_t Y1,uint16_t R, uint16_t color);


void LCD_PutPixel ( uint16_t x, uint16_t y, uint16_t color )
{
	uint16_t x1=x, y1=y;

	if ( orient == Orientation_Album )
	{
		if ( y >= DISP_WIDTH || x >= DISP_HEIGHT )
					return;

		x1 = y;
		y1 = DISP_HEIGHT - 1 - x;
	} // if
	else
		if ( x >= DISP_WIDTH || y >= DISP_HEIGHT )
			return;

	writeLCDCommand(0x0020, x1);
		writeLCDCommand(0x0021, y1);
		*(uint16_t *) (LCD_REG) = 0x0022;
		writeLCDData(color);
} // SSD1289_PutPixel

void LCD_SetOrient ( TOrientation orientation )
{
	orient = orientation;
	//if(orient == Orientation_Portrait) TouchSetScreenSize(240,320); else {
	//	TouchSetScreenSize(320,240);
	//}
}

uint16_t LCD_GetMaxX ( void )
{
	if ( orient == Orientation_Portrait )
		return DISP_WIDTH - 1;

	// Album
	return DISP_WIDTH - 1;
} // GetMaxX


uint16_t LCD_GetMaxY ( void )
{
	if ( orient == Orientation_Portrait )
		return DISP_HEIGHT - 1;

	// Album
	return DISP_HEIGHT - 1;
}
/*******************************************************************/

uint8_t LCD_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t textColor,
		uint16_t backColor, uint8_t isTransparent) {
	uint8_t i, j;
	uint16_t mask, b, pixX, pixY;
	uint8_t width, height, charIdx;
	const uint8_t *ptr;
	uint8_t FirstChar, LastChar;

	if (!font)
		return 0;
	FirstChar = font->FirstChar;
	LastChar = font->LastChar;

	if (c < FirstChar || c > LastChar)
		return 0;

	charIdx = c - font->FirstChar;

	width = font->FontTable[charIdx].width;
	height = font->Height;

	ptr = font->FontBitmaps;
	b = font->FontTable[charIdx].start;
	ptr += b;

	pixY = y;

	for ( i = 0; i < height; i ++ )
	 {
	   pixX = x;

	   mask = 0x8000;
	   b = *ptr;
	   b <<= 8;
	   ptr ++;
	   j = *ptr;
	   ptr ++;
	   b |= j;

	   for ( j = 0; j < width; j ++ )
	   {
		   if ( (b&mask) !=0 )
			 LCD_PutPixel ( pixX, pixY, textColor );
		   else {
			   if ( isTransparent != 0 )
				   LCD_PutPixel ( pixX, pixY, backColor );
		   }

		   pixX ++;
		   mask >>= 1;
	   }

	   pixY ++;
	 }

	return width;
} // SSD1289_DrawChar

uint8_t LCD_GetCharHight(void) {
	return font->Height;
} // SSD1289_GetCharHight

void LCD_DrawString(char* s, uint16_t x, uint16_t y, uint16_t color,
		uint16_t backColor, uint8_t isTransparent) {
	int i, len = strlen(s);

	for (i = 0; i < len; i++)
		x += LCD_DrawChar(x, y, s[i], color, backColor, isTransparent);
}

void LCD_SetCursor ( uint16_t Xpos, uint16_t Ypos )
{
  writeLCDCommand( 0x0020, Xpos );			// X address counter
  writeLCDCommand( 0x0021, Ypos );			// Y address counter
}

void LCD_drawLine ( int x1, int y1, int x2, int y2, uint16_t color)
{

	//Brethentham algorithm
  int deltaX, deltaY, signX, signY, error, error2;
  int dX, dY;
  int x, y;

  if ( x1 == x2 )
  {
    if ( y2 > y1 )
    {
      for ( y = y1; y < y2; y ++ )
    	  LCD_PutPixel ( x1, y, color );
    } // if
    else
    {
      for ( y = y2; y < y1; y ++ )
    	  LCD_PutPixel ( x1, y, color );
    } // if

    return;
  } // if

  if ( y1 == y2 )
  {
    if ( x2 > x1 )
    {
      for ( x = x1; x < x2; x ++ )
    	  LCD_PutPixel ( x, y1, color );
    } // if
    else
    {
      for ( x = x2; x < x1; x ++ )
    	  LCD_PutPixel ( x, y1, color );
    } // else

    return;
  } // if

  dX = x2 - x1;
  dY = y2 - y1;
  deltaX = abs(dX);
  deltaY = abs(dY);
  signX = (x1 < x2) ? 1 : -1;
  signY = (y1 < y2) ? 1 : -1;
  error = deltaX - deltaY;

  LCD_PutPixel ( x2, y2, color );
  while ( x1 != x2 || y1 != y2 )
  {
	 LCD_PutPixel ( x1, y1, color );
	  error2 = error * 2;

    if ( error2 > -deltaY )
    {
      error -= deltaY;
      x1 += signX;
    } // if

    if ( error2 < deltaX )
    {
      error += deltaX;
      y1 += signY;
    } // if
  } // while
} // SSD1289_drawLine

void LCD_DrawRect ( int x1, int y1, int x2, int y2, uint16_t color,uint8_t filled  )
{
	if(filled != 0)
	{
		uint32_t i;

			for ( i = y1; i <= y2; i ++ )
				LCD_drawLine ( x1, i, x2, i, color );
	}
	else
	{
	LCD_drawLine ( x1, y1, x2, y1, color );

	LCD_drawLine ( x2, y1, x2, y2, color );

	LCD_drawLine ( x1, y2, x2, y2, color );

	LCD_drawLine ( x1, y1, x1, y2, color );
	}
}


void LCD_FillScr(uint16_t color) {
	int i = 0;
	*(uint16_t *) (LCD_REG) = 0x0022;
	for (i = 0; i < DISP_WIDTH * DISP_HEIGHT; i++) {
		writeLCDData(color);
	}
}

void LCD_DrawEllipse(uint16_t X1,uint16_t Y1,uint16_t R, uint16_t color)
{

	 int x = 0, y = R, gap = 0, delta = (2 - 2 * R);
	            while (y >= 0)
	            {
	                LCD_PutPixel( X1 + x, Y1 + y, color);
	                LCD_PutPixel( X1 + x, Y1 - y, color);
	                LCD_PutPixel( X1 - x, Y1 - y, color);
	                LCD_PutPixel( X1 - x, Y1 + y, color);
	                gap = 2 * (delta + y) - 1;
	                if (delta < 0 && gap <= 0)
	                {
	                    x++;
	                    delta += 2 * x + 1;
	                    continue;
	                }
	                if (delta > 0 && gap > 0)
	                {
	                    y--;
	                    delta -= 2 * y + 1;
	                    continue;
	                }
	                x++;
	                delta += 2 * (x - y);
	                y--;
	            }
}
