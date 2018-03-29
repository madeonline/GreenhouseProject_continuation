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
  computeChart();
  
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
void InterruptScreen::drawSerie(Points& serie,RGBColor color)
{
  DBGLN(F("InterruptScreen::drawSerie"));
  
  if(serie.size() < 2 || !isActive()) // низзя рисовать
    return;
   
    UTFT* dc = Screen.getDC();
    word oldColor = dc->getColor();  
  
    dc->setColor(color.R, color.G, color.B);
      
    for (size_t i=1;i<serie.size();i++)
    {
        Point ptStart = serie[i-1];
        Point ptEnd = serie[i];
        dc->drawLine(ptStart.X , ptStart.Y, ptEnd.X , ptEnd.Y);
        yield();
    }
    
      dc->setColor(oldColor);        
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::computeSerie(InterruptTimeList& timeList,Points& serie, uint16_t xOffset, uint16_t yOffset)
{
  DBGLN(F("InterruptScreen::computeSerie"));
  // освобождаем серию
  serie.empty();

  size_t totalPulses = timeList.size();

  if(totalPulses < 2) // нет ничего к отрисовке, т.к. для графика нужны хотя бы две точки
  {
    DBGLN(F("NOT ENOUGH POINTS TO DRAW!"));
    return;
  }

  // получаем максимальное время импульса - это будет 100% по оси Y
  uint32_t maxPulseTime = 0;
  for(size_t i=1;i<timeList.size();i++)
  {
    maxPulseTime = max(maxPulseTime,(timeList[i] - timeList[i-1]));
  }

  DBG("MAX PULSE TIME=");
  DBGLN(maxPulseTime);  

  // теперь вычисляем положение по X для каждой точки импульсов
  uint16_t pointsAvailable = INTERRUPT_CHART_X_POINTS - xOffset;
  uint16_t xStep = pointsAvailable/(totalPulses-1);

  // сначала добавляем первую точку, у неё координаты по X - это 0, по Y - та же длительность импульса, что будет во второй точке
  uint32_t firstPulseTime = timeList[1] - timeList[0];
  firstPulseTime *= 100;
  uint16_t firstPointPercents = firstPulseTime/maxPulseTime;

  // получили значение в процентах от максимального значения Y для первой точки. Инвертируем это значение
  firstPointPercents = 100 - firstPointPercents;

  DBG("firstPointPercents=");
  DBGLN(firstPointPercents);

  // теперь можем высчитать абсолютное значение по Y для первой точки  
  uint16_t yCoord = INTERRUPT_CHART_Y_COORD - (firstPointPercents*(INTERRUPT_CHART_Y_POINTS-yOffset))/100;
  // здесь мы получили значение в пикселях, соответствующее проценту от максимального значения Y.
  // от этого значения надо отнять сдвиг по Y
  yCoord -= yOffset;

  // чтобы за сетку не вылазило
  if(yCoord < INTERRUPT_CHART_GRID_Y_START)
    yCoord = INTERRUPT_CHART_GRID_Y_START;

  DBG("yCoord=");
  DBGLN(yCoord);

  // добавляем первую точку
  uint16_t xCoord = INTERRUPT_CHART_X_COORD;
  Point pt = {xCoord,yCoord};
  serie.push_back(pt);

  xCoord += xOffset;

  // теперь считаем все остальные точки
  for(size_t i=1;i<timeList.size();i++)
  {
    uint32_t pulseTime = timeList[i] - timeList[i-1];
    pulseTime *= 100;
    
    uint16_t pulseTimePercents = pulseTime/maxPulseTime;
    pulseTimePercents = 100 - pulseTimePercents;

    DBG("pulseTimePercents=");
    DBGLN(pulseTimePercents);


    yCoord = INTERRUPT_CHART_Y_COORD - (pulseTimePercents*(INTERRUPT_CHART_Y_POINTS-yOffset))/100;
    yCoord -= yOffset;

  // чтобы за сетку не вылазило
  if(yCoord < INTERRUPT_CHART_GRID_Y_START)
    yCoord = INTERRUPT_CHART_GRID_Y_START;

    DBG("yCoord=");
    DBGLN(yCoord);

    Point ptNext = {xCoord,yCoord};
    serie.push_back(ptNext);
    
    xCoord += xStep;
    
  } // for

  // подсчёты завершены
  DBGLN("");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::computeChart()
{
  DBGLN(F("InterruptScreen::computeChart"));
 /*
  Формируем график
  Ось X время регистрации всех импульсов (общее время хода линейки, перемещения траверсы).
  Ось Y длительность импульсов.
  
  При этом максимальная длительность сформированных импульсов (в начале и конце движения) равна минимальным значениям по оси Y 
  Минимальная длительность сформированных импульсов (в середине хода линейки) соответствует максимальным значениям по оси Y. 
  */

  uint16_t yOffset = 0; // первоначальный сдвиг графиков по Y
  uint16_t yOffsetStep = 5; // шаг сдвига графиков по Y, чтобы не пересекались

  uint16_t xOffset = 5; // первоначальный сдвиг графиков по X, чтобы первый пик начинался не с начала координат
  uint16_t xOffsetStep = 5; // шаг сдвига графиков по X, чтобы не пересекались
  
  computeSerie(list1,serie1,xOffset, yOffset);
  yOffset += yOffsetStep;
  xOffset += xOffsetStep;

  computeSerie(list2,serie2,xOffset, yOffset);
  yOffset += yOffsetStep;
  xOffset += xOffsetStep;

  computeSerie(list3,serie3,xOffset, yOffset);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterruptScreen::drawChart()
{
  DBGLN(F("InterruptScreen::drawChart"));
  // рисуем сетку
  int gridX = INTERRUPT_CHART_GRID_X_START; // начальная координата сетки по X
  int gridY = INTERRUPT_CHART_GRID_Y_START; // начальная координата сетки по Y
  int columnsCount = 6; // 5 столбцов
  int rowsCount = 4; // 4 строки
  int columnWidth = INTERRUPT_CHART_X_POINTS/columnsCount; // ширина столбца
  int rowHeight = INTERRUPT_CHART_Y_POINTS/rowsCount; // высота строки 
  RGBColor gridColor = { 0,200,0 }; // цвет сетки


  // вызываем функцию для отрисовки сетки, её можно вызывать из каждого класса экрана
  Drawing::DrawGrid(gridX, gridY, columnsCount, rowsCount, columnWidth, rowHeight, gridColor);

  drawSerie(serie1,{ 255,255,255 });
  yield();
  drawSerie(serie2,{ 0,0,255 });
  yield();
  drawSerie(serie3,{ 255,255,0 });
  yield();
  
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
  drawChart();
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

