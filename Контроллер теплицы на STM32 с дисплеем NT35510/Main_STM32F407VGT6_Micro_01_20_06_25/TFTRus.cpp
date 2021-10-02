//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "TFTRus.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int utf8_strlen(const char* str) // возвращает длину в символах строки в кодировке UTF-8
{
    int len = strlen(str);
    
    int c,i,ix,q;
    for (q=0, i=0, ix=len; i < ix; i++, q++)
    {
        c = (unsigned char) str[i];
        if      (c>=0   && c<=127) i+=0;
        else if ((c & 0xE0) == 0xC0) i+=1;
        else if ((c & 0xF0) == 0xE0) i+=2;
        else if ((c & 0xF8) == 0xF0) i+=3;
        //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
        //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
        else return 0; //invalid utf8
    }
    return q;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int utf8_strlen(const String& str) // возвращает длину в символах строки в кодировке UTF-8
{   
    return utf8_strlen(str.c_str());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// индексы Ё и ё в массиве перекодировок
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define RUS_YO_BIG 64
#define RUS_YO_SMALL 65
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ПЕРВАЯ ВЕРСИЯ ТАБЛИЦЫ ПЕРЕКОДИРОВКИ //
const uint8_t utf8_rus_charmap[] PROGMEM = {
  'A', // А
  128, // Б
  'B', // В
  129, // Г
  130, // Д
  'E', // Е
  132, // Ж
  133, // З
  134, // И
  135, // Й
  'K', // К
  136, // Л
  'M', // М
  'H', // Н
  'O', // О
  137, // П
  'P', // Р
  'C', // С
  'T', // Т
  138, // У
  139, // Ф
  'X', // Х
  140, // Ц
  141, // Ч
  142, // Ш
  143, // Щ
  144, // Ъ
  145, // Ы
  146, // Ь
  147, // Э
  148, // Ю
  149, // Я
  
  'a', // а
  150, // б
  151, // в
  152, // г
  153, // д
  'e', // е
  155, // ж
  156, // з
  157, // и
  158, // й
  159, // к
  160, // л
  161, // м
  162, // н
  'o', // о
  163, // п
  
  'p', // р
  'c', // с
  164, // т
  'y', // у
  165, // ф
  'x', // х
  166, // ц
  167, // ч
  168, // ш
  169, // щ
  170, // ъ
  171, // ы
  172, // ь
  173, // э
  174, // ю
  175, // я
  
  'E', // Ё
  154, // ё
  };
 
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTRus
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTRus::TFTRus()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTRus::init(TFT_Class* uTft)
{
   pDisplay = uTft;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TFTRus::getStringLength(const char* str)
{
  return utf8_strlen(str);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TFTRus::printSpecialChar(TFTSpecialSimbol specialSymbol, int x, int y)
{
  unsigned int iCh = mapChar(specialSymbol);
  
  if(iCh == -1)
  {
    return 0;
  }

  //pDisplay->drawChar(iCh, x, y, 1);
  char buff[2] = {iCh,0};  
  pDisplay->drawString(buff, x, y, 1);
  
   yield();

  return textWidth(buff);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char TFTRus::mapChar(TFTSpecialSimbol ch)
{
  switch(ch)
  {
    case charComma: // ,
      return '5';
      
    case charDot: // .
      return '3';

    case charMinus: // -
      return '4';

    case charPlus: // +
      return '2';

    case charPercent: // PERCENT
      return '0';

    case charLux: // LUX
      return '1';

   case charDegree: // °
    return '6';

   case charRightArrow: // >
    return '7';

   case charLeftArrow: // <
    return '8';

   case charWindSpeed: // м/с
    return '9';

    case charLitres: // л
      return '0';

    case charM3: // кубометры
      return '1';

    case charEast:
      return '2';

    case charWest:
      return '3';

    case charNorth:
      return '4';

    case charSouth:
      return '5';

    case charCO2:
      return '6';

      
   case charUnknown:
    return -1;

  }

  return -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TFTRus::textWidth(const char* str)
{
  return print(str,0,0,BLACK,BLACK,true);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TFTRus::print(const char* st,int x, int y, word bgColor, word fgColor, bool computeTextWidthOnly)
{ 
  if(!st)
  {
    return 0; 
  }
  
 int stl, i, tw = 0;
 stl = strlen(st);

  if(!computeTextWidthOnly)
  {
    pDisplay->setTextColor(fgColor,  bgColor);
  }
  
 // y += pDisplay->fontHeight(1);
  
  uint8_t utf_high_byte = 0;
  unsigned int ch;

  String buff;
  buff.reserve(stl);

for (i = 0; i < stl; i++) 
  {
    ch = st[i];
    
    if ( ch >= 128) 
    {
      if ( utf_high_byte == 0 && (ch ==0xD0 || ch == 0xD1)) 
      {
        utf_high_byte = ch;
        continue;
      } 
      else 
      {
        if ( utf_high_byte == 0xD0) 
        {
          if (ch == 0x81) 
          { 
            //Ё
            ch = RUS_YO_BIG; // индекс буквы Ё в массиве перекодировки
          } 
          else 
          {
            if(ch <= 0xAF)  // [А-Я]
            {
              ch -= 0x90;
            } 
            else if( ch <= 0xBF) // [а-п]
            {
              ch -= 0x90;
            } 
            else 
            {
              // ch -= (0x90 - 1);
            }
          }
          
          ch = pgm_read_byte((utf8_rus_charmap + ch));
        
        } 
        else if (utf_high_byte == 0xD1) 
        {
          if (ch == 0x91) 
          {
            //ё
            ch = RUS_YO_SMALL; // индекс буквы ё в массиве перекодировки
          } 
          else   // [р-я]
          {
            ch -= 0x80;
            ch += 48;
          }
          
          ch = pgm_read_byte((utf8_rus_charmap + ch));
        }
        
        utf_high_byte = 0;
      }
    } 
    else 
    {
      utf_high_byte = 0;
    }

    buff += char(ch);
  } // for 

  if(computeTextWidthOnly)
  {
    return pDisplay->textWidth(buff,1);
  }
  else
  {
    return pDisplay->drawString(buff,x,y,1);
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
