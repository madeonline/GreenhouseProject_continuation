//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Drawing.h"
#include "UTFTMenu.h"
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
void ChartSerie::drawLine(uint16_t xPoint)
{
  if(!points || !pointsCount)
    return;

  if(xPoint < 1 || xPoint >= pointsCount)
    return;

  uint16_t startIdx = xPoint -1;
  uint16_t endIdx = xPoint;

  uint16_t maxPointY = parentChart->getMaxYValue(); 
  uint16_t maxY = parentChart->getYMax();

  
  UTFT* dc = Screen.getDC();
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
void ChartSerie::clearLine(uint16_t xPoint)
{
  if(xPoint >= pointsCount || !savedPixels.size())
    return;

  uint16_t startIdx = xPoint;
  uint16_t endIdx =  xPoint+1;

  if(endIdx >= savedPixels.size())
    return;

  UTFT* dc = Screen.getDC();
  word initialColor = dc->getColor();
  dc->setColor(dc->getBackColor());

  drawLine(dc,savedPixels[startIdx].x,savedPixels[startIdx].y,savedPixels[endIdx].x,savedPixels[endIdx].y);


  dc->setColor(initialColor);
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
  
 // savedPixels = new SavedPixelsList;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Chart::~Chart()
{
 //  delete savedPixels; 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Chart::draw()
{
  stopped = false;
    
  for(int i=0;i<xPoints;i++)
  {
    for(size_t k=0;k<series.size();k++)
    {
      series[k]->clearLine(i);
      
      if(stopped)
        return;
    }
    
    for(size_t k=0;k<series.size();k++)
    {
      series[k]->drawLine(i);
      
      if(stopped)
        return;
    }
    
  }

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
  uint16_t result = 0;
  for(size_t i=0;i<series.size();i++)
  {
    result = max(result,series[i]->getMaxYValue());
  }
  return result;
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


