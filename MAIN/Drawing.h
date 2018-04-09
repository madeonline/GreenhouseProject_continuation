#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <UTFT.h>
#include "TinyVector.h"
#include "InterruptHandler.h"
#include "UTFTMenu.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  uint8_t R,G,B;

} RGBColor;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  uint16_t X;
  uint16_t Y;
} Point;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<Point> Points;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AbstractTFTScreen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace Drawing
{
  void DrawGrid(int startX, int startY, int columnsCount, int rowsCount, int columnWidth, int rowHeight, RGBColor gridColor);
  void ComputeChart(InterruptTimeList& list1, Points& serie1, InterruptTimeList& list2, Points& serie2, InterruptTimeList& list3, Points& serie3);
  void DrawChart(AbstractTFTScreen* caller, Points& serie1, Points& serie2, Points& serie3, uint16_t serie1Color=VGA_WHITE, uint16_t serie2Color=VGA_BLUE, uint16_t serie3Color=VGA_YELLOW);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  int x;
  int y;
  
} ChartSavedPixel;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<ChartSavedPixel> SavedPixelsList;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Chart;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ChartSerie
{
  public:
    ChartSerie(Chart* parent, RGBColor color);
    ~ChartSerie();
    
    void setColor(RGBColor color)
    {
      serieColor = color;
    }

    void setPoints(uint16_t* pointsArray, uint16_t countOfPoints);
    uint16_t getPointsCount() {return pointsCount;}

    protected:

      friend class Chart;

      uint16_t getMaxYValue();

      void clearLine(UTFT* dc, uint16_t xPoint);
      void drawLine(UTFT* dc, uint16_t xPoint);
    

  private:

      void drawLine(UTFT* dc,uint16_t x, uint16_t y, uint16_t x2, uint16_t y2);

      uint16_t* points;
      uint16_t pointsCount;
      
      RGBColor serieColor;
      Chart* parentChart;

      SavedPixelsList savedPixels;
          
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<ChartSerie*> ChartSeries;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Chart
{
    public:
      Chart();
      ~Chart();

      void setCoords(uint16_t startX, uint16_t startY)
      {
        xCoord =  startX;
        yCoord = startY;
        
      }

      void setPoints(uint16_t pX, uint16_t pY);
      
      uint16_t getXCoord() { return xCoord; }
      uint16_t getYCoord() { return yCoord; }

       // очищает серии
      void clearSeries();
      
      // добавляет серию
      ChartSerie* addSerie(RGBColor color);

      // рисует все серии
      void draw();

     // прекращает отрисовку
     void stopDraw() { stopped = true; }

    protected:
    
      friend class ChartSerie;
      uint16_t getMaxYValue();
      
      uint16_t getYMax()
      {
        return yPoints;
      }

      uint16_t getXMax()
      {
        return xPoints;
      }


    private:

    bool stopped, inDraw;

    uint16_t xPoints, yPoints;
    byte* pixelsHits;

    uint16_t computedMaxYValue;
          
    uint16_t xCoord, yCoord;
    ChartSeries series;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

