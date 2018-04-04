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
  DBGLN(F("InterruptScreen::OnHaveInterruptData"));
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
  
  // запоминаем время начала показа и переключаемся на экран
  startSeenTime = millis();
  Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::OnInterruptRaised(const InterruptTimeList& list, uint8_t listNum)
{
  DBGLN(F("InterruptScreen::OnInterruptRaised"));

  if(!canAcceptInterruptData)
  {
    DBGLN("InterruptScreen::OnInterruptRaised - CAN'T ACCEPT INTERRUPT DATA!");
    return;
  }

  // пришли результаты серии прерываний с одного из списков.
  // мы запоминаем результаты в локальный список.

  switch(listNum)
  {
    case 0:
      list1 = list;
    break;      

    case 1:
      list2 = list;
    break;      

    case 2:
      list3 = list;
    break;      
    
  } // switch
  
  // для теста - печатаем в Serial
  #ifdef _DEBUG

    if(list.size() > 1)
    {
      DBGLN("INTERRUPT DATA >>");
      
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
    if(millis() - startSeenTime > INTERRUPT_SCREEN_SEE_TIME)
    {
      // время показа истекло, переключаемся на главный экран
      startSeenTime = 0;
      Screen.switchToScreen("Main");
    }
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

  // рисуем моторесурс системы по каналам
  uint16_t curX = 5;
  uint16_t curY = 130;
  uint8_t fontHeight = dc->getFontYsize();

  uint32_t channelResourceCurrent1 = Settings.getMotoresource(0);
  uint32_t channelResourceMax1 = Settings.getMotoresourceMax(0);
  uint32_t channelPercents1 = (channelResourceCurrent1*100)/channelResourceMax1;

  dc->setColor(255,255,255);

  String str = F("Ресурс 1: ");
  str += channelResourceCurrent1;
  str += F("/");
  str += channelResourceMax1;
  str += F(" (");
  str += channelPercents1;
  str += F("%)");

  menu->print(str.c_str(),curX,curY);
  curY += fontHeight + 4;


  uint32_t channelResourceCurrent2 = Settings.getMotoresource(1);
  uint32_t channelResourceMax2 = Settings.getMotoresourceMax(1);
  uint32_t channelPercents2 = (channelResourceCurrent2*100)/channelResourceMax2;

  dc->setColor(0,0,255);

  str = F("Ресурс 2: ");
  str += channelResourceCurrent2;
  str += F("/");
  str += channelResourceMax2;
  str += F(" (");
  str += channelPercents2;
  str += F("%)");

  menu->print(str.c_str(),curX,curY);
  curY += fontHeight + 4;  


  uint32_t channelResourceCurrent3 = Settings.getMotoresource(2);
  uint32_t channelResourceMax3 = Settings.getMotoresourceMax(2);
  uint32_t channelPercents3 = (channelResourceCurrent3*100)/channelResourceMax3;

  dc->setColor(255,255,0);

  str = F("Ресурс 3: ");
  str += channelResourceCurrent3;
  str += F("/");
  str += channelResourceMax3;
  str += F(" (");
  str += channelPercents3;
  str += F("%)");

  menu->print(str.c_str(),curX,curY);
  curY += fontHeight + 4;  
  


  dc->setFont(oldFont);
  dc->setColor(oldColor);
  dc->setBackColor(oldBackColor);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::doDraw(TFTMenu* menu)
{
  drawTime(menu);
  Drawing::DrawChart(this,serie1, serie2, serie3);
  drawMotoresource(menu);
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

