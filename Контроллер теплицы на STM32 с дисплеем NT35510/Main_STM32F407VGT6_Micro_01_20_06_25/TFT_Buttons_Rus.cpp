//--------------------------------------------------------------------------------------------------------------------------------------
#include "TFT_Buttons_Rus.h"
#include "TFTMenu.h"
//--------------------------------------------------------------------------------------------------------------------------------------
TFT_Buttons_Rus::TFT_Buttons_Rus(TFT_Class *ptrUTFT, TOUCH_Class *ptrURTouch, TFTRus* pTFTRus, int count_btns)
{
	_UTFT = ptrUTFT;
	_URTouch = ptrURTouch;
  pRusPrinter = pTFTRus;

  countButtons = count_btns;
  buttons = new button_type[count_btns];
  
	deleteAllButtons();
	_color_text				= WHITE;
	_color_text_inactive	= DGRAY;
	_color_background		= BLUE;
	_color_border			= WHITE;
	_color_hilite			= RED;
	_font_text				= NULL;
	_font_symbol			= NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
TFT_Buttons_Rus::~TFT_Buttons_Rus()
{
  delete [] buttons;
}
//--------------------------------------------------------------------------------------------------------------------------------------
word TFT_Buttons_Rus::getButtonBackColor(int buttonID)
{
   if(buttonID < 0)
    return 0;

  return buttons[buttonID].backColor;
}
//--------------------------------------------------------------------------------------------------------------------------------------
word TFT_Buttons_Rus::getButtonFontColor(int buttonID)
{
   if(buttonID < 0)
    return 0;

  return buttons[buttonID].fontColor;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setButtonBackColor(int buttonID, word color)
{
   if(buttonID < 0 || buttonID >= countButtons)
   {
    return;
   }

    buttons[buttonID].backColor = color;
    buttons[buttonID].flags |= BUTTON_HAS_BACK_COLOR;
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::selectButton(int buttonID, bool selected, boolean redraw)
{
   if(buttonID < 0 || buttonID >= countButtons)
   {
    return;
   }

    if(selected)
    {
      buttons[buttonID].flags |= BUTTON_SELECTED;
    }
    else
    {
      buttons[buttonID].flags &= ~BUTTON_SELECTED;
    }

    if (redraw)
    {
      drawButton(buttonID);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setButtonFontColor(int buttonID, word cl)
{
  if(buttonID < 0 || buttonID >= countButtons)
  {
    return;
  }

  buttons[buttonID].fontColor = cl;
  buttons[buttonID].flags |= BUTTON_HAS_FONT_COLOR;
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
int TFT_Buttons_Rus::addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *label, uint16_t flags)
{
	int btcnt = 0;
  
	while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) and (btcnt<countButtons))
		btcnt++;
  
	if (btcnt == countButtons)
		return -1;
	else
	{
		buttons[btcnt].pos_x  = x;
		buttons[btcnt].pos_y  = y;
		buttons[btcnt].width  = width;
		buttons[btcnt].height = height;
		buttons[btcnt].flags  = flags | BUTTON_VISIBLE;
		buttons[btcnt].label  = label;
//		buttons[btcnt].data   = NULL;
		return btcnt;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::hideButton(int buttonID, boolean redraw)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return;

if (!(buttons[buttonID].flags & BUTTON_UNUSED))
  {
    buttons[buttonID].flags = buttons[buttonID].flags & ~BUTTON_VISIBLE;
    if (redraw)
      drawButton(buttonID);
  }  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::showButton(int buttonID, boolean redraw)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return;

if (!(buttons[buttonID].flags & BUTTON_UNUSED))
  {
    buttons[buttonID].flags = buttons[buttonID].flags | BUTTON_VISIBLE;
    if (redraw)
      drawButton(buttonID);
  }  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::drawButtons(DrawButtonsUpdateFunc func)
{
  
	for (int i=0;i<countButtons;i++)
	{
		if ((buttons[i].flags & BUTTON_UNUSED) == 0)
    {
//Serial.print("Draw button #"); Serial.println(i); Serial.flush();
			drawButton(i);
      
      if(func)
        func();

     yield();
     
     
    }
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setIconFont(FONTTYPE font)
{
  _font_icon = font;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setButtonHasIcon(int buttonID)
{
   if(buttonID < 0 || buttonID >= countButtons)
    return;

    buttons[buttonID].flags |= BUTTON_HAS_ICON;
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::drawButton(int buttonID)
{  

	if (buttonID < 0 || buttonID >= countButtons)
		return;

//Serial.print("button #"); Serial.print(buttonID); Serial.print("; total buttons avail: "); Serial.println(countButtons); Serial.flush();
//Serial.println("1"); Serial.flush();

	int		text_x, text_y;
  if (!(buttons[buttonID].flags & BUTTON_VISIBLE))
  {
    _UTFT->fillRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, TFT_BACK_COLOR);    
    return;
  }

//Serial.println("2"); Serial.flush();

    uint16_t bkColor = _color_background;
  

    if(buttons[buttonID].flags & BUTTON_HAS_BACK_COLOR && !(buttons[buttonID].flags & BUTTON_DISABLED))
       bkColor = buttons[buttonID].backColor;
    else
       bkColor = _color_background;
     

    _UTFT->fillRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, 2, bkColor);
    yield();

//Serial.println("3"); Serial.flush();


    if(buttons[buttonID].flags & BUTTON_SELECTED)
      bkColor = _color_hilite;
    else
      bkColor = _color_border;

    _UTFT->drawRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, 2, bkColor);

    
    yield();

//Serial.println("4"); Serial.flush();

    uint16_t textColor;
    FONTTYPE curFont = _font_text;


    if (buttons[buttonID].flags & BUTTON_DISABLED)
    {
      textColor = _color_text_inactive;
    }
    else
    {
      if (buttons[buttonID].flags & BUTTON_HAS_FONT_COLOR)
      {
       textColor = buttons[buttonID].fontColor;
      }
      else
      {
        textColor = _color_text;
      }
    }

    const char* label = buttons[buttonID].label;
    int icon_x = 0;
    int icon_y = 0;    
 
    if (buttons[buttonID].flags & BUTTON_SYMBOL)
    {
      _UTFT->setFreeFont(_font_symbol);
      text_x = (buttons[buttonID].width/2) - (pRusPrinter->textWidth(buttons[buttonID].label)/2) + buttons[buttonID].pos_x;
      text_y = (buttons[buttonID].height/2) - (_UTFT->fontHeight(1)/2) + buttons[buttonID].pos_y;
    }
    else
    {
        if (buttons[buttonID].flags & BUTTON_HAS_FONT)
        {
//Serial.print("button HAS font: ");
//Serial.println((uint32_t)(buttons[buttonID].font));
//Serial.println((uint32_t)TFT_FONT);

          _UTFT->setFreeFont(buttons[buttonID].font);
          curFont = buttons[buttonID].font;
        }
        else
        {
          _UTFT->setFreeFont(_font_text);
        }

        if((buttons[buttonID].flags & BUTTON_HAS_ICON) && _font_icon)
        {
          label++;
        }        

      /*
      text_x = ((buttons[buttonID].width/2) - (pRusPrinter->textWidth(buttons[buttonID].label)/2)) + buttons[buttonID].pos_x;
      text_y = (buttons[buttonID].height/2) - (_UTFT->fontHeight(1)/2) + buttons[buttonID].pos_y;
      */

      int labelWidth = pRusPrinter->textWidth(label);
      int fontHeight = _UTFT->fontHeight(1);
      text_x = ((buttons[buttonID].width/2) - (labelWidth/2)) + buttons[buttonID].pos_x;
      text_y = (buttons[buttonID].height/2) - (fontHeight/2) + buttons[buttonID].pos_y;

      if((buttons[buttonID].flags & BUTTON_HAS_ICON) && _font_icon)
      {
        _UTFT->setFreeFont(_font_icon);

        char lbl[2] = {0};
        lbl[0] = buttons[buttonID].label[0];

        int iconWidth = pRusPrinter->textWidth(lbl);
        
        icon_x = text_x - iconWidth/2;
        text_x += iconWidth/2;
        icon_y = (buttons[buttonID].height/2) - (_UTFT->fontHeight(1)/2) + buttons[buttonID].pos_y;

        _UTFT->setFreeFont(curFont);
      }      
      
    } // else text label

//Serial.println("5"); Serial.flush();

    if(buttons[buttonID].flags & BUTTON_HAS_BACK_COLOR && !(buttons[buttonID].flags & BUTTON_DISABLED))
    {
      bkColor = buttons[buttonID].backColor;
    }
    else
    {
      bkColor = _color_background;      
    }
      
     pRusPrinter->print(label, text_x, text_y, bkColor, textColor);

    if((buttons[buttonID].flags & BUTTON_HAS_ICON) && _font_icon)
    {
      _UTFT->setFreeFont(_font_icon);

      char icon[2] = {0};
      icon[0] = buttons[buttonID].label[0];
      pRusPrinter->print(icon, icon_x, icon_y, bkColor, textColor);

      _UTFT->setFreeFont(curFont);
    }     

//Serial.println("DONE"); Serial.flush();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setButtonFont(int buttonID, FONTTYPE someFont)
{
    if(buttonID < 0)
    {
      return;
    }

   // Serial.print("Set font for button #"); Serial.print(buttonID); Serial.print("; font are: "); Serial.print((uint32_t)someFont);

    buttons[buttonID].font = someFont;
    buttons[buttonID].flags |= BUTTON_HAS_FONT; 

   // Serial.print("; after assignment: "); Serial.println((uint32_t)buttons[buttonID].font);Serial.flush();
   // Serial.print("TFT_FONT ARE: "); Serial.println((uint32_t)TFT_FONT);Serial.flush();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::enableButton(int buttonID, boolean redraw)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return;

	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags & ~BUTTON_DISABLED;
		if (redraw)
			drawButton(buttonID);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::disableButton(int buttonID, boolean redraw)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return;

	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags | BUTTON_DISABLED;
		if (redraw)
			drawButton(buttonID);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* TFT_Buttons_Rus::getLabel(int buttonID)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return "";

  if (!(buttons[buttonID].flags & BUTTON_UNUSED))
  {
    return buttons[buttonID].label;
  }  
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::relabelButton(int buttonID, const char *label, boolean redraw)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return;

	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].label = label;
		if (redraw)
			drawButton(buttonID);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
boolean TFT_Buttons_Rus::buttonEnabled(int buttonID)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return false;

	return !(buttons[buttonID].flags & BUTTON_DISABLED);
}
//-------------------------------------------------------------------------------------------------------------------------------------- 
void TFT_Buttons_Rus::deleteButton(int buttonID)
{
	if (buttonID < 0 || buttonID >= countButtons)
		return;

	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
		buttons[buttonID].flags = BUTTON_UNUSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::deleteAllButtons()
{
	for (int i=0;i<countButtons;i++)
	{
		buttons[i].pos_x=0;
		buttons[i].pos_y=0;
		buttons[i].width=0;
		buttons[i].height=0;
		buttons[i].flags=BUTTON_UNUSED;
    buttons[i].font = NULL;
		buttons[i].label="";
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
int TFT_Buttons_Rus::checkButtons(OnCheckButtonsFunc pressed, OnCheckButtonsFunc released)
{
  
    if (_URTouch->TouchPressed())
    {

      TS_Point p = _URTouch->getPoint();

		int		result = -1;
		int		touch_x = p.x;
		int		touch_y = p.y;

   // Serial.print("x: "); Serial.print(touch_x);
   // Serial.print(", y: "); Serial.println(touch_y);
      
		for (int i=0;i<countButtons;i++)
		{
			if (((buttons[i].flags & BUTTON_UNUSED) == 0) and ((buttons[i].flags & BUTTON_DISABLED) == 0) and ((buttons[i].flags & BUTTON_VISIBLE) ) and (result == -1))
			{
				if ((touch_x >= buttons[i].pos_x) and (touch_x <= (buttons[i].pos_x + buttons[i].width)) and (touch_y >= buttons[i].pos_y) and (touch_y <= (buttons[i].pos_y + buttons[i].height)))
					result = i;
			}
		}
		if (result != -1)
		{
			if (!(buttons[result].flags & BUTTON_NO_BORDER))
			{       
					_UTFT->drawRoundRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].width, buttons[result].height, 2, _color_hilite);
          yield();
			}
       if(pressed)
       {
        pressed(result);
       }
		}
   
		if (result != -1)
		{
      while (_URTouch->TouchPressed()) 
      {
        Ticker.tick(); 
        yield(); 
      }

      if(released)
      {
        released(result);
      }

			if (!(buttons[result].flags & BUTTON_NO_BORDER))
			{

        uint16_t cl;
				if(buttons[result].flags & BUTTON_SELECTED)
           cl = _color_hilite;
				else
				  cl = _color_border;
         
				_UTFT->drawRoundRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].width, buttons[result].height,2, cl);

        yield();
			}
		}
		return result;
	}
	else
		return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setTextFont(FONTTYPE font)
{
	_font_text = font;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setSymbolFont(FONTTYPE font)
{
	_font_symbol = font;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TFT_Buttons_Rus::setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back)
{
	_color_text				= atxt;
	_color_text_inactive	= iatxt;
	_color_background		= back;
	_color_border			= brd;
	_color_hilite			= brdhi;
}
//--------------------------------------------------------------------------------------------------------------------------------------
