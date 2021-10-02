#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------
#include "Globals.h"
#include "TFT_Touch.h"   // Подключение тачсрина
#include <GxTFT.h>         // Hardware-specific library // Настройки дисплея
#include <GxIO/STM32GENERIC/GxIO_STM32F4_FSMC/GxIO_STM32F4_FSMC.h>  // Версия для подключения FSMC "BLACK F407VE/ZE/ZG boards".

#if (TFT_DISPLAY_TYPE == 2) // 4 ''
  #include <GxCTRL/GxCTRL_NT35510/GxCTRL_NT35510.h> // 480х800  Подключение информации по настройке драйвера дисплея NT35510
#elif (TFT_DISPLAY_TYPE == 1) // 7 ''
  #include <GxCTRL/GxCTRL_SSD1963/GxCTRL_SSD1963.h> // 480х800  Подключение информации по настройке драйвера дисплея SSD1963
#else
  #error "UNKNOWN TFT DISPLAY !!!"
#endif
//--------------------------------------------------------------------------------------------------------------------------------------

#include "Fonts/GFXFF/gfxfont.h"
//#include "RusFont.h"
#include "BigRusFont.h"
#include "SmallRusFont.h"
#include "IconsFont.h"
#include "SensorFont.h"
#include "SensorFont2.h"
#include "SevenSegNumFontMDS.h"
#include "SevenSegNumFontPlus.h"
#include "Various_Symbols_32x32.h"

#define FONTTYPE const GFXfont*
//--------------------------------------------------------------------------------------------------------------------------------------
#define TFT_Class GxTFT         // класс поддержки TFT
#define TOUCH_Class MyTFTTouch // класс поддержки тача

//#define TFT_FONT (&keyrus) // какой шрифт юзаем
#define TFT_FONT (&BigRusFont) // какой шрифт юзаем
#define TFT_SMALL_FONT (&SmallRusFont) // какой шрифт юзаем
#define SENSOR_FONT (&SensorFont)
#define SENSOR_FONT2 (&SensorFont2)
#define SEVEN_SEG_NUM_FONT_MDS (&SevenSegNumFontMDS)
#define SEVEN_SEG_NUM_FONT_PLUS (&SevenSegNumFontPlus)
#define VARIOUS_SYMBOLS_32x32 (&Various_Symbols_32x32)
#define ICONS_FONT (&IconsFont)
