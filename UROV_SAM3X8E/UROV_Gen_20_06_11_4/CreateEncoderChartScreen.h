#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран создания графика прерываний для энкодера
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CreateEncoderChartScreen : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new CreateEncoderChartScreen();
  }
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);
	

private:
      CreateEncoderChartScreen();
	  void get_Point_Screen(TFTMenu* menu);
	  void create_Schedule(TFTMenu* menu);
	  void clear_Grid(TFTMenu* menu);
      int clearButton, calculateButton, backButton, file1Button, file2Button, file3Button, mem1Button, mem2Button, mem3Button, grid_Button;
	  int touch_x_min = 20;      // Запрет формировать точку по Х меньше предыдущей
	  int step_pount = 0;        // Текущее количество точек на графике
	  int max_step_pount = 8;    // Максимальное количество точек на графике
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

