#include "UTFTMenu.h"
#include "CONFIG.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTMenu* tftMenuManager;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen::AbstractTFTScreen(const char* name)
{
  screenName = name;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen::~AbstractTFTScreen()
{
  delete screenButtons;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AbstractTFTScreen::setup(TFTMenu* menu)
{
  screenButtons = new UTFT_Buttons_Rus(menu->getDC(), menu->getTouch(),menu->getRusPrinter());
  screenButtons->setTextFont(TFT_FONT);
  screenButtons->setButtonColors(TFT_BUTTON_COLORS);

  doSetup(menu);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AbstractTFTScreen::update(TFTMenu* menu)
{
  if(isActive())
  {
  int pressedButton = screenButtons->checkButtons();
  
  if(pressedButton != -1)
    onButtonPressed(menu, pressedButton);

    if(isActive())
      doUpdate(menu);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AbstractTFTScreen::draw(TFTMenu* menu)
{
  if(isActive())
  {
    doDraw(menu);
    
    if(isActive())
      screenButtons->drawButtons(); 
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTMenu
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTMenu::TFTMenu()
{
  currentScreenIndex = -1;
  requestedToActiveScreen = NULL;
  requestedToActiveScreenIndex = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::addScreen(AbstractTFTScreen* screen)
{
  screen->setup(this);
  screens.push_back(screen);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::setup()
{
  tftMenuManager = this;
  
  
  tftDC = new UTFT(TFT_MODEL,TFT_RS_PIN,TFT_WR_PIN,TFT_CS_PIN,TFT_RST_PIN);
  tftTouch = new URTouch(TFT_TOUCH_CLK_PIN,TFT_TOUCH_CS_PIN,TFT_TOUCH_DIN_PIN,TFT_TOUCH_DOUT_PIN,TFT_TOUCH_IRQ_PIN);

  #if TFT_INIT_DELAY > 0
  delay(TFT_INIT_DELAY);
  #endif
  
  tftDC->InitLCD(TFT_ORIENTATION);
  tftDC->setBackColor(TFT_BACK_COLOR);
  tftDC->fillScr(TFT_BACK_COLOR);
  tftDC->setFont(TFT_FONT);

  tftTouch->InitTouch(TFT_ORIENTATION);
  tftTouch->setPrecision(TOUCH_PRECISION);
  
  rusPrint.init(tftDC);

  // добавляем экран мессадж-бокса
  addScreen(MessageBoxScreen::create());

  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TFTMenu::print(const char* str,int x, int y, int deg, bool computeStringLengthOnly)
{
  return rusPrint.print(str,x,y,deg,computeStringLengthOnly);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::update()
{

  if(requestedToActiveScreen != NULL)
  {
    // попросили сделать экран активным
    AbstractTFTScreen* screen = requestedToActiveScreen;
    currentScreenIndex = requestedToActiveScreenIndex;
    
    requestedToActiveScreen = NULL;
    
    screen->setActive(true);
    screen->onActivate();

    tftDC->setBackColor(TFT_BACK_COLOR);
    tftDC->fillScr(TFT_BACK_COLOR); // clear screen first

    screen->update(this);
    screen->draw(this);
    
    return;
    
  } // if(requestedToActiveScreen != NULL)

  if(currentScreenIndex == -1) // ни разу не рисовали ещё ничего, исправляемся
  {
    if(screens.size())
     switchToScreen((unsigned int)0); // переключаемся на первый экран, если ещё ни разу не показали ничего     
  }

  if(currentScreenIndex == -1)
    return;

  // обновляем текущий экран
  AbstractTFTScreen* currentScreen = screens[currentScreenIndex];
  currentScreen->update(this);
  
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::switchToScreen(AbstractTFTScreen* screen)
{

  if(requestedToActiveScreen != NULL) // ждём переключения на новый экран
    return;
  
  if(currentScreenIndex > -1 && screens.size())
  {
     AbstractTFTScreen* si = screens[currentScreenIndex];
     si->setActive(false);
     si->onDeactivate();
  }
  
  for(size_t i=0;i<screens.size();i++)
  {
    if(screens[i] == screen)
    {
      requestedToActiveScreen = screen;
      requestedToActiveScreenIndex = i;
      /*
      currentScreenIndex = i;

      screen->setActive(true);
      screen->onActivate();

      tftDC->setBackColor(TFT_BACK_COLOR);
      tftDC->fillScr(TFT_BACK_COLOR); // clear screen first

      screen->update(this);
      screen->draw(this);
      */
      break;
    }
  }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::switchToScreen(unsigned int screenIndex)
{
  if(screenIndex < screens.size())
      switchToScreen(screens[screenIndex]);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::switchToScreen(const char* screenName)
{
  
  // переключаемся на запрошенный экран
  for(size_t i=0;i<screens.size();i++)
  {
    AbstractTFTScreen* si = screens[i];
    if(!strcmp(si->getName(),screenName))
    {
      switchToScreen(si);
      break;
    }
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTMenu Screen;
MessageBoxScreen* MessageBox;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBoxScreen::MessageBoxScreen() : AbstractTFTScreen("MessageBox")
{
  targetOkScreen = NULL;
  targetCancelScreen = NULL; 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::doSetup(TFTMenu* menu)
{

  yesButton = screenButtons->addButton(5, 107, 210, 30, "ДА");
  noButton = screenButtons->addButton(5, 142, 210, 30, "НЕТ");


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);

   int fontHeight = dc->getFontYsize();
   int fontWidth = dc->getFontXsize();
   int displayWidth = dc->getDisplayXSize();
   int lineSpacing = 2;  

   int curX = 0;
   int curY = 10;

    for(size_t i=0;i<lines.size();i++)
    {
      int lineLength = menu->print(lines[i],curX,curY,0,true);
      curX = (displayWidth - lineLength*fontWidth)/2;    
      menu->print(lines[i],curX,curY);
      curY += fontHeight + lineSpacing;
    }
  

  dc->setFont(oldFont);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == noButton && targetCancelScreen)
    menu->switchToScreen(targetCancelScreen);
  else
  if(pressedButton == yesButton && targetOkScreen)
    menu->switchToScreen(targetOkScreen);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::show(Vector<const char*>& _lines, const char* okTarget)
{
  lines = _lines;
  screenButtons->relabelButton(yesButton,"ОК");
  screenButtons->hideButton(noButton);
  
  targetOkScreen = okTarget;
  targetCancelScreen = NULL;

  Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::confirm(Vector<const char*>& _lines, const char* okTarget, const char* cancelTarget)
{
  lines = _lines;

  screenButtons->relabelButton(yesButton,"ДА");
  screenButtons->relabelButton(noButton,"НЕТ");
  
  screenButtons->showButton(noButton);
  
  targetOkScreen = okTarget;
  targetCancelScreen = cancelTarget;

  Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen* MessageBoxScreen::create()
{
    MessageBox = new MessageBoxScreen();
    return MessageBox;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

