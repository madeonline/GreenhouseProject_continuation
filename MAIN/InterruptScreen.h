#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
#include "InterruptHandler.h"
#include "Drawing.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран с графиком прерываний
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

    uint32_t channelMotoresourcePercents1, channelMotoresourcePercents2, channelMotoresourcePercents3;
    word motoresourceLastFontColor1, motoresourceLastFontColor2, motoresourceLastFontColor3;
    uint32_t motoresourceBlinkTimer1, motoresourceBlinkTimer2, motoresourceBlinkTimer3;
    void computeMotoresource();
    void drawMotoresource(TFTMenu* menu);

    InterruptTimeList list1;
    InterruptTimeList list2;
    InterruptTimeList list3;

    unsigned long startSeenTime;

    bool canAcceptInterruptData;


    Points serie1;
    Points serie2;
    Points serie3;

    uint32_t timerDelta;  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern InterruptScreen* ScreenInterrupt;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

