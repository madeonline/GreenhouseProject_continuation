#pragma once
//--------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include "CoreButton.h"
//--------------------------------------------------------------------------------------------------
class ButtonsList
{
  public:
    ButtonsList();
    void begin();
    void update();

  private:
  
    Button redButton;
    Button blue1Button;
    Button blue2Button;
    Button yellowButton;

    bool inited;
    
};
//--------------------------------------------------------------------------------------------------
extern ButtonsList Buttons;
//--------------------------------------------------------------------------------------------------

