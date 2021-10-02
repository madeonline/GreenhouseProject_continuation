
#ifndef TFT_LCD_H
#define TFT_LCD_H

//#include "stm32f4xx.h"
#include "fonts.h"    	  // шрифт

//#include "correct_ANSI.h" // коррекция выкинутых мною символов между рус. и ингл.

// для запуска дисплея в режиме 16 бит, парраллельная шина, необходимо на ножки дисплея подать IM3=0, IM2=0, IM1=0, IM0=1
// шина данных дисплея 16 бит
// подключение выводов: Микроконтроллер <-> Дисплей
// GPIO D0  <-> LCD D2
// GPIO D1  <-> LCD D3
// GPIO D4  <-> LCD RD
// GPIO D5  <-> LCD WR
// GPIO D7  <-> LCD CS
// GPIO D8  <-> LCD D13
// GPIO D9  <-> LCD D14
// GPIO D10 <-> LCD D15
// GPIO D13 <-> LCD RS
// GPIO D14 <-> LCD D0
// GPIO D15 <-> LCD D1
// GPIO E7  <-> LCD D4
// GPIO E8  <-> LCD D5
// GPIO E9  <-> LCD D6
// GPIO E10 <-> LCD D7
// GPIO E11 <-> LCD D8
// GPIO E12 <-> LCD D9
// GPIO E13 <-> LCD D10
// GPIO E14 <-> LCD D11
// GPIO E15 <-> LCD D12
// выводы дисплея: TE, VSYNC, HSYNC, ENABLE, DOTCLK, SDA не используются

//#define LCD_PORT GPIOC         // порт, к которому подключены управляющие выводы дисплея
//#define LCD_RST GPIO_Pin_4     // PC4, вывод RST (reset) дисплея (0 дисплей в состоянии сброса, 1 рабочий режим)

//#define GPIO_HIGH(a, b) a -> BSRRL = b  // теперь при помощи конструкции GPIO_HIGH(a, b) можно выдать лог.1 на порт "a" пин "b"
//#define GPIO_LOW(a, b) a -> BSRRH = b   // теперь при помощи конструкции GPIO_LOW(a, b) можно выдать лог.0 на порт "a" пин "b"

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
#define BLUE2			      0x051D
#define GREEN           0x07E0      /*   0, 255,   0 */
#define GREEN2		      0xB723
#define GREEN3		      0x8000
#define CYAN            0x07FF      /*   0, 255, 255 */
#define RED             0xF800      /* 255,   0,   0 */
#define MAGENTA         0xF81F      /* 255,   0, 255 */
#define YELLOW          0xFFE0      /* 255, 255,   0 */
#define WHITE           0xFFFF      /* 255, 255, 255 */
#define ORANGE          0xFD20      /* 255, 165,   0 */
#define GREENYELLOW     0xAFE5      /* 173, 255,  47 */
#define BROWN 			    0XBC40 //
#define BRRED 			    0XFC07 //

//#define LCD_LED_ON  GPIO_SetBits(GPIOB, 1);
//#define LCD_LED_OFF GPIO_ResetBits(GPIOB, 1);

#define ILI9341_WIDTH       240
#define ILI9341_HEIGHT      320
#define ILI9341_PIXEL_COUNT	ILI9341_WIDTH * ILI9341_HEIGHT

// (страница 127 даташит)
// для выбора ставим где надо 1 и не забываем поставить 0 вместо 1, где не надо ;-)
#define Dspl_Rotation_0_degr   1 // поворот дисплея, сколько градусов 01001000 портрет
#define Dspl_Rotation_90_degr  0 // поворот дисплея, сколько градусов 00101000 альбом
#define Dspl_Rotation_180_degr 0 // поворот дисплея, сколько градусов 10001000 портрет, но снизу вверх
#define Dspl_Rotation_270_degr 0 // поворот дисплея, сколько градусов 11101000 

//стр.155 даташит
#define Fram_Rate_61Hz  0 // частота кадров (70Hz по умолч) 
#define Fram_Rate_70Hz  1 // частота кадров 
#define Fram_Rate_90Hz  0 // частота кадров 
#define Fram_Rate_100Hz 0 // частота кадров 
#define Fram_Rate_119Hz 0 // частота кадров 

//Commands
#define ILI9341_RESET			 	1   //0x0001
#define ILI9341_SLEEP_OUT		  	0x0011
#define ILI9341_GAMMA			    0x0026
#define ILI9341_DISPLAY_OFF			0x0028
#define ILI9341_DISPLAY_ON			0x0029
#define ILI9341_COLUMN_ADDR			0x002A
#define ILI9341_PAGE_ADDR			0x002B
#define ILI9341_GRAM				0x002C
#define ILI9341_TEARING_OFF			0x0034
#define ILI9341_TEARING_ON			0x0035
#define ILI9341_DISPLAY_INVERSION			0x00b4
#define ILI9341_MAC			        0x0036
#define ILI9341_PIXEL_FORMAT    0x003A
#define ILI9341_WDB			    	  0x0051
#define ILI9341_WCD				      0x0053
#define ILI9341_RGB_INTERFACE   0x00B0
#define ILI9341_FRC					    0x00B1
#define ILI9341_BPC					    0x00B5
#define ILI9341_DFC				 	    0x00B6
#define ILI9341_Entry_Mode_Set	0x00B7
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

/**
 * Select font
 */
extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;

/**
 * Public enum
 *
 * Select orientation for ILI9341
 */
typedef enum {
	ILI9341_Orientation_Portrait_1,
	ILI9341_Orientation_Portrait_2,
	ILI9341_Orientation_Landscape_1,
	ILI9341_Orientation_Landscape_2
} ILI9341_Orientation_t;

/**
 * Orientation
 * Used private
 */
typedef enum {
	ILI9341_Landscape,
	ILI9341_Portrait
} ILI9341_Orientation;

/**
 * ILI9341 options
 * Used private
 */
typedef struct {
	uint16_t Width;
	uint16_t Height;
	ILI9341_Orientation orientation; // 1 = portrait; 0 = landscape
} ILI9341_Options_t;





class TFT_ILI9341_NEW
{
private:

	//void ILI9341_SendCommand(uint16_t com);

public:
	void ILI9341_Init();
	//void ILI9341_SendCommand(uint16_t com);
	//void ILI9341_SendData(uint16_t data);

	//void ILI9341_Putc(uint16_t x, uint16_t y, char c, FontDef_t *font, uint16_t foreground, uint16_t background);// Put single character to ILI9341
	//void ILI9341_Puts(uint16_t x, uint16_t y, char *str, FontDef_t *font, uint16_t foreground, uint16_t background);// Put string to ILI9341
	//void ILI9341_GetStringSize(char *str, FontDef_t *font, uint16_t *width, uint16_t *height);// Get width and height of box with text
	//void ILI9341_WriteString(unsigned char x0, unsigned int y0, unsigned char *s, unsigned int color, unsigned int back_color); // русский поддерживает

	/**
	 * 	- ILI9341_Orientation_t orientation
	 *		- ILI9341_Orientation_Portrait_1: no rotation
	 *		- ILI9341_Orientation_Portrait_2: rotate 180deg
	 *		- ILI9341_Orientation_Landscape_1: rotate 90deg
	 *		- ILI9341_Orientation_Landscape_2: rotate -90deg
	 */
	void ILI9341_Rotate(ILI9341_Orientation_t orientation); // Rotate ILI9341

	void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color); //Draw single pixel to ILI9341
	void ILI9341_Fill(uint16_t color); //Fill entire ILI9341 with color
	void ILI9341_Fill_Rect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, uint16_t color);
	//void ILI9341_Fill_String(unsigned int y, uint16_t color);
	void ILI9341_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void ILI9341_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);// Draw line to ILI9341
	void ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);// Draw rectangle on ILI9341
	void ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);// Draw filled rectangle on ILI9341
	void ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);// Draw circle on ILI9341
	void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	//void ILI9341_DrawRange(int16_t X1, int16_t Y1, int16_t R, uint16_t color);
	void ILI9341_IMAGE(uint16_t x11, uint16_t y11, uint16_t size_x, uint16_t size_y, uint16_t *color);

	uint16_t LCD_GetXSize();
	uint16_t LCD_GetYSize();

};
//extern TFT_ILI9341_NEW TFT_ILI9341_NEW1;

#endif
