#ifndef _UTFTMENU_H
#define _UTFTMENU_H
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "TinyVector.h"
#include "TFTSettings.h"

#include <UTFT.h>
#include <URTouchCD.h>
#include <URTouch.h>
#include "UTFT_Buttons_Rus.h"
#include "UTFTRus.h"
#include "Drawing.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTMenu;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// абстрактный класс экрана для TFT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AbstractTFTScreen
{
  public:

    void setup(TFTMenu* menu);
    void update(TFTMenu* menu);
    void draw(TFTMenu* menu);

    const char* getName() {return screenName;}
    bool isActive() {return isActiveScreen; }
    void setActive(bool val){ isActiveScreen = val; }
  
    AbstractTFTScreen(const char* name);
    virtual ~AbstractTFTScreen();

  protected:

    virtual void doSetup(TFTMenu* menu) = 0;
    virtual void doUpdate(TFTMenu* menu) = 0;
    virtual void doDraw(TFTMenu* menu) = 0;
    virtual void onButtonPressed(TFTMenu* menu,int pressedButton) = 0;

    UTFT_Buttons_Rus* screenButtons; // кнопки на экране

    private:
      const char* screenName;
      bool isActiveScreen;
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<AbstractTFTScreen*> TFTScreensList; // список экранов
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// класс-менеджер работы с TFT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTMenu
{

public:
  TFTMenu();

  void setup();
  void update();

  void addScreen(AbstractTFTScreen* screen);

  void switchToScreen(AbstractTFTScreen* screen);
  void switchToScreen(const char* screenName);
  void switchToScreen(unsigned int screenIndex);
  
  UTFT* getDC() { return tftDC; };
  URTouch* getTouch() { return tftTouch; };
  UTFTRus* getRusPrinter() { return &rusPrint; };

  int print(const char* str,int x, int y, int deg=0, bool computeStringLengthOnly=false);

 

private:

  TFTScreensList screens;
  UTFT* tftDC;
  URTouch* tftTouch;
  UTFTRus rusPrint;

  int currentScreenIndex;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern TFTMenu Screen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MessageBoxScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create();

  void confirm(Vector<const char*>& lines, const char* okTarget, const char* cancelTarget);
  void show(Vector<const char*>& lines, const char* okTarget);
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
      MessageBoxScreen();

      int noButton,yesButton;
      const char* targetOkScreen;
      const char* targetCancelScreen;
      Vector<const char*> lines;

      
     
  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MessageBoxScreen* MessageBox;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
