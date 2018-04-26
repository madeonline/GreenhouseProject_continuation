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
  //String line;
 // пишем положение штанги
  RodPosition rodPos = ConfigPin::getRodPosition(channelNumber);

  uint8_t workBuff[2] = {0};
  workBuff[0] = recordRodPosition;
  workBuff[1] = rodPos;
  
  Logger.write(workBuff,2);
  
  /*
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
  */
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeLogRecord(uint8_t channelNumber, InterruptTimeList& _list, EthalonCompareResult compareResult)
{
  if(_list.size() < 2) // ничего в списке прерываний нет
    return;

 // String line;
 uint8_t workBuff[5] = {0};

  workBuff[0] = recordInterruptRecordBegin;
  Logger.write(workBuff,1);
  
  //Logger.writeLine("[INTERRUPT_RECORD_BEGIN]");

  // пишем номер канала, для которого сработало прерывание
  workBuff[0] = recordChannelNumber;
  workBuff[1] = channelNumber;
  Logger.write(workBuff,2);

//  line = "[RECORD_CHANNEL]";
//  line += channelNumber;
//  Logger.writeLine(line);
  
  // пишем положение штанги №1
  writeRodPositionToLog(channelNumber);

  // пишем время движения штанги  
  uint32_t moveTime = _list[_list.size()-1] - _list[0];
  workBuff[0] = recordMoveTime;
  memcpy(&(workBuff[1]),&moveTime,4);
  Logger.write(workBuff,5);
  
  /*
  line = F("[LINE_MOVE_TIME]");
  line += moveTime;

  Logger.writeLine(line);
  */

  // пишем кол-во срабатываний канала
  uint32_t motoresource = Settings.getMotoresource(channelNumber);
  motoresource++;
  Settings.setMotoresource(channelNumber,motoresource);

/*
  line = "[MOTORESOURCE]";
  line += motoresource;

  Logger.writeLine(line);
*/
  workBuff[0] = recordMotoresource;
  memcpy(&(workBuff[1]),&motoresource,4);
  Logger.write(workBuff,5);  

  // пишем результат сравнения с эталоном для канала
  /*
  line = "[COMPARE_RESULT]";
  line += compareResult;
  
  Logger.writeLine(line);
  */
  workBuff[0] = recordCompareResult;
  workBuff[1] = compareResult;
  Logger.write(workBuff,2);

  // пишем список прерываний
  if(_list.size() > 1)
  {
    // есть список прерываний
   //Logger.write("[INTERRUPT_DATA]");
   //String dt;
   workBuff[0] = recordInterruptDataBegin;
   Logger.write(workBuff,1);

   Logger.write((uint8_t*) _list.pData(), _list.size()*sizeof(uint32_t));

   /*
   for(size_t i=0;i<_list.size();i++)
   {
      dt = _list[i];
      if(i < (_list.size()-1))
        dt += ",";

        Logger.write(dt);
   }
   */
   //Logger.writeLine("");
   workBuff[0] = recordInterruptDataEnd;
   Logger.write(workBuff,1);
  }

  // заканчиваем запись
  workBuff[0] = recordInterruptRecordEnd;
  Logger.write(workBuff,1);
  //Logger.writeLine("[INTERRUPT_RECORD_END]");
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeToLog(InterruptTimeList& lst1, InterruptTimeList& lst2, InterruptTimeList& lst3, EthalonCompareResult res1, EthalonCompareResult res2, EthalonCompareResult res3)
{

  uint8_t workBuff[10] = {0};

  //Logger.writeLine(F("[INTERRUPT_INFO_BEGIN]"));
  workBuff[0] = recordInterruptInfoBegin;
  Logger.write(workBuff,1);
  
  // пишем время срабатывания прерывания
  DS3231Time tm = RealtimeClock.getTime();
  /*
  String line;
  line = F("[INTERRUPT_TIME]");
  line += RealtimeClock.getDateStr(tm);
  line += ' ';
  line += RealtimeClock.getTimeStr(tm);

  Logger.writeLine(line);
  */

  workBuff[0] = recordInterruptTime;
  workBuff[1] = tm.dayOfMonth;
  workBuff[2] = tm.month;
  memcpy(&(workBuff[3]),&(tm.year),2);
  workBuff[5] = tm.hour;
  workBuff[6] = tm.minute;
  workBuff[7] = tm.second;
  
  Logger.write(workBuff,8);

  // пишем температуру системы
  DS3231Temperature temp = Settings.getTemperature();

  workBuff[0] = recordSystemTemperature;
  workBuff[1] = temp.Value;
  workBuff[2] = temp.Fract;
  
  Logger.write(workBuff,3);
  
/*
  line = "[TEMP]";
  line += temp.Value;
  line += '.';

  if(temp.Fract < 10)
    line += '0';

  line += temp.Fract;

  Logger.writeLine(line);
*/

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


    workBuff[0] = recordInterruptInfoEnd;
    Logger.write(workBuff,1);
//  Logger.writeLine(F("[INTERRUPT_INFO_END]"));
//  Logger.writeLine("");
  
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
