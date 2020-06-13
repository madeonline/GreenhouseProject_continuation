//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screen1.h"
#include "DS3231.h"
#include "ConfigPin.h"
#include "ADCSampler.h"
#include "CONFIG.h"
#include "InterruptHandler.h"
#include "InterruptScreen.h"
#include "Settings.h"
#include "Relay.h"
#include "Utils.h"
#include "PulsesGen.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen1* mainScreen = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// настройки по измерению тока
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8_t CURRENT_NUM_SAMPLES = 10; // за сколько измерений вычислять ток?

const float COEFF_1 = 5.0; // первый коэффициент по пересчёту тока
const float COEFF_2 = 3.795; // второй коэффициент по пересчёту тока

const uint32_t CURRENT_DIVIDER = 1000; // делитель для пересчёта напряжения в ток
const uint32_t CURRENT_MIN_TREAT_AS_ZERO = 900; // минимальное значение тока, которое интерпретируется как 0

const uint16_t CURRENT_DRAW_X_COORD = 165; // координата по X для начала отрисовки значений токов по каналам
const uint16_t CURRENT_DRAW_Y_COORD = 67;  // координата по Y для начала отрисовки значений токов по каналам

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// служебная информация по измерению тока
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t redCurrentInfoMax = 0; // макс. данные по току, канал 1 (красный)
uint32_t redCurrentInfoMin = 0; // мин. данные по току, канал 1 (красный)

uint32_t blueCurrentInfoMax = 0; // макс. данные по току, канал 2 (синий)
uint32_t blueCurrentInfoMin = 0; // мин. данные по току, канал 2 (синий)

uint32_t yellowCurrentInfoMax = 0; // макс. данные по току, канал 3 (желтый)
uint32_t yellowCurrentInfoMin = 0; // мин. данные по току, канал 3 (желтый)

uint8_t currentNumSamples = 0; // кол-во семплов измерений по току

uint16_t channel1Current = 0; // ток канала 1
uint16_t channel2Current = 0; // ток канала 2
uint16_t channel3Current = 0; // ток канала 3
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loopADC()
{
#ifndef _ADC_OFF

  static bool dataHigh_old = false;
  
  if (adcSampler.available()) 
  {
    int bufferLength = 0;
    uint16_t* cBuf = adcSampler.getFilledBuffer(&bufferLength);    // Получить буфер с данными


    static uint16_t countOfPoints = 0;    
    static uint16_t* serie1 = NULL;
    static uint16_t* serie2 = NULL;
    static uint16_t* serie3 = NULL;
    
    uint16_t currentCountOfPoints = bufferLength/ NUM_CHANNELS;

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

 
    uint16_t serieWriteIterator = 0;

    uint32_t raw200V = 0;
    uint32_t raw5V = 0;
    uint32_t raw3V3 = 0;

    
    for (int i = 0; i < bufferLength; i = i + NUM_CHANNELS, serieWriteIterator++)                // получить результат измерения поканально, с интервалом 3
    {
	  serie1[serieWriteIterator] = cBuf[i + 4];        // Данные 1 графика  (красный)
	  serie2[serieWriteIterator] = cBuf[i + 3];        // Данные 2 графика  (синий)
	  serie3[serieWriteIterator] = cBuf[i + 2];        // Данные 3 графика  (желтый)

	  //raw3V3  += cBuf[i + 7];                          // Данные Измерение 3V3 
	  //raw5V   += cBuf[i + 8];                          // Данные Измерение +5V 
	  //raw200V += cBuf[i + 6];                          // Данные Измерение =200В


	  } // for


   // у нас заполнен массив показаний, можно считать ток.
    // для этого собираем максимальные и минимальные значения по каждому из каналов,
    // и плюсуем их. Как только наберём нужное кол-во семплов - работаем дальше.
    uint32_t ch1Min = 0xFFFFFFFF, ch1Max = 0, ch2Min = 0xFFFFFFFF, ch2Max = 0, ch3Min = 0xFFFFFFFF, ch3Max = 0;
    
    for(uint16_t i=0;i<countOfPoints;i++)
    {
              ch1Min = min(ch1Min,serie1[i]);
              ch2Min = min(ch2Min,serie2[i]);
              ch3Min = min(ch3Min,serie3[i]);

              ch1Max = max(ch1Max,serie1[i]);
              ch2Max = max(ch2Max,serie2[i]);
              ch3Max = max(ch3Max,serie3[i]);
              
    } // for

    if(ch1Min == 0xFFFFFFFF)
    {
      ch1Min = ch1Max;
    }

    if(ch2Min == 0xFFFFFFFF)
    {
      ch2Min = ch2Max;
    }

    if(ch3Min == 0xFFFFFFFF)
    {
      ch3Min = ch3Max;
    }

    // плюсуем полученные значения в накопительную часть
    redCurrentInfoMin += ch1Min;
    blueCurrentInfoMin += ch2Min;
    yellowCurrentInfoMin += ch3Min;

    redCurrentInfoMax += ch1Max;
    blueCurrentInfoMax += ch2Max;
    yellowCurrentInfoMax += ch3Max;

    // проверяем, собрали ли нужное кол-во семплов?
    currentNumSamples++;

    if(currentNumSamples >= CURRENT_NUM_SAMPLES)
    {
      // собрали нужное кол-во семплов, можно вычислять ток по каналам.

      // Вычисляем среднее делением на Х. От максимального отнимаем минимальное - получаем размах. Это будет величина переменного тока. 
      // Вернее, измеренное напряжение, которое мы потом преобразуем в ток из расчета 3 вольта равны 5 амперам.
      
      uint32_t channel1Avg = redCurrentInfoMax/CURRENT_NUM_SAMPLES - redCurrentInfoMin/CURRENT_NUM_SAMPLES;
      uint32_t channel2Avg = blueCurrentInfoMax/CURRENT_NUM_SAMPLES - blueCurrentInfoMin/CURRENT_NUM_SAMPLES;
      uint32_t channel3Avg = yellowCurrentInfoMax/CURRENT_NUM_SAMPLES - yellowCurrentInfoMin/CURRENT_NUM_SAMPLES;

      // вычислили напряжение, теперь вычисляем ток по формуле: 3В = 5А. Для этого напряжение надо умножить на 5, и разделить на 3
      
      channel1Current = (COEFF_1*channel1Avg)/COEFF_2;
      channel2Current = (COEFF_1*channel2Avg)/COEFF_2;
      channel3Current = (COEFF_1*channel3Avg)/COEFF_2;

      // отсекаем минимальный нижний порог
      if(channel1Current <= CURRENT_MIN_TREAT_AS_ZERO)
      {
        channel1Current = 0;
      }

      if(channel2Current <= CURRENT_MIN_TREAT_AS_ZERO)
      {
        channel2Current = 0;
      }

      if(channel3Current <= CURRENT_MIN_TREAT_AS_ZERO)
      {
        channel3Current = 0;
      }


      // не забываем чистить за собой, подготавливая к следующему обновлению
      currentNumSamples = 0;
      
      redCurrentInfoMin = 0;
      blueCurrentInfoMin = 0;
      yellowCurrentInfoMin = 0;

      redCurrentInfoMax = 0;
      blueCurrentInfoMax = 0;
      yellowCurrentInfoMax = 0;
      
    } // if
    
    

   /* raw200V /= countOfPoints;
    raw3V3 /= countOfPoints;
    raw5V /= countOfPoints;

    Settings.set3V3RawVoltage(raw3V3);
    Settings.set5VRawVoltage(raw5V);
    Settings.set200VRawVoltage(raw200V);*/
      
    adcSampler.reset();                                  // все данные переданы в ком

    if(mainScreen && mainScreen->isActive())
    {
      mainScreen->requestToDrawChart(serie1, serie2, serie3, countOfPoints);      
    }

  }
    if (dataHigh_old != adcSampler.dataHigh)
    {
      dataHigh_old = adcSampler.dataHigh;
    }
#endif // !_ADC_OFF
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
//  last3V3Voltage = last5Vvoltage = last200Vvoltage = -1;
  canLoopADC = false;
  oldChannel1Current = oldChannel2Current = oldChannel3Current = 0xFFFF;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::startGeneratePulses()
{
  // делаем список импульсов энкодера
  encoderAPulses.clear();
  encoderBPulses.clear();

  // импульсы по линии A энкодера, паузы между сменой уровня на выводе, в микросекундах
  encoderAPulses.push_back(100);
  encoderAPulses.push_back(200);
  encoderAPulses.push_back(300);
  encoderAPulses.push_back(400);
  encoderAPulses.push_back(500);
  encoderAPulses.push_back(600);
  encoderAPulses.push_back(500);
  encoderAPulses.push_back(400);
  encoderAPulses.push_back(300);
  encoderAPulses.push_back(200);
  encoderAPulses.push_back(100);

  // импульсы по линии B энкодера, паузы между сменой уровня на выводе, в микросекундах
  encoderBPulses.push_back(200);
  encoderBPulses.push_back(300);
  encoderBPulses.push_back(400);
  encoderBPulses.push_back(500);
  encoderBPulses.push_back(600);
  encoderBPulses.push_back(700);
  encoderBPulses.push_back(600);
  encoderBPulses.push_back(500);
  encoderBPulses.push_back(400);
  encoderBPulses.push_back(300);
  encoderBPulses.push_back(200);


  // запускаем генерацию импульсов
  ImpulseGeneratorA.start(encoderAPulses);
  ImpulseGeneratorB.start(encoderBPulses);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::onDeactivate()
{
  // станем неактивными, надо выключить подписчика результатов прерываний
  InterruptHandler.setSubscriber(NULL);
  
 // last3V3Voltage = last5Vvoltage = last200Vvoltage = -1;
  
  // прекращаем отрисовку графика
  chart.stopDraw();
  inDrawingChart = false;
  canDrawChart = false;

#ifndef _ADC_OFF
	canLoopADC = false;
#endif // !_ADC_OFF

  DBGLN(F("MainScreen::onDeactivate()"));
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::onActivate()
{
#ifndef _ADC_OFF
  canLoopADC = true;
#endif // !_ADC_OFF


  oldChannel1Current = oldChannel2Current = oldChannel3Current = 0xFFFF;
  oldCurrentString1 = oldCurrentString2 = oldCurrentString3 = "";


    sensor1DisplayString = "";
    sensor2DisplayString = "";
    
    sensor1Temperature.reset();
    sensor2Temperature.reset();


    oldFreeMemory = -1;
    oldFreeMemCaption = "";

    relAll_State = Relay_LineALL.isOn();
    relLineA_State = Relay_LineA.isOn();
    relLineB_State = Relay_LineB.isOn();
    relLineC_State = Relay_LineC.isOn();
    relShunt1_State = Relay_Shunt1.isOn();
    relShunt2_State = Relay_Shunt2.isOn();


    if(relAll_State)
    {
      screenButtons->setButtonBackColor(relAllButton,VGA_GREEN);
    }
    else
    {
      screenButtons->setButtonBackColor(relAllButton,VGA_BLUE);      
    }


    if(relShunt1_State)
    {
      screenButtons->setButtonBackColor(shunt1Button,VGA_GREEN);
    }
    else
    {
      screenButtons->setButtonBackColor(shunt1Button,VGA_BLUE);      
    }    

    if(relShunt2_State)
    {
      screenButtons->setButtonBackColor(shunt2Button,VGA_GREEN);
    }
    else
    {
      screenButtons->setButtonBackColor(shunt2Button,VGA_BLUE);      
    }    

  // мы активизируемся, назначаем подписчика результатов прерываний
  InterruptHandler.setSubscriber(ScreenInterrupt);

  DBGLN(F("MainScreen::onActivate()"));
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doSetup(TFTMenu* menu)
{
#ifndef _DISABLE_MAIN_SCREEN_BUTTONS
	screenButtons->setSymbolFont(Various_Symbols_32x32);
  
	// тут настраиваемся, например, можем добавлять кнопки
	settingsButton = screenButtons->addButton(290, 210, 175, 40, "НАСТРОЙКИ");

	// Кнопки подачи тока в УРОВ. При включении/отключении изменять цвет заполнения кнопки 


	encoderButton = screenButtons->addButton(5, 210, 80, 40, "ЭНК");    // включить генерацию импульсов энкодера
	screenButtons->addButton(95, 210, 80, 40, "---");   // кнопка 
	endButton = screenButtons->addButton(185, 210, 80, 40, "END");  // кнопка выхода из просмотра редультатов теста

	relAllButton = screenButtons->addButton(5, 260, 80, 40, "ТОК");    // включить ток на линии rele_lineAll
	shunt1Button = screenButtons->addButton(95, 260, 80, 40, "3,6А");  // кнопка шунт1 rele_shunt1
	shunt2Button = screenButtons->addButton(185, 260, 80, 40, "5,2А"); // кнопка шунт2 rele_shunt2


#endif // !_DISABLE_MAIN_SCREEN_BUTTONS

	// ТУТ НАСТРАИВАЕМ НАШ ГРАФИК
	// устанавливаем ему начальные точки отсчёта координат

	chart.setCoords(INTERRUPT_CHART_X_COORD, INTERRUPT_CHART_Y_COORD);
	// говорим, какое у нас кол-во точек по X и по Y
	chart.setPoints(CHART_POINTS_COUNT,100);
	// добавляем наши тестовые графики, количеством 1

	serie1 = chart.addSerie({ 255,0,0 });     // первый график - красного цвета
	serie2 = chart.addSerie({ 0,0,255 });     // второй график - голубого цвета
	serie3 = chart.addSerie({ 255,255,0 });   // третий график - желтого цвета

#ifndef _ADC_OFF
	unsigned int samplingRate = 2500;   // Частота вызова (стробирования) АЦП 50мс  

	adcSampler.setLowBorder(Settings.getTransformerLowBorder());
	adcSampler.setHighBorder(Settings.getTransformerHighBorder());
  
	adcSampler.begin(samplingRate);  
#endif
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::drawCurrent(TFTMenu* menu)
{

  if(!isActive())
  {
    return;      
  }  

   UTFT* dc = menu->getDC();
   dc->setFont(BigRusFont);
    
  uint8_t fontHeight = dc->getFontYsize();
  const uint8_t y_spacing = 5;
  
  uint16_t curX = CURRENT_DRAW_X_COORD;
  uint16_t curY = CURRENT_DRAW_Y_COORD;  

  word bgcolor = VGA_BLACK;
  word fgcolor = VGA_RED;

   dc->setBackColor(bgcolor);

  // не забываем, что раз у нас разрядность АЦП - в микровольтах (3.3/4095 = 0.0008 В), то и результат у нас - в микроамперах  
  if(oldChannel1Current != channel1Current)
  {
//    DBG("CHANNEL 1 CURRENT: "); DBGLN(channel1Current);

    dc->setColor(bgcolor);        
    menu->print(oldCurrentString1.c_str(),curX,curY);
    
    dc->setColor(fgcolor);    
    oldChannel1Current = channel1Current;
    oldCurrentString1 = formatFloat(float(oldChannel1Current)/CURRENT_DIVIDER,2);
    oldCurrentString1 += "A";

    menu->print(oldCurrentString1.c_str(),curX,curY);
  }

  fgcolor = VGA_BLUE;
  curY += fontHeight + y_spacing;

 if(oldChannel2Current != channel2Current)
  {
//    DBG("CHANNEL 2 CURRENT: "); DBGLN(channel2Current);
    
    dc->setColor(bgcolor);
    menu->print(oldCurrentString2.c_str(),curX,curY);
    
    oldChannel2Current = channel2Current;
    oldCurrentString2 = formatFloat(float(oldChannel2Current)/CURRENT_DIVIDER,2);
    oldCurrentString2 += "A";

    dc->setColor(fgcolor);
    menu->print(oldCurrentString2.c_str(),curX,curY);
  }  

 fgcolor = VGA_YELLOW;

  curY += fontHeight + y_spacing; 

 if(oldChannel3Current != channel3Current)
  {

//    DBG("CHANNEL 3 CURRENT: "); DBGLN(channel3Current);
    

    dc->setColor(bgcolor);
    menu->print(oldCurrentString3.c_str(),curX,curY);
    
    oldChannel3Current = channel3Current;
    oldCurrentString3 = formatFloat(float(oldChannel3Current)/CURRENT_DIVIDER,2);
    oldCurrentString3 += "A";

    dc->setColor(fgcolor);
    menu->print(oldCurrentString3.c_str(),curX,curY);
  }    

 dc->setFont(SmallRusFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::drawTemperature(TFTMenu* menu, bool anyway)
{
    UTFT* dc = menu->getDC();
    dc->setBackColor(VGA_BLACK);
    dc->setFont(BigRusFont);
  
    DS18B20Temperature t1 = TempSensors.getTemperature(0);
    DS18B20Temperature t2 = TempSensors.getTemperature(1);

    if(anyway || t1 != sensor1Temperature)
    {
      sensor1Temperature = t1;

      // рисуем данные с датчика
      if(sensor1DisplayString != "")
      {
        dc->setColor(VGA_BLACK);
        dc->print(sensor1DisplayString.c_str(), SENSOR_1_DRAW_X, SENSOR_1_DRAW_Y);
        
      }

      if(hasSensor1Alarm())
      {
        dc->setColor(VGA_RED);
      }
      else
      {
        dc->setColor(VGA_WHITE);
      }

      sensor1DisplayString = "T1:";
      sensor1DisplayString += sensor1Temperature;
      if(sensor1Temperature.hasData())
      {
        sensor1DisplayString += char(127); // значок градуса
        sensor1DisplayString += "C"; // значок градуса
      }
      dc->print(sensor1DisplayString.c_str(), SENSOR_1_DRAW_X, SENSOR_1_DRAW_Y);
    }


    if(anyway || t2 != sensor2Temperature)
    {
      sensor2Temperature = t2;
      
      // рисуем данные с датчика
      if(sensor2DisplayString != "")
      {
        dc->setColor(VGA_BLACK);
        dc->print(sensor2DisplayString.c_str(), SENSOR_2_DRAW_X, SENSOR_2_DRAW_Y);
      }

      if(hasSensor2Alarm())
      {
        dc->setColor(VGA_RED);
      }
      else
      {
        dc->setColor(VGA_WHITE);
      }


      sensor2DisplayString = "T2:";
      sensor2DisplayString += sensor2Temperature;
      
      if(sensor2Temperature.hasData())
      {
        sensor2DisplayString += char(127); // значок градуса
        sensor2DisplayString += "C"; // значок градуса
      }
      
      dc->print(sensor2DisplayString.c_str(), SENSOR_2_DRAW_X, SENSOR_2_DRAW_Y);
    }    


  dc->setFont(SmallRusFont);   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::drawTime(TFTMenu* menu)
{
    UTFT* dc = menu->getDC();
    dc->setColor(VGA_WHITE);
    dc->setBackColor(VGA_BLACK);
    dc->setFont(SmallRusFont);
  
#ifndef _DISABLE_DRAW_TIME
    DS3231Time tm = RealtimeClock.getTime();
    if (oldsecond != tm.second)
    {
        oldsecond = tm.second;
      // получаем компоненты даты в виде строк
      String strDate = RealtimeClock.getDateStr(tm);
      String strTime = RealtimeClock.getTimeStr(tm);
  
      // печатаем их
      dc->print(strDate.c_str(), 5, 21);
      dc->print(strTime.c_str(), 90, 21);
        
    }

#endif // !_DISABLE_DRAW_TIME


#ifndef _DISABLE_DRAW_RAM_ON_SCREEN

        int freeMem = getFreeMemory();
        
        if(oldFreeMemory != freeMem)
        {
          oldFreeMemory = freeMem;

          if(oldFreeMemCaption != "") // инфа по свободной памяти изменилась, стираем надпись фоновым цветом
          {
            dc->setColor(VGA_BLACK);
            menu->print(oldFreeMemCaption.c_str(), freeMemX,freeMemY);
            dc->setColor(VGA_WHITE);
            oldFreeMemCaption = "";
          }
          
          oldFreeMemCaption = "RAM: ";
          oldFreeMemCaption += freeMem;
    
          uint16_t w = dc->getDisplayXSize();
          uint8_t fw = dc->getFontXsize();

          int strL = menu->print(oldFreeMemCaption.c_str(),0,0,0,true);
          int strW = strL*fw;
        
          freeMemY = 1;
          freeMemX = w - strW - 3;
      
          menu->print(oldFreeMemCaption.c_str(), freeMemX,freeMemY);
        }

#endif // !_DISABLE_DRAW_RAM_ON_SCREEN


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doUpdate(TFTMenu* menu)
{
	
  drawTime(menu);
  drawRelayState(menu);
  drawTemperature(menu);
  drawCurrent(menu);
  drawChart();

#ifndef _ADC_OFF
  if(canLoopADC)
    loopADC();
#endif // !_ADC_OFF
	// тут обновляем внутреннее состояние
 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Screen1::hasSensor1Alarm()
{
 if(sensor1Temperature.hasData())
  {
      int t = sensor1Temperature.Whole;
      if(sensor1Temperature.Negative)
      {
        t = -t;
      }

      if(t >= Settings.getTemperatureAlertBorder())
      {
        return true;
      }
  }  

  return false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Screen1::hasSensor2Alarm()
{
 if(sensor2Temperature.hasData())
  {
      int t = sensor2Temperature.Whole;
      if(sensor2Temperature.Negative)
      {
        t = -t;
      }

      if(t >= Settings.getTemperatureAlertBorder())
      {
        return true;
      }
  }  

  return false;
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
	int gridY = 40; // начальная координата сетки по Y
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
void Screen1::drawRelayState(TFTMenu* menu, bool anyway)
{
    bool r_all = Relay_LineALL.isOn();
    bool r_a = Relay_LineA.isOn();
    bool r_b = Relay_LineB.isOn();
    bool r_c = Relay_LineC.isOn();
    bool r_shunt1 = Relay_Shunt1.isOn();
    bool r_shunt2 = Relay_Shunt2.isOn();
    

    UTFT* dc = menu->getDC();
    dc->setBackColor(VGA_BLACK);
	//dc->setColor(VGA_WHITE);
	//dc->setBackColor(VGA_BLACK);
	dc->setFont(BigRusFont);

    uint16_t x = RELAY_DRAW_COORD_X;
    uint16_t y = RELAY_DRAW_COORD_Y;
    const uint8_t spacer = 5;

    // красная кнопка 
 	if (anyway || relLineA_State != r_a)
	{
	   relLineA_State = r_a;

	  if (relLineA_State)
      {
        dc->setColor(VGA_RED);
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
		dc->setColor(VGA_WHITE);
		dc->setBackColor(VGA_RED);
		dc->print("A", x + 12, y + 12);
      }
      else
      {
		
        dc->setColor(VGA_BLACK);
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);

		dc->setColor(VGA_WHITE);
		dc->setBackColor(VGA_BLACK);
		dc->print("A", x + 12, y + 12);
        
        dc->setColor(VGA_RED);
        dc->drawRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
      }
    }

    x += RELAY_BOX_SIZE + spacer;

    // синяя кнопка
 	if (anyway || relLineB_State != r_b)
	{
	  relLineB_State = r_b;

	  if (relLineB_State)
      {
        dc->setColor(VGA_BLUE);
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
		dc->setColor(VGA_WHITE);
		dc->setBackColor(VGA_BLUE);
		dc->print("B", x + 12, y + 12);
      }
      else
      {
        dc->setColor(VGA_BLACK);
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);

		dc->setColor(VGA_WHITE);
		dc->setBackColor(VGA_BLACK);
		dc->print("B", x + 12, y + 12);

        
        dc->setColor(VGA_BLUE);
        dc->drawRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
      }
    }

    x += RELAY_BOX_SIZE + spacer;

    // серая кнопка
	if (anyway || relLineC_State != r_c)
	{
		relLineC_State = r_c;

		if (relLineC_State)
      {
        dc->setColor(VGA_WHITE);        
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
		dc->setColor(VGA_BLACK);
		dc->setBackColor(VGA_WHITE);
		dc->print("C", x + 12, y + 12);
      }
      else
      {
        dc->setColor(VGA_BLACK);
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);

		dc->setColor(VGA_WHITE);
		dc->setBackColor(VGA_BLACK);
		dc->print("C", x + 12, y + 12);

        dc->setColor(VGA_WHITE);
        dc->drawRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
      }
    }

   x += RELAY_BOX_SIZE + spacer;

    // черная кнопка
    if (anyway || relAll_State != r_all)
    {
	   relAll_State = r_all;

	   if (relAll_State)
      {
        dc->setColor(VGA_GRAY);
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);

		dc->setColor(VGA_WHITE);
		dc->setBackColor(VGA_GRAY);
		dc->print("S", x + 12, y + 12);

		screenButtons->setButtonBackColor(relAllButton,VGA_GREEN);
	    screenButtons->drawButton(relAllButton);
      }
      else
      {
        dc->setColor(VGA_BLACK);
        dc->fillRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
        
        dc->setColor(VGA_GRAY);
        dc->drawRect(x,y,x+RELAY_BOX_SIZE,y+RELAY_BOX_SIZE);
		dc->setColor(VGA_WHITE);
		dc->setBackColor(VGA_BLACK);
		dc->print("S", x + 12, y + 12);

		screenButtons->setButtonBackColor(relAllButton,VGA_BLUE);
		screenButtons->drawButton(relAllButton);
      }
    }

    x += RELAY_BOX_SIZE + spacer;    


    if(r_shunt1 != relShunt1_State)
    {
      relShunt1_State = r_shunt1;
      
      if(relShunt1_State)
      {
        screenButtons->setButtonBackColor(shunt1Button,VGA_GREEN);
      }
      else
      {
        screenButtons->setButtonBackColor(shunt1Button,VGA_BLUE);        
      }
      screenButtons->drawButton(shunt1Button);      
    }

    if(r_shunt2 != relShunt2_State)
    {
      relShunt2_State = r_shunt2;
      
      if(relShunt2_State)
      {
        screenButtons->setButtonBackColor(shunt2Button,VGA_GREEN);
      }
      else
      {
        screenButtons->setButtonBackColor(shunt2Button,VGA_BLUE);        
      }
      screenButtons->drawButton(shunt2Button);      
    }    
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::doDraw(TFTMenu* menu)
{
  drawTime(menu);
  drawRelayState(menu,true);
  drawTemperature(menu,true);
  drawCurrent(menu);

#ifndef _DISABLE_DRAW_SOFTWARE_VERSION
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

  int top = 20;
  int left = w - strW - 3;

  menu->print(str.c_str(),left,top);
#endif // !_DISABLE_DRAW_SOFTWARE_VERSION

  DBGLN(F("MainScreen::draw()"));
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen1::onButtonPressed(TFTMenu* menu, int pressedButton)
{
#ifndef _DISABLE_MAIN_SCREEN_BUTTONS
  // обработчик нажатия на кнопку. Номера кнопок начинаются с 0 и идут в том порядке, в котором мы их добавляли
	if (pressedButton == settingsButton)
	{
		menu->switchToScreen("Settings"); // переключаемся на экран настроек
	}
 else if(pressedButton == relAllButton)
 {
    Relay_LineALL.switchState();
 }
 else if(pressedButton == shunt1Button)
 {
    Relay_Shunt1.switchState();
 }
 else if(pressedButton == shunt2Button)
 {
    Relay_Shunt2.switchState();
 }
 else if(pressedButton == encoderButton)
 {
   startGeneratePulses();
 }
#endif // !_DISABLE_MAIN_SCREEN_BUTTONS
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
/*
Изменение в строках
25,26, 366, 433, 324, 326



*/
