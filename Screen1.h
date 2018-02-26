#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран номер 1
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Screen1 : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new Screen1();
  }
  
  void DrawChart(); // рисуем наши графики
  void addPoints(uint8_t serieNumber, uint16_t* points, uint16_t pointsCount);

protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
    Screen1();
    
	  int getFreeMemory();
	  int oldsecond;

    void drawTime(TFTMenu* menu);
    
  	uint16_t* points1;
    uint16_t* points2;
    uint16_t* points3;
   
	  // НАШ ГРАФИК ДЛЯ ЭКРАНА
	  Chart chart;
	  ChartSerie* serie1;
	  ChartSerie* serie2;
	  ChartSerie* serie3;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

