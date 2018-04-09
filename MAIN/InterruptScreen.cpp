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

  for(int i=0;i<3;i++)
  {
    EthalonCompareBox box;
    compareBoxes.push_back(box);
  }
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
  Drawing::ComputeChart(list1, serie1, list2, serie2, list3, serie3);

  // вычисляем моторесурс
  computeMotoresource();

  // запоминаем время начала показа и переключаемся на экран
  startSeenTime = millis();
  Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::OnInterruptRaised(const InterruptTimeList& list, uint8_t listNum, EthalonCompareResult compareResult)
{

  if(!canAcceptInterruptData)
  {
    DBGLN("InterruptScreen::OnInterruptRaised - CAN'T ACCEPT INTERRUPT DATA!");
    return;
  }

  // пришли результаты серии прерываний с одного из списков.
  // мы запоминаем результаты в локальный список.
  EthalonCompareBox box;

  switch(listNum)
  {
    case 0:
      list1 = list;
      box.chartColor = VGA_WHITE;
    break;      

    case 1:
      list2 = list;
      box.chartColor = VGA_BLUE;
    break;      

    case 2:
      list3 = list;
      box.chartColor = VGA_YELLOW;
    break;      
    
  } // switch


  box.channelNum = listNum;
  
  switch(compareResult)
  {
    case COMPARE_RESULT_NoSourcePulses:
    case COMPARE_RESULT_NoEthalonFound:
    case COMPARE_RESULT_RodBroken:
      box.compareColor = VGA_GRAY;
      box.foreCompareColor = VGA_BLACK;
      box.compareCaption = "-";
    break;

    case COMPARE_RESULT_MatchEthalon:
      box.compareColor = VGA_LIME;
      box.foreCompareColor = VGA_BLACK;
      box.compareCaption = "OK";
    break;

    case COMPARE_RESULT_MismatchEthalon:
      box.compareColor = VGA_RED;
      box.foreCompareColor = VGA_WHITE;
      box.compareCaption = "ERR";
    break;
  }

  compareBoxes[listNum] = box;  
  
  
  // для теста - печатаем в Serial
  #ifdef _DEBUG

    if(list.size() > 1)
    {
      DBG("INTERRUPT #")
      DBG(listNum);
      DBGLN(" DATA >>");
      
      for(size_t i=0;i<list.size();i++)
      {
        DBGLN(list[i]);
      }
    }

    DBGLN("<< END OF INTERRUPT DATA");
    
  #endif // _DEBUG  
 
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
    dc->print(strDate, 5, 1);
    dc->print(strTime, 90, 1);

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

    if(channelMotoresourcePercents2 >= (100 - MOTORESOURCE_BLINK_PERCENTS) )
    {
      // ресурс по системе на канале 2 исчерпан, надо мигать надписью
       motoresourceBlinkTimer2 += dT;
      if(motoresourceBlinkTimer2 > MOTORESOURCE_BLINK_DURATION)
      {
        motoresourceBlinkTimer2 -= MOTORESOURCE_BLINK_DURATION;
        
        if(motoresourceLastFontColor2 == VGA_RED)
          motoresourceLastFontColor2 = VGA_BLACK;
        else
          motoresourceLastFontColor2 = VGA_RED;

          canRedrawMotoresource = true;
      }
    }

    if(channelMotoresourcePercents3 >= (100 - MOTORESOURCE_BLINK_PERCENTS) )
    {
      // ресурс по системе на канале 3 исчерпан, надо мигать надписью
      motoresourceBlinkTimer3 += dT;
      if(motoresourceBlinkTimer3 > MOTORESOURCE_BLINK_DURATION)
      {
        motoresourceBlinkTimer3 -= MOTORESOURCE_BLINK_DURATION;
        
        if(motoresourceLastFontColor3 == VGA_RED)
          motoresourceLastFontColor3 = VGA_BLACK;
        else
          motoresourceLastFontColor3 = VGA_RED;

          canRedrawMotoresource = true;
      }
    }

    if(canRedrawMotoresource)
      drawMotoresource(menu);
      
      
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::drawMotoresource(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();
  dc->setFont(SmallRusFont);

  word oldColor = dc->getColor();
  word oldBackColor = dc->getBackColor();
  dc->setBackColor(VGA_BLACK);

  uint32_t channelResourceCurrent1 = Settings.getMotoresource(0);
  uint32_t channelResourceCurrent2 = Settings.getMotoresource(1);
  uint32_t channelResourceCurrent3 = Settings.getMotoresource(2);

  uint32_t channelResourceMax1 = Settings.getMotoresourceMax(0);
  uint32_t channelResourceMax2 = Settings.getMotoresourceMax(1);
  uint32_t channelResourceMax3 = Settings.getMotoresourceMax(2);

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

  dc->setColor(motoresourceLastFontColor2);

  str = F("Ресурс 2: ");
  str += channelResourceCurrent2;
  str += F("/");
  str += channelResourceMax2;
  str += F(" (");
  str += channelMotoresourcePercents2;
  str += F("%)");

  menu->print(str.c_str(),curX,curY);
  curY += fontHeight + 4;  

  dc->setColor(motoresourceLastFontColor3);

  str = F("Ресурс 3: ");
  str += channelResourceCurrent3;
  str += F("/");
  str += channelResourceMax3;
  str += F(" (");
  str += channelMotoresourcePercents3;
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
  channelMotoresourcePercents1 = (channelResourceCurrent1*100)/channelResourceMax1;

  uint32_t channelResourceCurrent2 = Settings.getMotoresource(1);
  uint32_t channelResourceMax2 = Settings.getMotoresourceMax(1);
  channelMotoresourcePercents2 = (channelResourceCurrent2*100)/channelResourceMax2;

  uint32_t channelResourceCurrent3 = Settings.getMotoresource(2);
  uint32_t channelResourceMax3 = Settings.getMotoresourceMax(2);
  channelMotoresourcePercents3 = (channelResourceCurrent3*100)/channelResourceMax3;

  motoresourceLastFontColor1 = channelMotoresourcePercents1 < (100 - MOTORESOURCE_BLINK_PERCENTS) ? VGA_WHITE : VGA_RED;
  motoresourceLastFontColor2 = channelMotoresourcePercents2 < (100 - MOTORESOURCE_BLINK_PERCENTS) ? VGA_BLUE : VGA_RED;
  motoresourceLastFontColor3 = channelMotoresourcePercents3 < (100 - MOTORESOURCE_BLINK_PERCENTS) ? VGA_YELLOW : VGA_RED;

  timerDelta = millis();
  motoresourceBlinkTimer3 = 0;
  motoresourceBlinkTimer2 = MOTORESOURCE_BLINK_DURATION/3;
  motoresourceBlinkTimer1 = (MOTORESOURCE_BLINK_DURATION/3)*2;

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


  for(size_t i=0;i<compareBoxes.size();i++)
  {

    EthalonCompareBox box = compareBoxes[i];
    dc->setBackColor(VGA_BLACK);
    dc->setColor(box.chartColor);
    String channelNum = String(box.channelNum+1);
    uint8_t captionLen = menu->print(channelNum.c_str(),0,0,0,true);
    menu->print(channelNum.c_str(), curX, curY + (boxHeight - fontHeight)/2 );

    dc->setBackColor(box.compareColor);
    dc->setColor(box.compareColor);

    uint16_t boxLeft = curX + captionLen*fontWidth + spacing;
    dc->fillRoundRect(boxLeft, curY, boxLeft + boxWidth, curY + boxHeight);

    dc->setColor(box.foreCompareColor);
    captionLen = menu->print(box.compareCaption,0,0,0,true);
    menu->print(box.compareCaption, boxLeft + (boxWidth - captionLen*fontWidth)/2, curY + (boxHeight - fontHeight)/2 );  
    
    curY += boxHeight + spacing;

  } // for

  dc->setBackColor(oldBackColor);
  dc->setColor(oldColor);
  dc->setFont(oldFont);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::doDraw(TFTMenu* menu)
{
  drawTime(menu);
  Drawing::DrawChart(this,serie1, serie2, serie3);
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

