//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screen1.h"
#include "DS3231.h"
#include "ConfigPin.h"
#include "ADCSampler.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ADCSampler sampler;
Screen1* mainScreen = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ADC_Handler()
{
  sampler.handleInterrupt();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loopADC()
{
  static bool dataHigh_old = false;
  
  if (sampler.available()) 
  {
    int bufferLength = 0;
    uint16_t* cBuf = sampler.getFilledBuffer(&bufferLength);    // Получить буффер с данными
    
    uint16_t countOfPoints = bufferLength/3;
    uint16_t* serie1 = new uint16_t[countOfPoints];
    uint16_t* serie2 = new uint16_t[countOfPoints];
    uint16_t* serie3 = new uint16_t[countOfPoints];
    uint16_t serieWriteIterator = 0;
    
    for (int i = 0; i < bufferLength; i = i + 3, serieWriteIterator++)                // получить результат измерения поканально, с интервалом 3
    {
   // if (sampler.dataHigh == true) break; // sampler.dataHigh == false;      // Если поступил сигнал превышения порога - прекратить вывод !! Не работает
      serie1[serieWriteIterator] = cBuf[i];
      serie2[serieWriteIterator] = cBuf[i+1];
      serie3[serieWriteIterator] = cBuf[i+2];
      /*
      Serial.print(cBuf[i]);
      Serial.print(",");
      Serial.print(cBuf[i + 1]);
      Serial.print(",");
      Serial.println(cBuf[i + 2]);
      */
    } // for
    //Serial.println(" ----");
    sampler.readBufferDone();                                  // все данные переданы в ком

    if(mainScreen && mainScreen->isActive())
    {
      mainScreen->addPoints(0,serie1,countOfPoints);
      mainScreen->addPoints(1,serie2,countOfPoints);
      mainScreen->addPoints(2,serie3,countOfPoints);
      
      mainScreen->DrawChart();
    }
    else
    {
      delete [] serie1;
      delete [] serie2;
      delete [] serie3;
    }
  }
  yield();

    if (dataHigh_old != sampler.dataHigh)
    {
      //Serial.println("Signal High*********************");
      dataHigh_old = sampler.dataHigh;
    }
   yield();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen1::Screen1() : AbstractTFTScreen("Main")
{
  oldsecond = 0;
  mainScreen = this;
  points1 = NULL;
  points2 = NULL;
  points3 = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doSetup(TFTMenu* menu)
{
	screenButtons->setSymbolFont(Various_Symbols_32x32);
	// тут настраиваемся, например, можем добавлять кнопки
	screenButtons->addButton(5, 140, 165, 30, "НАСТРОЙКИ");
	screenButtons->addButton(179, 130, 35, 40, "z", BUTTON_SYMBOL); // кнопка Часы 

	// ТУТ НАСТРАИВАЕМ НАШ ГРАФИК
	// устанавливаем ему начальные точки отсчёта координат

	chart.setCoords(5, 120);
  // говорим, какое у нас кол-во точек по X и по Y
  chart.setPoints(CHART_POINTS_COUNT,100);
	// добавляем наши тестовые графики, количеством 1

	serie1 = chart.addSerie({ 255,0,0 });     // первый график - красного цвета
	serie2 = chart.addSerie({ 0,0,255 });     // второй график - голубого цвета
	serie3 = chart.addSerie({ 255,255,0 });   // третий график - желтого цвета

  unsigned int samplingRate = 3000;   // Частота вызова (стробирования) АЦП 50мс
  sampler.begin(samplingRate);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::drawTime(TFTMenu* menu)
{
  DS3231Time tm = RealtimeClock.getTime();
  if (oldsecond != tm.second)
  {
      oldsecond = tm.second;
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

    String str = "RAM: ";
    str += getFreeMemory();
    Screen.print(str.c_str(), 10,123);
    
  }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doUpdate(TFTMenu* menu)
{
	
  drawTime(menu);
  loopADC();
	// тут обновляем внутреннее состояние
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doPointsSynchronization(ChartSerie* serie,uint16_t* points, uint16_t pointsCount)
{
  //Тут синхронизируем график, ища нужную нам точку, с которой мы его выводим

  if(pointsCount <= CHART_POINTS_COUNT)
  {
    // кол-во точек уже равно кол-ву точек на графике, синхронизировать начало - не получится
    serie->setPoints(points, pointsCount);
    return;
  }
  
  // у нас условия - ищем первый 0, после него ищём первый 2048. Если наййдено за 30 первых точек - выводим следующие 150.
  // если не найдено - просто выводим первые 150 точек
  
  const uint16_t lowBorder = 0; // нижняя граница, по которой ищем начало
  const uint16_t wantedBorder = 2048; // граница синхронизации
  const uint8_t maxPointToSeek = 30; // сколько точек просматриваем вперёд, для поиска значения синхронизации

  uint8_t iterator = 0;
  bool found = false;
  for(; iterator < maxPointToSeek; iterator++)
  {
    if(points[iterator] <= lowBorder)
    {
      // нашли нижнюю границу
      found = true;
      break;
    }
  }

  if(!found)
  {
    // нижняя граница не найдена, просто рисуем как есть
    serie->setPoints(points, min(CHART_POINTS_COUNT,pointsCount));
    return;
  }

  found = false;

  // теперь ищем нужную границу для синхронизации
  for(; iterator < maxPointToSeek; iterator++)
  {
    if(points[iterator] >= wantedBorder)
    {
      // нашли границу синхронизации
      found = true;
      break;
    }
  } // for

  if(!found)
  {
    // за maxPointToSeek мы так и не нашли значение синхронизации, выводим как есть
    serie->setPoints(points, min(CHART_POINTS_COUNT,pointsCount));
    return;
  }

  // нужная граница синхронизации найдена - выводим график, начиная с этой точки
  serie->setPoints(&(points[iterator]), min(CHART_POINTS_COUNT,( pointsCount - (&(points[iterator]) - points) ) ));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::addPoints(uint8_t serieNumber, uint16_t* points, uint16_t pointsCount)
{
  if(serieNumber > 2)
    return;

  switch(serieNumber)
  {
    case 0:
    {
      delete [] points1;
      points1 = points;
      doPointsSynchronization(serie1,points,pointsCount);
    }
    break;
    
    case 1:
    {
      delete [] points2;
      points2 = points;
      doPointsSynchronization(serie2,points,pointsCount);
    }
    break;

    case 2:
    {
      delete [] points3;
      points3 = points;
      doPointsSynchronization(serie3,points,pointsCount);
    }
    break;
    
  } // switch
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::DrawChart()
{
	// рисуем сетку
	int gridX = 5; // начальная координата сетки по X
	int gridY = 20; // начальная координата сетки по Y
	int columnsCount = 6; // 5 столбцов
	int rowsCount = 4; // 6 строк
	int columnWidth = 25; // ширина столбца
	int rowHeight = 25; // высота строки 
	RGBColor gridColor = { 0,200,0 }; // цвет сетки


	// вызываем функцию для отрисовки сетки, её можно вызывать из каждого класса экрана
	Drawing::DrawGrid(gridX, gridY, columnsCount, rowsCount, columnWidth, rowHeight, gridColor);

	chart.draw();// просим график отрисовать наши серии  

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doDraw(TFTMenu* menu)
{
  drawTime(menu);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  // обработчик нажатия на кнопку. Номера кнопок начинаются с 0 и идут в том порядке, в котором мы их добавляли
	if (pressedButton == 0)
	{
		menu->switchToScreen("SD"); // переключаемся на экран работы с SD
  }
	else if (pressedButton == 1)
	{
		menu->switchToScreen("SCREEN4"); // переключаемся на третий экран
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Screen1::getFreeMemory()
{
	struct mallinfo mi = mallinfo();
	char* heapend = _sbrk(0);
	register char* stack_ptr asm("sp");

	return (stack_ptr - heapend + mi.fordblks);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

