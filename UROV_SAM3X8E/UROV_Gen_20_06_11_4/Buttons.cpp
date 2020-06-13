//--------------------------------------------------------------------------------------------------
#include "Buttons.h"
#include "Feedback.h"
#include "CONFIG.h"
#include "Relay.h"
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
	blueButton.begin(BUTTON_BLUE);
	blackButton.begin(BUTTON_BLACK);
	greyButton.begin(BUTTON_GREY);

  inited = true;
}
//--------------------------------------------------------------------------------------------------
void ButtonsList::update()
{
  if(!inited)
    return;
    
  redButton.update();
  blueButton.update();
  greyButton.update();
  blackButton.update();

  if (redButton.isClicked())
  {
	  DBGLN(F("RED BUTTON CLICKED!"));
	  // подать/отключить ток на фазу "A"
	  Relay_LineA.switchState(); // просим реле переключить состояние
  }

  if (blueButton.isClicked())
  {
	  DBGLN(F("BLUE BUTTON CLICKED!"));
	  // подать/отключить ток на фазу "B"
   	  Relay_LineB.switchState(); // просим реле переключить состояние
  }

  if (greyButton.isClicked())
  {
	  DBGLN(F("YELLOW BUTTON CLICKED!"));
     // подать/отключить ток на фазу "С"
     Relay_LineC.switchState();
  }

  if (blackButton.isClicked())
  {
	  DBGLN(F("BLACK BUTTON CLICKED!"));
	  // Запускаем тест 
   	  Relay_LineALL.switchState();
  }
}
//--------------------------------------------------------------------------------------------------

