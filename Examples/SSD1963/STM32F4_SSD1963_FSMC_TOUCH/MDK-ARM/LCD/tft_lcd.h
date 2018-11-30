#ifndef __TFT_LCD_H
#define __TFT_LCD_H
#include "stm32f4xx.h"

//#include "font.h"	

#define TFT_RST_SET       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);         // 1 на вывод reset дисплея
#define TFT_RST_RESET     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);       // 0 на вывод reset
											

/*******************************************************************/
// Определяем адреса, по которым будем записывать данные
   
// Для записи команд
#define CMD_ADR   		  	    (*(uint16_t *)0x60000000)
// Для записи данных
#define DAT_ADR    			      (*(uint16_t *)0x60020000) 

#define LCD_WIDTH       480
#define LCD_HEIGHT      800
#define LCD_PIXEL_COUNT	LCD_WIDTH * LCD_HEIGHT

extern char array [100]; 

void LCD_SendCommand(uint16_t com);
void LCD_SendData(uint16_t data);
void LCD_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);


extern void LCD_ini(void);
void bright_PWM_ssd1963(uint8_t bright);
extern void LCD_Clear( uint16_t color);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawString(uint32_t x, uint32_t y,char *str);
void WriteChar(unsigned char x0, unsigned char y0, unsigned char s, uint16_t color, uint16_t back_color);
void WriteString(unsigned char x0,unsigned int y0, char *s,unsigned int color);//, unsigned int back_color)
void LCD_DrawPoint(uint16_t ysta, uint16_t xsta, uint16_t color);
void LCD_DrawVLine(uint16_t xsta, uint16_t ysta, uint16_t yend, uint16_t color);
void LCD_DrawHLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t color);
void LCD_DrawLine(uint16_t ysta, uint16_t xsta, uint16_t yend, uint16_t xend, uint16_t color);



//Commands
#define ILI9341_RESET			 		  0x0001
#define ILI9341_SLEEP_OUT		  	0x0011
#define ILI9341_GAMMA			    	0x0026
#define ILI9341_DISPLAY_OFF			0x0028
#define ILI9341_DISPLAY_ON			0x0029
#define ILI9341_COLUMN_ADDR			0x002A
#define ILI9341_PAGE_ADDR			  0x002B
#define ILI9341_GRAM				    0x002C
#define ILI9341_MAC			        0x0036
#define ILI9341_PIXEL_FORMAT    0x003A
#define ILI9341_WDB			    	  0x0051
#define ILI9341_WCD				      0x0053
#define ILI9341_RGB_INTERFACE   0x00B0
#define ILI9341_FRC					    0x00B1
#define ILI9341_BPC					    0x00B5
#define ILI9341_DFC				 	    0x00B6
#define ILI9341_POWER1					0x00C0
#define ILI9341_POWER2					0x00C1
#define ILI9341_VCOM1						0x00C5
#define ILI9341_VCOM2						0x00C7
#define ILI9341_POWERA					0x00CB
#define ILI9341_POWERB					0x00CF
#define ILI9341_PGAMMA					0x00E0
#define ILI9341_NGAMMA					0x00E1
#define ILI9341_DTCA						0x00E8
#define ILI9341_DTCB						0x00EA
#define ILI9341_POWER_SEQ				0x00ED
#define ILI9341_3GAMMA_EN				0x00F2
#define ILI9341_INTERFACE				0x00F6
#define ILI9341_PRC				   	  0x00F7
#define ILI9341_VERTICAL_SCROLL 0x0033 






/*------------Замена цвета---------------*/
                                              
#define BLACK           0x0000      /*   0,   0,   0 */
#define NAVY            0x000F      /*   0,   0, 128 */
#define DGREEN          0x03E0      /*   0, 128,   0 */
#define DCYAN           0x03EF      /*   0, 128, 128 */
#define MAROON          0x7800      /* 128,   0,   0 */
#define PURPLE          0x780F      /* 128,   0, 128 */
#define OLIVE           0x7BE0      /* 128, 128,   0 */
#define LGRAY           0xC618      /* 192, 192, 192 */
#define DGRAY           0x7BEF      /* 128, 128, 128 */
#define BLUE            0x001F      /*   0,   0, 255 */
#define GREEN           0x07E0      /*   0, 255,   0 */
#define CYAN            0x07FF      /*   0, 255, 255 */
#define RED             0xF800      /* 255,   0,   0 */
#define MAGENTA         0xF81F      /* 255,   0, 255 */
#define YELLOW          0xFFE0      /* 255, 255,   0 */
#define WHITE           0xFFFF      /* 255, 255, 255 */
#define ORANGE          0xFD20      /* 255, 165,   0 */
#define GREENYELLOW     0xAFE5      /* 173, 255,  47 */
#define BROWN 			     0XBC40 //
#define BRRED 			     0XFC07 //



#endif /* __TFT_LCD_H */
