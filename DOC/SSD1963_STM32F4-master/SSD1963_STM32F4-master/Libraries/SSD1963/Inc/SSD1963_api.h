#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdint.h>
u16 Lcd_Color565(u32 RGB);  // RGB颜色转为16位(565)
typedef union
{
  u16 U16;
  u8 U8[2];
}ColorTypeDef;

void LCD_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor);
void LCD_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
void LCD_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill); 
void LCD_Rectangle(u16 left, u16 top, u16 right, u16 bottom, u16 color, bool fill);
void LCD_Square(u16 x0, u16 y0, u16 width, u16 color,u8 fill);
void LCD_ClearCharBox(u16 x,u16 y,u16 color);

void LCD_DispPic_FullSize(const unsigned char *str);
void LCD_DispPic(u16 x0, u16 y0, const unsigned char *str);

int power (int m, int n);