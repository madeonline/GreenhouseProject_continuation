#pragma once


#include <Arduino.h>
#include "Globals.h"
#include "TFTRus.h"
#include "TFT_Includes.h"

#define MAX_BUTTONS	25	// Maximum number of buttons available at one time

// флаги состояний кнопок

#define BUTTON_DISABLED			1
#define BUTTON_SYMBOL			2
#define BUTTON_HAS_ICON   4
//#define BUTTON_SYMBOL_REP_3X	4
//#define BUTTON_BITMAP			8	
#define BUTTON_NO_BORDER		16
#define BUTTON_HAS_BACK_COLOR 32
#define BUTTON_HAS_FONT_COLOR 64
#define BUTTON_VISIBLE 128
#define BUTTON_SELECTED 256
#define BUTTON_HAS_FONT 512
#define BUTTON_UNUSED      0x8000


#pragma pack(push,1)
typedef struct
{
  int16_t			pos_x, pos_y, width, height;
  uint16_t backColor, fontColor;
  uint16_t			flags;
  const char				*label;
  FONTTYPE font;
  
} button_type;
#pragma pack(pop)

typedef void (*DrawButtonsUpdateFunc)(void);
typedef void (*OnCheckButtonsFunc)(int button);

class TFT_Buttons_Rus
{
	public:
		TFT_Buttons_Rus(TFT_Class *ptrUTFT, TOUCH_Class *ptrURTouch, TFTRus* pTFTRus, int count_btns = MAX_BUTTONS);
    ~TFT_Buttons_Rus();

		int		addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *label, uint16_t flags=0);
		void	drawButtons(DrawButtonsUpdateFunc func=NULL);
		void	drawButton(int buttonID);
		void	enableButton(int buttonID, boolean redraw=false);
		void	disableButton(int buttonID, boolean redraw=false);
		void	relabelButton(int buttonID, const char *label, boolean redraw=false);
		boolean	buttonEnabled(int buttonID);
		void	deleteButton(int buttonID);
		void	deleteAllButtons();
		int		checkButtons(OnCheckButtonsFunc pressed = NULL, OnCheckButtonsFunc released = NULL);
		void	setTextFont(FONTTYPE font);
		void	setSymbolFont(FONTTYPE font);
		void	setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back);
    void selectButton(int buttonID, bool selected, boolean redraw=false);
    
		word getButtonBackColor(int buttonID);
		void 	setButtonBackColor(int buttonID, word color);

   void setButtonFont(int buttonID, FONTTYPE font);
		word getButtonFontColor(int buttonID);
		void 	setButtonFontColor(int buttonID, word color);
   
    void showButton(int buttonID, boolean redraw=false);
    void hideButton(int buttonID, boolean redraw=false);
    const char* getLabel(int buttonID);

    void setIconFont(FONTTYPE font);
    void setButtonHasIcon(int buttonID);

	protected:
		TFT_Class		*_UTFT;
		TOUCH_Class		*_URTouch;

    TFTRus* pRusPrinter;
    
		button_type*	buttons;
    int countButtons;
    
		word		_color_text, _color_text_inactive, _color_background, _color_border, _color_hilite;
		FONTTYPE	_font_text;
		FONTTYPE _font_symbol;
    FONTTYPE _font_icon;
};
