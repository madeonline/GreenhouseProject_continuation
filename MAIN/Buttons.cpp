//--------------------------------------------------------------------------------------------------
#include "Buttons.h"
#include "InfoDiodes.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------
ButtonsList Buttons;
//--------------------------------------------------------------------------------------------------
ButtonsList::ButtonsList()
{
  inited = false;
}
//--------------------------------------------------------------------------------------------------
void ButtonsList::begin()
{
  redButton.begin(BUTTON_RED);
  blue1Button.begin(BUTTON_BLUE1);
  blue2Button.begin(BUTTON_BLUE2);
  yellowButton.begin(BUTTON_YELLOW);

  inited = true;
}
//--------------------------------------------------------------------------------------------------
void ButtonsList::update()
{
  if(!inited)
    return;
    
  redButton.update();
  blue1Button.update();
  blue2Button.update();
  yellowButton.update();

  if(yellowButton.isClicked())
  {
    DBGLN(F("YELLOW BUTTON CLICKED!"));
    InfoDiodes.test(false); // гасим светодиод ТЕСТ
  }

  if(redButton.isClicked())
  {
    DBGLN(F("RED BUTTON CLICKED!"));
  }

  if(blue1Button.isClicked())
  {
    DBGLN(F("BLUE #1 BUTTON CLICKED!"));
  }

  if(blue2Button.isClicked())
  {
    DBGLN(F("BLUE #2 BUTTON CLICKED!"));
  }    
}
//--------------------------------------------------------------------------------------------------

