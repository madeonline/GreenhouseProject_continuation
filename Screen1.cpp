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
  chart.setPoints(150,100);
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
      serie1->setPoints(points, pointsCount);
    }
    break;
    
    case 1:
    {
      delete [] points2;
      points2 = points;
      serie2->setPoints(points, pointsCount);
    }
    break;

    case 2:
    {
      delete [] points3;
      points3 = points;
      serie3->setPoints(points, pointsCount);
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
  
 

 // DS3231Temperature  temp_clock = RealtimeClock.getTemperature();
 //
 // dc->printNumI(temp_clock.Value, 120, 105);
 // dc->printNumI(temp_clock.Fract, 20, 105);
 // рисуем сетку
 /*
  int gridX = 5; // начальная координата сетки по X
  int gridY = 20; // начальная координата сетки по Y
  int columnsCount = 6; // 5 столбцов
  int rowsCount = 4; // 6 строк
  int columnWidth = 25; // ширина столбца
  int rowHeight = 25; // высота строки 
  RGBColor gridColor = { 0,200,0 }; // цвет сетки
									// вызываем функцию для отрисовки сетки, её можно вызывать из каждого класса экрана
  Drawing::DrawGrid(gridX, gridY, columnsCount, rowsCount, columnWidth, rowHeight, gridColor);


  RGBColorRound gridRound = { 255,255,255 };     // цвет рамки
  RGBColorFill gridFill = { 255,0,0 };           // цвет заполнения
  Drawing::DrawPin(160, 45, 30, 20, gridRound, gridFill);
  Drawing::DrawPin(160, 73, 30, 20, gridRound, gridFill);
  Drawing::DrawPin(160, 100, 30, 20, gridRound, gridFill);
  dc->setColor(VGA_WHITE);
  dc->setBackColor(VGA_BLACK);
  dc->setFont(TFT_FONT);

  // тут рисуем, что надо именно нам, кнопки прорисуются сами после того, как мы тут всё отрисуем
  
  menu->print("1", 195, 48);
  menu->print("2", 195, 76);
  menu->print("3", 195, 103);
*/

 // DrawChart();   // рисуем графики
/*
  for (int i = 0; i<155; i++)
  {
	  //myGLCD.drawPixel(i, 88 + (sin(((i*6.65)*3.14) / 180) * 50));
	  //test[i][0] = i;
	  test[i][0] = 51 + (sin(((i*6.65)*3.14) / 180) * 50);
	  test[i][1] = 51 + (sin(((i*4.65)*3.14) / 180) * 30);
	  test[i][2] = 51 + (sin(((i*2.65)*3.14) / 180) * 40);
  }
*/
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

