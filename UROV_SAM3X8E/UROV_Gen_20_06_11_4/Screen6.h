#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран номер 6 Установка даты
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Screen6 : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new Screen6();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);


private:
      Screen6();
	  byte validateDate(byte d, byte m, word y);
	  byte validateDateForMonth(byte d, byte m, word y);
	  int t_temp_sec, t_temp_min, t_temp_hour, t_temp_dow, t_temp_date, t_temp_mon, t_temp_year;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

