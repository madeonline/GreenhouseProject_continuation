#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран номер 5 Установка даты и времени
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Screen5 : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new Screen5();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);


private:
      Screen5();
	  int t_temp_sec, t_temp_min, t_temp_hour, t_temp_dow, t_temp_date, t_temp_mon, t_temp_year;

  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

