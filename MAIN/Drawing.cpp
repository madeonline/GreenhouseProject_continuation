//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Drawing.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
 Проблема рисования:

  1. Медленная работа с шиной.

 Вариант решения:

 1. Перед первой отрисовкой инициализировать массив с информацией - задействована ли точка в отрисовке или нет;
 2. Перед отрисовкой создавать массив точек, участвующих в текущем цикле отрисовки;
 3. Проверять:
    - если точка в текущем цикле отрисовки была задействована в предыдущем - не писать в шину ничего;
    - если точка из текущего цикла отрисовки отсутствует в предыдущем - рисовать её;
    - если точка из предыдущего цикла отрисовки отсутствует в текущем - стирать её;

  Однако, при таком подходе существуют проблемы, а именно (на примере области отрисовки 100х160):

    1. Большой размер матрицы - 16 000;
    2. Если убрать информацию по цветам - всё равно для одной матрицы - 2 000 байт информации, т.е. 4 000 байт на две матрицы;
    3. Если не убирать информацию по цветам - то для каждой серии - своя матрица на 2 000 байт.
 
 */
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace Drawing
{
  void ComputeSerie(InterruptTimeList& timeList,Points& serie, uint16_t xOffset, uint16_t yOffset)
  {
   //  DBGLN(F("ComputeSerie"));
      // освобождаем серию
      serie.empty();
    
      size_t totalPulses = timeList.size();
    
      if(totalPulses < 2) // нет ничего к отрисовке, т.к. для графика нужны хотя бы две точки
      {
      //  DBGLN(F("NOT ENOUGH POINTS TO DRAW!"));
        return;
      }
    
      // получаем максимальное время импульса - это будет 100% по оси Y
      uint32_t maxPulseTime = 0;
      for(size_t i=1;i<timeList.size();i++)
      {
        maxPulseTime = max(maxPulseTime,(timeList[i] - timeList[i-1]));
      }
    
    //  DBG("MAX PULSE TIME=");
   //   DBGLN(maxPulseTime);  
    
      // теперь вычисляем положение по X для каждой точки импульсов
      uint16_t pointsAvailable = INTERRUPT_CHART_X_POINTS - xOffset;
      uint16_t xStep = pointsAvailable/(totalPulses-1);
    
      // сначала добавляем первую точку, у неё координаты по X - это 0, по Y - та же длительность импульса, что будет во второй точке
      uint32_t firstPulseTime = timeList[1] - timeList[0];
      firstPulseTime *= 100;
      uint16_t firstPointPercents = firstPulseTime/maxPulseTime;
    
      // получили значение в процентах от максимального значения Y для первой точки. Инвертируем это значение
      firstPointPercents = 100 - firstPointPercents;
    
   //   DBG("firstPointPercents=");
   //   DBGLN(firstPointPercents);
    
      // теперь можем высчитать абсолютное значение по Y для первой точки  
      uint16_t yCoord = INTERRUPT_CHART_Y_COORD - (firstPointPercents*(INTERRUPT_CHART_Y_POINTS-yOffset))/100;
      // здесь мы получили значение в пикселях, соответствующее проценту от максимального значения Y.
      // от этого значения надо отнять сдвиг по Y
      yCoord -= yOffset;
    
      // чтобы за сетку не вылазило
      if(yCoord < INTERRUPT_CHART_GRID_Y_START)
        yCoord = INTERRUPT_CHART_GRID_Y_START;
    
   //   DBG("yCoord=");
   //   DBGLN(yCoord);
    
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
    
  //      DBG("pulseTimePercents=");
   //     DBGLN(pulseTimePercents);
    
    
        yCoord = INTERRUPT_CHART_Y_COORD - (pulseTimePercents*(INTERRUPT_CHART_Y_POINTS-yOffset))/100;
        yCoord -= yOffset;
    
      // чтобы за сетку не вылазило
      if(yCoord < INTERRUPT_CHART_GRID_Y_START)
        yCoord = INTERRUPT_CHART_GRID_Y_START;
    
  //      DBG("yCoord=");
  //      DBGLN(yCoord);
    
        Point ptNext = {xCoord,yCoord};
        serie.push_back(ptNext);
        
        xCoord += xStep;
        
      } // for
    
      // подсчёты завершены
    //  DBGLN("");    
  }

  void doDrawSerie(UTFT* dc,Points& serie)
  {
      for (size_t i=1;i<serie.size();i++)
      {
          Point ptStart = serie[i-1];
          Point ptEnd = serie[i];
          dc->drawLine(ptStart.X , ptStart.Y, ptEnd.X , ptEnd.Y);
          yield();
      }    
  }

  void DrawSerie(AbstractTFTScreen* caller, Points& serie, uint16_t color)
  {
    if(serie.size() < 2 || !caller->isActive()) // низзя рисовать
      return;
     
      UTFT* dc = Screen.getDC();
      word oldColor = dc->getColor();  
    
      dc->setColor(color);
      doDrawSerie(dc,serie);        
      dc->setColor(oldColor);          
  }
  
  void DrawSerie(AbstractTFTScreen* caller, Points& serie, RGBColor color)
  {
    if(serie.size() < 2 || !caller->isActive()) // низзя рисовать
      return;
     
      UTFT* dc = Screen.getDC();
      word oldColor = dc->getColor();  
    
      dc->setColor(color.R, color.G, color.B);
      doDrawSerie(dc,serie);        
      dc->setColor(oldColor);   
  }

  void DrawChart(AbstractTFTScreen* caller, Points& serie1, Points& serie2, Points& serie3, uint16_t serie1Color, uint16_t serie2Color, uint16_t serie3Color)
  {
    
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
  
    Drawing::DrawSerie(caller, serie1,serie1Color);
    yield();
    Drawing::DrawSerie(caller, serie2,serie2Color);
    yield();
    Drawing::DrawSerie(caller, serie3,serie3Color);
    yield();    
  }

  void ComputeChart(InterruptTimeList& list1, Points& serie1, InterruptTimeList& list2, Points& serie2, InterruptTimeList& list3, Points& serie3)
  {
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
      
      ComputeSerie(list1,serie1,xOffset, yOffset);
      yOffset += yOffsetStep;
      xOffset += xOffsetStep;
    
      ComputeSerie(list2,serie2,xOffset, yOffset);
      yOffset += yOffsetStep;
      xOffset += xOffsetStep;
    
      ComputeSerie(list3,serie3,xOffset, yOffset);    
  }
  
  void DrawGrid(int startX, int startY, int columnsCount, int rowsCount, int columnWidth, int rowHeight, RGBColor gridColor)
  {
    UTFT* dc = Screen.getDC();
    word color = dc->getColor();  
  
    dc->setColor(gridColor.R, gridColor.G, gridColor.B);
      
    for (int col = 0; col <= columnsCount; col++)
    {
        int colX = startX + col*columnWidth;
        dc->drawLine(colX , startY, colX , startY + rowHeight*rowsCount);
        yield();
    }
    for (int row = 0; row <= rowsCount; row++)
    {
        int rowY = startY + row*rowHeight;
        dc->drawLine(startX, rowY,  startX + columnWidth*columnsCount, rowY);
        yield();
    }
    
      dc->setColor(color);    
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ChartSerie
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ChartSerie::ChartSerie(Chart* parent, RGBColor color)
{
  parentChart = parent;
  serieColor = color;
  points = NULL;
  pointsCount = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ChartSerie::~ChartSerie()
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ChartSerie::setPoints(uint16_t* pointsArray, uint16_t countOfPoints)
{
  points = pointsArray;
  pointsCount = countOfPoints;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ChartSerie::drawLine(UTFT* dc,uint16_t x, uint16_t y, uint16_t x2, uint16_t y2)
{
   dc->drawLine(x,y,x2,y2);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ChartSerie::drawLine(UTFT* dc, uint16_t xPoint)
{
  if(!points || !pointsCount)
    return;

  if(xPoint < 1 || xPoint >= pointsCount)
    return;

  uint16_t startIdx = xPoint -1;
  uint16_t endIdx = xPoint;

  uint16_t maxPointY = parentChart->getMaxYValue(); 
  uint16_t maxY = parentChart->getYMax();
  
  word initialColor = dc->getColor();
  dc->setColor(serieColor.R,serieColor.G,serieColor.B);

  uint16_t startX = parentChart->getXCoord();
  uint16_t startY = parentChart->getYCoord();
  
  uint16_t lineX = startX + startIdx;
  uint16_t lineY = startY - map(points[startIdx],0,maxPointY,0,maxY);

  uint16_t lineX2 = startX + endIdx;
  uint16_t lineY2 = startY - map(points[endIdx],0,maxPointY,0,maxY);


  drawLine(dc,lineX,lineY,lineX2,lineY2);

  // сохраняем точку, с которой рисовали линию
  while(savedPixels.size() < xPoint)
  {
    savedPixels.push_back({0,0});
  }

  savedPixels[startIdx].x = lineX;
  savedPixels[startIdx].y = lineY;


  dc->setColor(initialColor);
  yield();
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ChartSerie::clearLine(UTFT* dc, uint16_t xPoint)
{
  if(xPoint >= pointsCount || !savedPixels.size())
    return;

  uint16_t startIdx = xPoint;
  uint16_t endIdx =  xPoint+1;

  if(endIdx >= savedPixels.size())
    return;

  drawLine(dc,savedPixels[startIdx].x,savedPixels[startIdx].y,savedPixels[endIdx].x,savedPixels[endIdx].y);
  yield();
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t ChartSerie::getMaxYValue()
{
  uint16_t result = 0;
  
  if(!points || !pointsCount)
    return result;

    for(uint16_t i=0;i<pointsCount;i++)
    {
      result = max(result,points[i]);
    }

    return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chart
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Chart::Chart()
{
  xCoord = 0;
  yCoord = 0;
  computedMaxYValue = 0;
  inDraw = false;
  stopped = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Chart::~Chart()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Chart::draw()
{
  if(inDraw)
  {
    stopDraw();
    
    while(inDraw)
      yield();
  }
  
  stopped = false;
  inDraw = true;

  size_t seriesSize = series.size();

  // вычисляем максимальное значение по Y
  computedMaxYValue = 0;
  for(size_t i=0;i<seriesSize;i++)
  {
    computedMaxYValue = max(computedMaxYValue,series[i]->getMaxYValue());
  }
  
  UTFT* dc = Screen.getDC();
  word oldColor = dc->getColor();
  dc->setColor(dc->getBackColor());
     
  for(int i=0;i<xPoints;i++)
  {
          
    for(size_t k=0;k<seriesSize;k++)
    {
      series[k]->clearLine(dc, i);
      
      if(stopped)
      {
        inDraw = false;
        dc->setColor(oldColor);
        return;
      }
    }
      
    for(size_t k=0;k<seriesSize;k++)
    {
      series[k]->drawLine(dc, i);
      
      if(stopped)
      {
        inDraw = false;
        dc->setColor(oldColor);
        return;
      }
    }
    
  }
  dc->setColor(oldColor);
  inDraw = false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Chart::clearSeries()
{
  for(size_t i=0;i<series.size();i++)
  {
    delete series[i]; 
  }

  series.empty();

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t Chart::getMaxYValue()
{
  return computedMaxYValue;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ChartSerie* Chart::addSerie(RGBColor color)
{
  ChartSerie* serie = new ChartSerie(this, color);
  series.push_back(serie);
  return serie;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Chart::setPoints(uint16_t pX, uint16_t pY)
{
    xPoints = pX;
    yPoints = pY;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


