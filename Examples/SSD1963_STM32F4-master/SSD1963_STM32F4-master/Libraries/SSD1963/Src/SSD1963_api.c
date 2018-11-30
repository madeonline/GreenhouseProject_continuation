#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_gpio.h"
#include "SSD1963.h"
#include "SSD1963_api.h"
#include "SSD1963_font.h"

void LCD_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			LCD_PutPixel(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			LCD_PutPixel(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			LCD_PutPixel(x,y,color);
  		}
 	}
 	else
 	{
 		LCD_PutPixel(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				LCD_PutPixel(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				LCD_PutPixel(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				LCD_PutPixel(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				LCD_PutPixel(x, y,color);
    			}
   			}
  		}
 	}
}


void LCD_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
	u16 x,y;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);

	while(y>x)
	{
		if(fill)
		{
			LCD_Line(cx+x,cy+y,cx-x,cy+y,color);
			LCD_Line(cx+x,cy-y,cx-x,cy-y,color);
			LCD_Line(cx+y,cy+x,cx-y,cy+x,color);
			LCD_Line(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
			LCD_PutPixel(cx+x,cy+y,color);
			LCD_PutPixel(cx-x,cy+y,color);
			LCD_PutPixel(cx+x,cy-y,color);
			LCD_PutPixel(cx-x,cy-y,color);
			LCD_PutPixel(cx+y,cy+x,color);
			LCD_PutPixel(cx-y,cy+x,color);
			LCD_PutPixel(cx+y,cy-x,color);
			LCD_PutPixel(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
}

/*********************************************************************
* Function: WORD LCD_Rectangle(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/

void LCD_Rectangle(u16 left, u16 top, u16 right, u16 bottom, u16 color, bool fill)
{
	register u16  x,y;


  if (!fill) {
    LCD_Line(left,top,left,bottom,color);
    LCD_Line(left,bottom,right,bottom,color);
    LCD_Line(right,bottom,right,top,color);
    LCD_Line(right,top,left,top,color);
  
  } else { // Filled Rectangle
	if(_clipRgn){
		if(left<_clipLeft)
			left = _clipLeft;
		if(right>_clipRight)
			right= _clipRight;
		if(top<_clipTop)
			top = _clipTop;
		if(bottom>_clipBottom)
			bottom = _clipBottom;
	}

	LCD_SetArea(left,top,right,bottom);
	LCD_WriteCommand(CMD_WR_MEMSTART);

	Clr_Cs;
	for(y=top; y<bottom+1; y++){
		for(x=left; x<right+1; x++){
			LCD_WriteData(color);
		}
	}
	Set_Cs;
  }
}

void LCD_Square(u16 x0, u16 y0, u16 width, u16 color,u8 fill)
{
	LCD_Rectangle(x0, y0, x0+width, y0+width, color,fill);
}

void LCD_ClearCharBox(u16 x,u16 y,u16 color)
{
	LCD_Rectangle(x*8,y*16,x*8+8,y*16+16,color,true); 
}

void LCD_DispPic_FullSize(const unsigned char *str)
{

	u32 temp;
	ColorTypeDef color;

	LCD_SetArea(0,0,GetMaxX(),GetMaxY());
	LCD_WriteCommand(CMD_WR_MEMSTART);

	Clr_Cs;
  
	for (temp = 2; temp < ((GetMaxX()+1)*(GetMaxY()+1))+2; temp++)
	{  
		color.U8[1] =*(unsigned short *)(&str[ 2 * temp]);
		color.U8[0]=*(unsigned short *)(&str[ 2 * temp+1]);
		//DataToWrite(i);
	
		LCD_WriteData(color.U16);
	}
    Set_Cs;

//==============================  
}

void LCD_DispPic(u16 x0, u16 y0, const unsigned char *str)
{

	u32 temp;
    u32  i;
    u32 difference;
    
    u16 x1;
    u16 y1;
    u16 imageWidth;
    u16 imageHeight;
    
    ColorTypeDef color;
    
    color.U8[1] =*(unsigned short *)(&str[ 0]);
	color.U8[0]=*(unsigned short *)(&str[ 1]);
    imageWidth = color.U16;
    x1 = imageWidth-1 + x0;

    color.U8[1] =*(unsigned short *)(&str[ 2]);
	color.U8[0]=*(unsigned short *)(&str[ 3]);
    imageHeight = color.U16;
    y1 = imageHeight-1 + y0;
    
	LCD_SetArea(x0,y0,x1,y1);
	LCD_WriteCommand(CMD_WR_MEMSTART);

	Clr_Cs;    

	for (temp = 2; temp < (imageWidth*imageHeight)+2; temp++)
	{  
		color.U8[1] =*(unsigned short *)(&str[ 2 * temp]);
		color.U8[0]=*(unsigned short *)(&str[ 2 * temp+1]);
		//DataToWrite(i);
	
		LCD_WriteData(color.U16);
	}
    Set_Cs;

//==============================  
}


int power (int base, int n) {
    int     i,
            p;
    p = 1;
    for (i = 1; i <= n; ++i)
	p *= base;
    return p;
}

void LCD_PutChar(u16 x, u16 y, u8 Character, u16 ForeColor, u16 BackColor)
        {
        int row = 0;
        int column = 0;
        int offset = (Character - 32) * MyFont[0];
        
        LCD_SetArea(x, y, x+MyFont[1]-1, y+MyFont[2]-1);
        LCD_WriteCommand(CMD_WR_MEMSTART);
        Clr_Cs;        

            for (row = 0; row < MyFont[2]; row++)
            {
                for (column = 0; column < MyFont[1]; column++)
                {
                    if (row == 0)
                    {
                        if ((MyFont[2 * column + 1 +3+offset] & 1) > 0)
                        {
                            LCD_WriteData(ForeColor);
                        } else {
                            LCD_WriteData(BackColor);
                        }
                    }

                    if (row > 0 && row < 8)
                    {
                        if ((MyFont[2 * column + 1 + 3+offset] & (power(2, row))) > 0)  // Math.Pow = ^ (opløftet i)
                        {
                            LCD_WriteData(ForeColor);
                        } else {
                            LCD_WriteData(BackColor);
                        }
                    }

                    if (row == 8)
                    {
                        if ((MyFont[2 * column + 2 + 3+offset] & 1) > 0)
                        {
                            LCD_WriteData(ForeColor);
                        } else {
                            LCD_WriteData(BackColor);
                        }
                    }

                    if (row > 8 && row < 16)
                    {
                        if ((MyFont[2 * column + 2 + 3+offset] & (power(2, (row - 8)))) > 0)  // Math.Pow = ^ (opløftet i)
                        {
                            LCD_WriteData(ForeColor);
                        } else {
                            LCD_WriteData(BackColor);
                        }
                    }

                }
            }
        Set_Cs;
}    

void LCD_Text(u16 x, u16 y, u8 *str, u16 len, u16 ForeColor, u16 BackColor)
{
  u8 i;
  
  for (i=0;i<len;i++)
  {
    LCD_PutChar((x+MyFont[1]*i),y,*str++,ForeColor,BackColor);
  }
}