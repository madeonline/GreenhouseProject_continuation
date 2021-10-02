#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include "Globals.h"
#include "TFT_Includes.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// класс поддержки русских шрифтов для UTFT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  charUnknown,

  // SensorFont
  charMinus,
  charPlus,
  charPercent,
  charDegree,
  charLux,
  charDot,
  charComma,
  charRightArrow,
  charLeftArrow,
  charWindSpeed,

  // SensorFont2
  charLitres,
  charM3,
  charEast,
  charWest,
  charNorth,
  charSouth,
  charCO2,
  
} TFTSpecialSimbol;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTRus
{
  public:
    TFTRus();
    void init(TFT_Class* uTft);

//    int print(const char* str,int x, int y, word bgColor=TFT_BACK_COLOR, word fgColor=TFT_FONT_COLOR);
    int print(const char* str,int x, int y, word bgColor=TFT_BACK_COLOR, word fgColor=TFT_FONT_COLOR, bool computeTextWidthOnly=false);
    int getStringLength(const char* str);
    int textWidth(const char* str);
    
    int printSpecialChar(TFTSpecialSimbol ch, int x, int y);
    char mapChar(TFTSpecialSimbol ch);
    
  private:
    TFT_Class* pDisplay;


  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
