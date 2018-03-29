#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
#include "InterruptHandler.h"
#include "Drawing.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран с графиком прерываний
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<Point> Points;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class InterruptScreen : public AbstractTFTScreen, public InterruptEventSubscriber
{
  public:

  static AbstractTFTScreen* create();  

  void OnInterruptRaised(const InterruptTimeList& list, uint8_t listNum);
  void OnHaveInterruptData();
    
protected:

    virtual void doSetup(TFTMenu* menu);
    virtual void doUpdate(TFTMenu* menu);
    virtual void doDraw(TFTMenu* menu);
    virtual void onButtonPressed(TFTMenu* menu, int pressedButton);

    virtual void onDeactivate();

private:
    InterruptScreen();

    void drawTime(TFTMenu* menu);
    void drawMotoresource(TFTMenu* menu);

    InterruptTimeList list1;
    InterruptTimeList list2;
    InterruptTimeList list3;

    unsigned long startSeenTime;

    bool canAcceptInterruptData;
    void drawChart(); // рисуем график
    void computeChart(); // пересчитываем график

    Points serie1;
    Points serie2;
    Points serie3;

    void computeSerie(InterruptTimeList& timeList,Points& serie, uint16_t xOffset, uint16_t yOffset);
    void drawSerie(Points& serie,RGBColor color);

  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern InterruptScreen* ScreenInterrupt;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

