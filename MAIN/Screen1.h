#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран номер 1
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CHART_POINTS_COUNT 150 // кол-во точек на графике
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Screen1 : public AbstractTFTScreen
{
  public:

  static AbstractTFTScreen* create()
  {
    return new Screen1();
  }
  
  void requestToDrawChart( uint16_t* points1,   uint16_t* points2,  uint16_t* points3, uint16_t pointsCount);

   virtual void onActivate();
   virtual void onDeactivate();


protected:
  
    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

private:
    Screen1();

    int last3V3Voltage, last5Vvoltage, last200Vvoltage;
    void drawVoltage(TFTMenu* menu);

    void drawInductiveSensors(TFTMenu* menu);
    uint8_t inductiveSensorState1, inductiveSensorState2, inductiveSensorState3;

    void drawChart(); // рисуем наши графики
    bool canDrawChart;
    bool inDrawingChart;
    bool canLoopADC;
    
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

   uint16_t getSynchroPoint(uint16_t* points, uint16_t pointsCount);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern Screen1* mainScreen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

