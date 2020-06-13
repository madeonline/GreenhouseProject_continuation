//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "InterruptScreen.h"
#include "DS3231.h"
#include "Settings.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
InterruptScreen* ScreenInterrupt = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen* InterruptScreen::create()
{
  if(ScreenInterrupt)
    return ScreenInterrupt;
    
  ScreenInterrupt = new InterruptScreen();

  // назначаем обработчика прерываний по умолчанию - наш экран
  InterruptHandler.setSubscriber(ScreenInterrupt);
  
  return ScreenInterrupt;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
InterruptScreen::InterruptScreen() : AbstractTFTScreen("INTERRUPT")
{
  startSeenTime = 0;
  timerDelta = 0;
  canAcceptInterruptData = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::onDeactivate()
{
    // после деактивирования нашего экрана мы опять можем принимать данные прерываний, чтобы показать новые графики
    canAcceptInterruptData = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::OnHaveInterruptData()
{
  // пришло событие, что хотя бы одно из прерываний заполнено серией измерений.
  // в этом случае, раз мы являемся активным подписчиком класса обработки прерываний - 
  // мы должны показать график на экране.

  // принимать данные с прерываний мы можем только тогда, когда уже показали текущие прерывания на экране и переключились на главный экран
  if(!canAcceptInterruptData)
  {
    DBGLN("InterruptScreen::OnHaveInterruptData - CAN'T ACCEPT INTERRUPT DATA!");    
    return;
  }

  // говорим, что до нового цикла мы не можем больше ничего принимать.
  canAcceptInterruptData = false;

  // сначала делаем пересчёт точек на график, т.к. у нас ограниченное кол-во точек - это раз.
  // два - когда в списках прерываний точек заведомо меньше, чем точек на графике (например, 20 вместо 150) - без пересчёта получим
  // куцый график, в этом случае нам надо его растянуть по-максимуму.

  Drawing::ComputeChart(list1, serie1);

  // вычисляем моторесурс
  computeMotoresource();

  // запоминаем время начала показа и переключаемся на экран
  startSeenTime = millis();
  Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::OnInterruptRaised(const InterruptTimeList& list, EthalonCompareResult compareResult)
{

  if(!canAcceptInterruptData)
  {
    DBGLN("InterruptScreen::OnInterruptRaised - CAN'T ACCEPT INTERRUPT DATA!");
    return;
  }

  // пришли результаты серии прерываний с одного из списков.
  // мы запоминаем результаты в локальный список.
  EthalonCompareBox box;
  list1 = list;
  box.chartColor = VGA_GRAY;
  box.compareColor = VGA_GRAY;
  box.foreCompareColor = VGA_GRAY;
  box.compareCaption = "-";

  switch(compareResult)
  {
    case COMPARE_RESULT_NoSourcePulses:
    case COMPARE_RESULT_NoEthalonFound:
    case COMPARE_RESULT_RodBroken:
	{
		box.compareColor = VGA_GRAY;
		box.foreCompareColor = VGA_BLACK;
		box.compareCaption = "-";
	}
    break;

    case COMPARE_RESULT_MatchEthalon:
	{
		box.compareColor = VGA_LIME;
		box.foreCompareColor = VGA_BLACK;
		box.compareCaption = "OK";
	}
    break;

    case COMPARE_RESULT_MismatchEthalon:
	{
		box.compareColor = VGA_RED;
		box.foreCompareColor = VGA_WHITE;
		box.compareCaption = "ERR";
	}
    break;
  }

 compareBox = box;
   
  // для теста - печатаем в Serial
  #ifdef _PRINT_INTERRUPT_DATA

    if(list1.size() > 1)
    {
		DBG("INTERRUPT");
      DBGLN(" DATA >>");
      
      for(size_t i=0;i<list1.size();i++)
      {
        DBGLN(list1[i]);
      }
    }

    DBGLN("<< END OF INTERRUPT DATA");
    
  #endif // _PRINT_INTERRUPT_DATA  
 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::drawTime(TFTMenu* menu)
{
    DS3231Time tm = RealtimeClock.getTime();

    // получаем компоненты даты в виде строк
    UTFT* dc = menu->getDC();
    dc->setColor(VGA_WHITE);
    dc->setBackColor(VGA_BLACK);
    dc->setFont(SmallRusFont);
    String strDate = RealtimeClock.getDateStr(tm);
    String strTime = RealtimeClock.getTimeStr(tm);

    // печатаем их
    dc->print(strDate.c_str(), 5, 1);
    dc->print(strTime.c_str(), 90, 1);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::doSetup(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::doUpdate(TFTMenu* menu)
{
    uint32_t now = millis();
    uint32_t dT = now - timerDelta;
    timerDelta = now;
    
    if(now - startSeenTime > INTERRUPT_SCREEN_SEE_TIME)
    {
      // время показа истекло, переключаемся на главный экран
		DBGLN(F("График показан, переключаемся на главный экран!"));

      startSeenTime = 0;
      Screen.switchToScreen("Main");
      return;
    }

    bool canRedrawMotoresource = false;
    
    if(channelMotoresourcePercents1 >= (100 - MOTORESOURCE_BLINK_PERCENTS) )
    {
      // ресурс по системе на канале 1 исчерпан, надо мигать надписью
      motoresourceBlinkTimer1 += dT;
      
      if(motoresourceBlinkTimer1 > MOTORESOURCE_BLINK_DURATION)
      { 
        motoresourceBlinkTimer1 -= MOTORESOURCE_BLINK_DURATION;
               
        if(motoresourceLastFontColor1 == VGA_RED)
          motoresourceLastFontColor1 = VGA_BLACK;
        else
          motoresourceLastFontColor1 = VGA_RED;

          canRedrawMotoresource = true;
      }
    }

    if(canRedrawMotoresource)
      drawMotoresource(menu);
      
      
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::drawMotoresource(TFTMenu* menu)
{
	DBGLN("Рисуем моторесурс...");

  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();
  dc->setFont(SmallRusFont);

  word oldColor = dc->getColor();
  word oldBackColor = dc->getBackColor();
  dc->setBackColor(VGA_BLACK);

  uint32_t channelResourceCurrent1 = Settings.getMotoresource(0);  
  uint32_t channelResourceMax1 = Settings.getMotoresourceMax(0);

  // рисуем моторесурс системы по каналам
  uint16_t curX = 5;
  uint16_t curY = 130;
  uint8_t fontHeight = dc->getFontYsize();

  dc->setColor(motoresourceLastFontColor1);

  String str = F("Ресурс 1: ");
  str += channelResourceCurrent1;
  str += F("/");
  str += channelResourceMax1;
  str += F(" (");
  str += channelMotoresourcePercents1;
  str += F("%)");

  menu->print(str.c_str(),curX,curY);
  curY += fontHeight + 4;

  
  dc->setFont(oldFont);
  dc->setColor(oldColor);
  dc->setBackColor(oldBackColor);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::computeMotoresource()
{
  
  uint32_t channelResourceCurrent1 = Settings.getMotoresource(0);
  uint32_t channelResourceMax1 = Settings.getMotoresourceMax(0);
  channelMotoresourcePercents1 = (channelResourceCurrent1*100)/(channelResourceMax1 ? channelResourceMax1 : 1);
  motoresourceLastFontColor1 = channelMotoresourcePercents1 < (100 - MOTORESOURCE_BLINK_PERCENTS) ? VGA_WHITE : VGA_RED;
  timerDelta = millis();
  motoresourceBlinkTimer1 = (MOTORESOURCE_BLINK_DURATION/3)*2;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::OnTimeBeforeInterruptsBegin(uint32_t tm, bool hasTime)
{
  timeBeforeInterrupts = tm;
  hasRelayTriggeredTime = hasTime;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::drawCompareResult(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  word oldBackColor = dc->getBackColor();  
  word oldColor = dc->getColor();
  uint8_t* oldFont = dc->getFont();
  dc->setFont(SmallRusFont);
  uint8_t fontWidth = dc->getFontXsize();
  uint8_t fontHeight = dc->getFontYsize();
  
  uint16_t curX = 162;
  uint16_t curY = 20; 
  uint8_t boxWidth = 30;  
  uint8_t boxHeight = 18;
  uint8_t spacing = 4;


    dc->setBackColor(VGA_BLACK);
    dc->setColor(compareBox.chartColor);
	String channelNum = "1"; /////// String(compareBox.channelNum + 1);
    uint8_t captionLen = menu->print(channelNum.c_str(),0,0,0,true);
    menu->print(channelNum.c_str(), curX, curY + (boxHeight - fontHeight)/2 );

    dc->setBackColor(compareBox.compareColor);
    dc->setColor(compareBox.compareColor);

    uint16_t boxLeft = curX + captionLen*fontWidth + spacing;
    dc->fillRoundRect(boxLeft, curY, boxLeft + boxWidth, curY + boxHeight);

    dc->setColor(compareBox.foreCompareColor);
    captionLen = menu->print(compareBox.compareCaption,0,0,0,true);
    menu->print(compareBox.compareCaption, boxLeft + (boxWidth - captionLen*fontWidth)/2, curY + (boxHeight - fontHeight)/2 );  

  dc->setBackColor(oldBackColor);
  dc->setColor(oldColor);
  dc->setFont(oldFont);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::doDraw(TFTMenu* menu)
{
	Drawing::DrawChart(this, serie1);
	drawTime(menu);
	drawMotoresource(menu);
	drawCompareResult(menu);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  // обработчик нажатия на кнопку. Номера кнопок начинаются с 0 и идут в том порядке, в котором мы их добавляли
 /*
  if (pressedButton == 0)
	  menu->switchToScreen("SCREEN5"); // переключаемся на 5 экран
  else if (pressedButton == 1)
	  menu->switchToScreen("SCREEN6"); // переключаемся на 6 экран
  else if (pressedButton == 2)
	  menu->switchToScreen("Main"); // переключаемся на первый экран
*/    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

