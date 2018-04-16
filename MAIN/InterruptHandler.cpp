#include "InterruptHandler.h"
#include "InterruptScreen.h"
#include "ConfigPin.h"
#include "InfoDiodes.h"
#include "DS3231.h"
#include "FileUtils.h"
#include "Logger.h"
#include "Settings.h"
//--------------------------------------------------------------------------------------------------------------------------------------
InterruptHandlerClass InterruptHandler;
//--------------------------------------------------------------------------------------------------------------------------------------
// списки времён срабатываний прерываний на наших портах
InterruptTimeList list1;
InterruptTimeList list2;
InterruptTimeList list3;
//--------------------------------------------------------------------------------------------------------------------------------------
volatile bool onTimer = false;
volatile uint32_t lastInterruptTime = 0;
//--------------------------------------------------------------------------------------------------------------------------------------

/*
volatile uint32_t list1LastDataAt = 0;
volatile uint32_t list2LastDataAt = 0;
volatile uint32_t list3LastDataAt = 0;
*/
//--------------------------------------------------------------------------------------------------------------------------------------
InterruptEventSubscriber* subscriber = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------
void setInterruptFlag()
{
  onTimer = true;
  lastInterruptTime = micros();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt1Handler()
{
    uint32_t now = micros();
    list1.push_back(now);
    setInterruptFlag();
    //list1LastDataAt = now;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt2Handler()
{
    uint32_t now = micros();
    list2.push_back(now);
    setInterruptFlag();
    //list2LastDataAt = now;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt3Handler()
{
    uint32_t now = micros();
    list3.push_back(now);
    setInterruptFlag();
   // list3LastDataAt = now;
}
//--------------------------------------------------------------------------------------------------------------------------------------
InterruptHandlerClass::InterruptHandlerClass()
{
  subscriber = NULL;
  /*
  list1LastDataAt = 0;
  list2LastDataAt = 0;
  list3LastDataAt = 0;
  */
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::begin()
{
  // резервируем память
  list1.reserve(INTERRUPT_RESERVE_RECORDS);
  list2.reserve(INTERRUPT_RESERVE_RECORDS);
  list3.reserve(INTERRUPT_RESERVE_RECORDS);

  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT1_PIN),Interrupt1Handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT2_PIN),Interrupt2Handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT3_PIN),Interrupt3Handler, CHANGE);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::normalizeList(InterruptTimeList& list)
{
  size_t sz = list.size();
  
  if(sz < 2)
    return;

  // нормализуем список относительно первого значения
  uint32_t first = list[0];
  list[0] = 0;

  for(size_t i=1;i<sz;i++)
  {
    list[i] = (list[i] - first);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeRodPositionToLog(uint8_t channelNumber)
{
  String line;
 // пишем положение штанги
  RodPosition rodPos = ConfigPin::getRodPosition(channelNumber);
  line = "[ROD_";
  line += channelNumber;
  line += "]";

  switch(rodPos)
  {
    case rpBroken:
      line += "BROKEN";
    break;
    
    case rpUp:
      line += "UP";
    break;

    case rpDown:
      line += "DOWN";
    break;

  }

  Logger.writeLine(line);  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeLogRecord(uint8_t channelNumber, const InterruptTimeList& _list, EthalonCompareResult compareResult)
{
  if(_list.size() < 2) // ничего в списке прерываний нет
    return;

  String line;
  
  // пишем положение штанги №1
  writeRodPositionToLog(channelNumber);

  // пишем время движения штанги
  line = "[LINE_MOVE_TIME_";
  line += channelNumber;
  line += "]";
  
  uint32_t moveTime = _list[_list.size()-1] - _list[0];
  line += moveTime;

  Logger.writeLine(line);

  // пишем кол-во срабатываний канала
  uint32_t motoresource = Settings.getMotoresource(channelNumber);
  motoresource++;
  Settings.setMotoresource(channelNumber,motoresource);

  line = "[MOTORESOURCE_";
  line += channelNumber;
  line += "]";  
  line += motoresource;

  Logger.writeLine(line);

  // пишем результат сравнения с эталоном для канала
  line = "[COMPARE_RESULT_";
  line += channelNumber;
  line += "]";
  line += compareResult;
  
  Logger.writeLine(line);    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeToLog(const InterruptTimeList& lst1, const InterruptTimeList& lst2, const InterruptTimeList& lst3, EthalonCompareResult res1, EthalonCompareResult res2, EthalonCompareResult res3)
{

  Logger.writeLine(F("[INTERRUPT_INFO_BEGIN]"));
  // пишем время срабатывания прерывания
  DS3231Time tm = RealtimeClock.getTime();
  String line;
  line = F("[INTERRUPT_TIME]");
  line += RealtimeClock.getDateStr(tm);
  line += ' ';
  line += RealtimeClock.getTimeStr(tm);

  Logger.writeLine(line);

  // пишем температуру системы
  DS3231Temperature temp = Settings.getTemperature();

  line = "[TEMP]";
  line += temp.Value;
  line += '.';

  if(temp.Fract < 10)
    line += '0';

  line += temp.Fract;

  Logger.writeLine(line);


  // теперь смотрим, в каких списках есть данные, и пишем записи в лог
  if(lst1.size() > 1)
  {
    writeLogRecord(0,lst1,res1); 
  } // if

  if(lst2.size() > 1)
  {
    writeLogRecord(1,lst2,res2); 
  } // if

  if(lst3.size() > 1)
  {
    writeLogRecord(2,lst3,res3);
  } // if


  Logger.writeLine(F("[INTERRUPT_INFO_END]"));
  Logger.writeLine("");
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::update()
{

  static bool inProcess = false;

  noInterrupts();
    bool thisOnTimer = onTimer;
    uint32_t lastTime = lastInterruptTime;
  interrupts();


  if(!thisOnTimer || inProcess)
    return;

    if(!(micros() - lastTime > INTERRUPT_MAX_IDLE_TIME))
    {
      return;
    }

    noInterrupts();

      inProcess = true;
      onTimer = false;
      
      InterruptTimeList copyList1 = list1;
      // вызываем не clear, а empty, чтобы исключить лишние переаллокации памяти
      list1.empty();
  
      InterruptTimeList copyList2 = list2;      
      // вызываем не clear, а empty, чтобы исключить лишние переаллокации памяти
      list2.empty();
      
      InterruptTimeList copyList3 = list3;      
      // вызываем не clear, а empty, чтобы исключить лишние переаллокации памяти
      list3.empty();
          
    interrupts();

     InterruptHandlerClass::normalizeList(copyList1);
     InterruptHandlerClass::normalizeList(copyList2);
     InterruptHandlerClass::normalizeList(copyList3);

     EthalonCompareResult compareRes1 = COMPARE_RESULT_NoSourcePulses;
     EthalonCompareResult compareRes2 = COMPARE_RESULT_NoSourcePulses;
     EthalonCompareResult compareRes3 = COMPARE_RESULT_NoSourcePulses;
     
    bool needToLog = false;

    // теперь смотрим - надо ли нам самим чего-то обрабатывать?
    if(copyList1.size() > 1)
    {
      DBG("INTERRUPT #1 HAS SERIES OF DATA: ");
      DBGLN(copyList1.size());

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();

      needToLog = true;
        
       // здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные
       compareRes1 = EthalonComparer::Compare(copyList1, 0);

       if(compareRes1 == COMPARE_RESULT_MatchEthalon)
        {}
       else if(compareRes1 == COMPARE_RESULT_MismatchEthalon || compareRes1 == COMPARE_RESULT_RodBroken)
        InfoDiodes.failure();
    }
    
    if(copyList2.size() > 1)
    {
      DBG("INTERRUPT #2 HAS SERIES OF DATA: ");
      DBGLN(copyList2.size());

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();

      needToLog = true;
       
       // здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные
       compareRes2 = EthalonComparer::Compare(copyList2, 1);
       
       if(compareRes2 == COMPARE_RESULT_MatchEthalon)
        {}
       else if(compareRes2 == COMPARE_RESULT_MismatchEthalon || compareRes2 == COMPARE_RESULT_RodBroken)
        InfoDiodes.failure();
    }
    
    if(copyList3.size() > 1)
    {
      DBG("INTERRUPT #3 HAS SERIES OF DATA: ");
      DBGLN(copyList3.size());

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();

      needToLog = true;
       
       // здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные
       compareRes3 = EthalonComparer::Compare(copyList3, 2);

       if(compareRes3 == COMPARE_RESULT_MatchEthalon)
        {}
       else if(compareRes3 == COMPARE_RESULT_MismatchEthalon || compareRes3 == COMPARE_RESULT_RodBroken)
        InfoDiodes.failure();
       
    }

    if(needToLog)
    {
      // надо записать в лог дату срабатывания системы
      InterruptHandlerClass::writeToLog(copyList1, copyList2, copyList3, compareRes1, compareRes2, compareRes3);     
    } // needToLog
    

    // если в каком-то из списков есть данные - значит, одно из прерываний сработало,
    // в этом случае мы должны сообщить обработчику, что данные есть. При этом мы
    // не в ответе за то, что делает сейчас обработчик - пускай сам разруливает ситуацию
    // так, как нужно ему.

    bool wantToInformSubscriber = subscriber && ( (copyList1.size() > 1) || (copyList2.size() > 1) || (copyList3.size() > 1) );


    if(wantToInformSubscriber)
    {
      
      subscriber->OnInterruptRaised(copyList1, 0, compareRes1);
      subscriber->OnInterruptRaised(copyList2, 1, compareRes2);      
      subscriber->OnInterruptRaised(copyList3, 2, compareRes3);
      
       // сообщаем обработчику, что данные в каком-то из списков есть
       subscriber->OnHaveInterruptData();
      
    }    

    inProcess = false;

}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::setSubscriber(InterruptEventSubscriber* h)
{  
  // устанавливаем подписчика результатов прерываний.
  subscriber = h;
}
//--------------------------------------------------------------------------------------------------------------------------------------
