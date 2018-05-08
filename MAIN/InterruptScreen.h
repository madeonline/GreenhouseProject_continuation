#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "UTFTMenu.h"
#include "InterruptHandler.h"
#include "Drawing.h"
#include "EthalonComparer.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  uint8_t channelNum;
  uint16_t chartColor;
  uint16_t compareColor;
  uint16_t foreCompareColor;
  const char* compareCaption;
  
} EthalonCompareBox;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<EthalonCompareBox> EthalonCompareBoxes;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// экран с графиком прерываний
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class InterruptScreen : public AbstractTFTScreen, public InterruptEventSubscriber
{
  public:

  static AbstractTFTScreen* create();  

  void OnInterruptRaised(const InterruptTimeList& list, uint8_t listNum, EthalonCompareResult result);
  void OnHaveInterruptData();
  void OnTimeBeforeInterruptsBegin(uint32_t tm, bool hasRelayTime);
    
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
    void drawTimeBeforeInterrupt(TFTMenu* menu);

    InterruptTimeList list1;
    InterruptTimeList list2;
    InterruptTimeList list3;

    unsigned long startSeenTime;
    uint32_t timeBeforeInterrupts;
    bool hasRelayTriggeredTime;

    bool canAcceptInterruptData;

    EthalonCompareBoxes compareBoxes;
    void drawCompareResult(TFTMenu* menu);

    Points serie1;
    Points serie2;
    Points serie3;

    uint32_t timerDelta;  
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern InterruptScreen* ScreenInterrupt;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

