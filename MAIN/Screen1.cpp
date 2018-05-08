//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screen1.h"
#include "DS3231.h"
#include "ConfigPin.h"
#include "ADCSampler.h"
#include "CONFIG.h"
#include "InterruptHandler.h"
#include "InterruptScreen.h"
#include "Settings.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen1* mainScreen = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ADC_Handler()
{
  adcSampler.handleInterrupt();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loopADC()
{
  static bool dataHigh_old = false;
  
  if (adcSampler.available()) 
  {
    int bufferLength = 0;
    uint16_t* cBuf = adcSampler.getFilledBuffer(&bufferLength);    // Получить буфер с данными


    static uint16_t countOfPoints = 0;    
    static uint16_t* serie1 = NULL;
    static uint16_t* serie2 = NULL;
    static uint16_t* serie3 = NULL;
    
    uint16_t currentCountOfPoints = bufferLength/6;

    if(currentCountOfPoints != countOfPoints)
    {
      countOfPoints = currentCountOfPoints;
      
      delete [] serie1;
      delete [] serie2;
      delete [] serie3;

      serie1 = new uint16_t[countOfPoints];
      serie2 = new uint16_t[countOfPoints];
      serie3 = new uint16_t[countOfPoints];

    }

    /*
    uint16_t countOfPoints = bufferLength/6;
    uint16_t* serie1 = new uint16_t[countOfPoints];
    uint16_t* serie2 = new uint16_t[countOfPoints];
    uint16_t* serie3 = new uint16_t[countOfPoints];
    */
    uint16_t serieWriteIterator = 0;

    uint32_t raw200V = 0;
    uint32_t raw5V = 0;
    uint32_t raw3V3 = 0;
    
    for (int i = 0; i < bufferLength; i = i + 6, serieWriteIterator++)                // получить результат измерения поканально, с интервалом 3
    {
   // if (sampler.dataHigh == true) break; // sampler.dataHigh == false;      // Если поступил сигнал превышения порога - прекратить вывод !! Не работает
      serie1[serieWriteIterator] = cBuf[i];          // Данные 1 графика
      serie2[serieWriteIterator] = cBuf[i+1];        // Данные 2 графика
      serie3[serieWriteIterator] = cBuf[i+2];        // Данные 3 графика

      raw200V += cBuf[i+3];        // Данные Измерение =200В
      raw3V3 += cBuf[i+4];         // Данные Измерение 3V3 
      raw5V += cBuf[i+5];          // Данные Измерение +5V 
      
	  } // for

    raw200V /= countOfPoints;
    raw3V3 /= countOfPoints;
    raw5V /= countOfPoints;

    Settings.set3V3RawVoltage(raw3V3);
    Settings.set5VRawVoltage(raw5V);
    Settings.set200VRawVoltage(raw200V);
      
    adcSampler.readBufferDone();                                  // все данные переданы в ком

    if(mainScreen && mainScreen->isActive())
    {
      mainScreen->requestToDrawChart(serie1, serie2, serie3, countOfPoints);      
    }
    /*
    else
    {
      delete [] serie1;
      delete [] serie2;
      delete [] serie3;
    }
    */
  }
    if (dataHigh_old != adcSampler.dataHigh)
    {
      dataHigh_old = adcSampler.dataHigh;
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen1::Screen1() : AbstractTFTScreen("Main")
{
  oldsecond = 0;
  mainScreen = this;
  points1 = NULL;
  points2 = NULL;
  points3 = NULL;
  canDrawChart = false;
  inDrawingChart = false;
  last3V3Voltage = last5Vvoltage = last200Vvoltage = -1;
  canLoopADC = false;

  inductiveSensorState1 = inductiveSensorState2 = inductiveSensorState3 = 0xFF;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::drawInductiveSensors(TFTMenu* menu)
{
  if(!isActive())
    return; 

  word color = VGA_RED;
  UTFT* dc = menu->getDC();
  
  word oldBackColor = dc->getBackColor();  
  word oldColor = dc->getColor();
  uint8_t* oldFont = dc->getFont();
  dc->setFont(SmallRusFont);

  uint16_t curX = 162;
  uint16_t curY = 20; 
  uint8_t boxSize = 20;   

  uint8_t curVal = Settings.getInductiveSensorState(0);
  if(inductiveSensorState1 != curVal)
  {

    inductiveSensorState1 = curVal;
    if(inductiveSensorState1)
      color = VGA_LIME;
    else
      color = VGA_RED;

    dc->setColor(color);
    dc->setBackColor(color);
    dc->fillRoundRect(curX, curY, curX + boxSize, curY + boxSize);
    dc->setColor(VGA_BLACK);
    menu->print("1", curX + 8, curY + 4);

  }

  curY += boxSize + 4;

  curVal = Settings.getInductiveSensorState(1);
  if(inductiveSensorState2 != curVal)
  {
    inductiveSensorState2 = curVal;
    if(inductiveSensorState2)
      color = VGA_LIME;
    else
      color = VGA_RED;

    dc->setColor(color);
    dc->setBackColor(color);
    dc->fillRoundRect(curX, curY, curX + boxSize, curY + boxSize);
    dc->setColor(VGA_BLACK);
    menu->print("2", curX + 8, curY + 4);

  }

  curY += boxSize + 4;  

  curVal = Settings.getInductiveSensorState(2);
  if(inductiveSensorState3 != curVal)
  {
    inductiveSensorState3 = curVal;
    if(inductiveSensorState3)
      color = VGA_LIME;
    else
      color = VGA_RED;

    dc->setColor(color);
    dc->setBackColor(color);
    dc->fillRoundRect(curX, curY, curX + boxSize, curY + boxSize);
    dc->setColor(VGA_BLACK);
    menu->print("3", curX + 8, curY + 4);

  }


  dc->setBackColor(oldBackColor);
  dc->setColor(oldColor);
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::drawVoltage(TFTMenu* menu)
{
  if(!isActive())
    return;

  word color = VGA_RED;
  UTFT* dc = menu->getDC();
  
  word oldColor = dc->getColor();  
  dc->setBackColor(VGA_BLACK);
  dc->setFont(SmallRusFont);
  uint8_t fontHeight = dc->getFontYsize();
  
  uint16_t curX = 190;
  uint16_t curY = 20;

  VoltageData vData = Settings.get3V3Voltage();// Контроль источника питания +3.3в

  if(last3V3Voltage != vData.raw)
  {
    last3V3Voltage = vData.raw;
    
    float threshold = (3.3/100)*VOLTAGE_THRESHOLD;
    float lowBorder = 3.3 - threshold;
    float highBorder = 3.3 + threshold;
  
    if(vData.voltage >= lowBorder && vData.voltage <= highBorder)
      color = VGA_LIME;
  
    String data = String(vData.voltage,1)+"V";
    while(data.length() < 4)
      data += ' ';
    
    dc->setColor(color);
    dc->print(data.c_str(),curX,curY);
  }
  
  curY += fontHeight + 2;
  
  
  vData = Settings.get5Vvoltage();        // Контроль источника питания +5.0в

  if(last5Vvoltage != vData.raw)
  {
    last5Vvoltage = vData.raw;
    
    float threshold = (5.0/100)*VOLTAGE_THRESHOLD;
    float lowBorder = 5.0 - threshold;
    float highBorder = 5.0 + threshold;
  
    color = VGA_RED;
  
    if(vData.voltage >= lowBorder && vData.voltage <= highBorder)
      color = VGA_LIME;
  
    String data = String(vData.voltage,1) + "V";
    while(data.length() < 4)
      data += ' ';
  
    dc->setColor(color);  
    dc->print(data.c_str(),curX,curY);
  }
  
  curY += fontHeight + 2;

  
  vData = Settings.get200Vvoltage();      // Контроль источника питания 200в

  if(last200Vvoltage != vData.raw)
  {
    last200Vvoltage = vData.raw;
    
    float threshold = (200.0/100)*VOLTAGE_THRESHOLD;
    float lowBorder = 200.0 - threshold;
    float highBorder = 200.0 + threshold;
  
    color = VGA_RED;
  
    if(vData.voltage >= lowBorder && vData.voltage <= highBorder)
      color = VGA_LIME;
  
    String data = String((uint16_t)vData.voltage) + "V";
    while(data.length() < 4)
      data += ' ';
  
    dc->setColor(color);  
    dc->print(data.c_str(),curX,curY);
  }

  dc->setColor(oldColor);

   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::onDeactivate()
{
  // станем неактивными, надо выключить подписчика результатов прерываний
  InterruptHandler.setSubscriber(NULL);
  
  last3V3Voltage = last5Vvoltage = last200Vvoltage = -1;

  inductiveSensorState1 = inductiveSensorState2 = inductiveSensorState3 = 0xFF;
  
  // прекращаем отрисовку графика
  chart.stopDraw();

  inDrawingChart = false;
  canDrawChart = false;
  canLoopADC = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::onActivate()
{
  canLoopADC = true;
  // мы активизируемся, назначаем подписчика результатов прерываний
  InterruptHandler.setSubscriber(ScreenInterrupt);
  
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

  unsigned int samplingRate = 2500;   // Частота вызова (стробирования) АЦП 50мс
  
  adcSampler.setLowBorder(Settings.getTransformerLowBorder());
  adcSampler.setHighBorder(Settings.getTransformerHighBorder());
  
  adcSampler.begin(samplingRate);  
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
  drawVoltage(menu);
  drawInductiveSensors(menu);
  drawChart();

  if(canLoopADC)
    loopADC();
	// тут обновляем внутреннее состояние
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t Screen1::getSynchroPoint(uint16_t* points, uint16_t pointsCount)
{
 //Тут синхронизируем график, ища нужную нам точку, с которой мы его выводим
  const uint16_t lowBorder = 100; // нижняя граница, по которой ищем начало
  const uint16_t wantedBorder = 2048; // граница синхронизации
  const uint8_t maxPointToSeek = 48; // сколько точек просматриваем вперёд, для поиска значения синхронизации

  if(pointsCount <= CHART_POINTS_COUNT || pointsCount <= maxPointToSeek)
  {
    // кол-во точек уже равно кол-ву точек на графике, синхронизировать начало - не получится
   // DBGLN(F("Too many points!!!!"));
    return 0;
  }
  
  // у нас условия - ищем первый 0, после него ищём первый 2048. Если найдено за 30 первых точек - выводим следующие 150.
  // если не найдено - просто выводим первые 150 точек
  

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
    //DBGLN(F("Low border not found!!!!"));
    return 0;
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
    //DBGLN(F("High border not found!!!!"));
    return 0;
  }

  //DBGLN(F("Found shift: "));
  //DBGLN((&(points[iterator]) - points));

  // нужная граница синхронизации найдена - выводим график, начиная с этой точки
 return ( (&(points[iterator]) - points) );  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::requestToDrawChart(uint16_t* _points1,   uint16_t* _points2,  uint16_t* _points3, uint16_t pointsCount)
{
  if(inDrawingChart)
  {
    chart.stopDraw();
  }

  canDrawChart = true;
  inDrawingChart = false;
  
  points1 = _points1;
  points2 = _points2;
  points3 = _points3;

  int shift = getSynchroPoint(points1,pointsCount);
  int totalPoints = min(CHART_POINTS_COUNT, (pointsCount - shift));

  serie1->setPoints(&(points1[shift]), totalPoints);
  serie2->setPoints(&(points2[shift]), totalPoints);
  serie3->setPoints(&(points3[shift]), totalPoints);
    
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::drawChart()
{
  if(!isActive() || !canDrawChart || inDrawingChart)
    return;

  inDrawingChart = true;    
	// рисуем сетку
	int gridX = 5; // начальная координата сетки по X
	int gridY = 20; // начальная координата сетки по Y
	int columnsCount = 6; // 5 столбцов
	int rowsCount = 4; // 6 строк
	int columnWidth = 25; // ширина столбца
	int rowHeight = 25; // высота строки 
	RGBColor gridColor = { 0,200,0 }; // цвет сетки


  static uint32_t fpsMillis = 0;
  uint32_t now = millis();

  if(now - fpsMillis > (1000/CHART_FPS) )
  {
  	// вызываем функцию для отрисовки сетки, её можно вызывать из каждого класса экрана
  	Drawing::DrawGrid(gridX, gridY, columnsCount, rowsCount, columnWidth, rowHeight, gridColor);
  
  	chart.draw();// просим график отрисовать наши серии

   fpsMillis = millis();
  }

  inDrawingChart = false;
  canDrawChart = false;

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doDraw(TFTMenu* menu)
{
  drawTime(menu);

  // рисуем версию ПО
  UTFT* dc = menu->getDC();
  dc->setColor(VGA_WHITE);
  dc->setBackColor(VGA_BLACK);
  dc->setFont(SmallRusFont);

  uint16_t w = dc->getDisplayXSize();
  uint8_t fw = dc->getFontXsize();
  String str = SOFTWARE_VERSION;

  int strL = menu->print(str.c_str(),0,0,0,true);
  int strW = strL*fw;

  int top = 1;
  int left = w - strW - 3;

  menu->print(str.c_str(),left,top);
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  // обработчик нажатия на кнопку. Номера кнопок начинаются с 0 и идут в том порядке, в котором мы их добавляли
	if (pressedButton == 0)
	{
		menu->switchToScreen("Settings"); // переключаемся на экран работы с SD
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

