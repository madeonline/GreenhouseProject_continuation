#include "InterruptHandler.h"
#include "InterruptScreen.h"
#include "ConfigPin.h"
#include "Feedback.h"
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
volatile bool onInterruptSeriesTimer = false;
volatile uint32_t lastInterruptTime = 0;

volatile uint32_t relayTriggeredTime = 0;
volatile bool onRelayTriggeredTimer = false;

volatile uint32_t timeBeforeInterruptsBegin = 0; // время от срабатывания реле защиты до первого прерывания
volatile bool hasRelayTriggeredTime = false; // флаг, что было срабатывание реле защиты перед пачкой прерываний
//--------------------------------------------------------------------------------------------------------------------------------------
InterruptEventSubscriber* subscriber = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------
void setInterruptFlag()
{
  onInterruptSeriesTimer = true;
  lastInterruptTime = micros();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt1Handler()
{
    uint32_t now = micros();
    list1.push_back(now);    
    setInterruptFlag();

  if(list1.size() < 2)
  {
    timeBeforeInterruptsBegin = (micros() - relayTriggeredTime);
  }  
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt2Handler()
{
    uint32_t now = micros();
    list2.push_back(now);
    setInterruptFlag();

  if(list2.size() < 2)
  {
    timeBeforeInterruptsBegin = (micros() - relayTriggeredTime);
  }  
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt3Handler()
{
    uint32_t now = micros();
    list3.push_back(now);
    setInterruptFlag();

  if(list3.size() < 2)
  {
    timeBeforeInterruptsBegin = (micros() - relayTriggeredTime);
  }  
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void RelayTriggered()
{
  // запоминаем время срабатывания защиты
  relayTriggeredTime = micros();
  onRelayTriggeredTimer = true;
  hasRelayTriggeredTime = true;
  timeBeforeInterruptsBegin = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
InterruptHandlerClass::InterruptHandlerClass()
{
  subscriber = NULL;
  hasAlarm = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::begin()
{
  // резервируем память
  list1.reserve(INTERRUPT_RESERVE_RECORDS);
  list2.reserve(INTERRUPT_RESERVE_RECORDS);
  list3.reserve(INTERRUPT_RESERVE_RECORDS);

  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);

  attachInterrupt(digitalPinToInterrupt(RELAY_PIN),RelayTriggered, RISING);
  
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
 // пишем положение штанги
  RodPosition rodPos = ConfigPin::getRodPosition(channelNumber);

  uint8_t workBuff[2] = {0};
  workBuff[0] = recordRodPosition;
  workBuff[1] = rodPos;
  
  Logger.write(workBuff,2);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeLogRecord(uint8_t channelNumber, InterruptTimeList& _list, EthalonCompareResult compareResult, EthalonCompareNumber num, InterruptTimeList& ethalonData)
{
  if(_list.size() < 2) // ничего в списке прерываний нет
    return;

 uint8_t workBuff[5] = {0};

  workBuff[0] = recordInterruptRecordBegin;
  Logger.write(workBuff,1);
  
  // пишем номер канала, для которого сработало прерывание
  workBuff[0] = recordChannelNumber;
  workBuff[1] = channelNumber;
  Logger.write(workBuff,2);
  
  // пишем положение штанги №1
  writeRodPositionToLog(channelNumber);

  // пишем время движения штанги  
  uint32_t moveTime = _list[_list.size()-1] - _list[0];
  workBuff[0] = recordMoveTime;
  memcpy(&(workBuff[1]),&moveTime,4);
  Logger.write(workBuff,5);
  

  // пишем кол-во срабатываний канала
  uint32_t motoresource = Settings.getMotoresource(channelNumber);
  motoresource++;
  Settings.setMotoresource(channelNumber,motoresource);

  workBuff[0] = recordMotoresource;
  memcpy(&(workBuff[1]),&motoresource,4);
  Logger.write(workBuff,5);  

  // пишем номер эталона, с которым сравнивали
  workBuff[0] = recordEthalonNumber;
  workBuff[1] = num;
  Logger.write(workBuff,2);
  
  // пишем результат сравнения с эталоном для канала
  workBuff[0] = recordCompareResult;
  workBuff[1] = compareResult;
  Logger.write(workBuff,2);

  // пишем список прерываний
  if(_list.size() > 1)
  {
    // есть список прерываний
   workBuff[0] = recordInterruptDataBegin;
   uint16_t dataLen = _list.size();
   memcpy(&(workBuff[1]),&dataLen,2);
   Logger.write(workBuff,3);

   Logger.write((uint8_t*) _list.pData(), _list.size()*sizeof(uint32_t));
   workBuff[0] = recordInterruptDataEnd;
   Logger.write(workBuff,1);
  }

  if(ethalonData.size() > 1)
  {
   // пишем данные эталона, с которым сравнивали
   workBuff[0] = recordEthalonDataFollow;
   uint16_t dataLen = ethalonData.size();
   memcpy(&(workBuff[1]),&dataLen,2);
   Logger.write(workBuff,3);

   Logger.write((uint8_t*) ethalonData.pData(), ethalonData.size()*sizeof(uint32_t));    
  }

  // заканчиваем запись
  workBuff[0] = recordInterruptRecordEnd;
  Logger.write(workBuff,1);
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeToLog(InterruptTimeList& lst1, InterruptTimeList& lst2, InterruptTimeList& lst3, EthalonCompareResult res1, EthalonCompareResult res2, EthalonCompareResult res3
,EthalonCompareNumber num1,EthalonCompareNumber num2, EthalonCompareNumber num3, InterruptTimeList& ethalonData1, InterruptTimeList& ethalonData2, InterruptTimeList& ethalonData3)
{

  uint8_t workBuff[10] = {0};

  //Logger.writeLine(F("[INTERRUPT_INFO_BEGIN]"));
  workBuff[0] = recordInterruptInfoBegin;
  Logger.write(workBuff,1);
  
  // пишем время срабатывания прерывания
  DS3231Time tm = RealtimeClock.getTime();

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
  
  // теперь смотрим, в каких списках есть данные, и пишем записи в лог
  if(lst1.size() > 1)
  {
    writeLogRecord(0,lst1,res1,num1, ethalonData1); 
  } // if

  if(lst2.size() > 1)
  {
    writeLogRecord(1,lst2,res2,num2, ethalonData2); 
  } // if

  if(lst3.size() > 1)
  {
    writeLogRecord(2,lst3,res3,num3, ethalonData3);
  } // if


    workBuff[0] = recordInterruptInfoEnd;
    Logger.write(workBuff,1);
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::update()
{

  static bool inProcess = false;

  noInterrupts();
    bool thisOnInterruptSeriesTimer = onInterruptSeriesTimer;
    uint32_t thisLastInterruptTime = lastInterruptTime;
    
    bool thisOnRelayTriggeredTimer  = onRelayTriggeredTimer;
    uint32_t thisRelayTriggeredTime = relayTriggeredTime;
  interrupts();

  // проверяем факт срабатывания защиты
  if(thisOnRelayTriggeredTimer)
  {
    // было прерывание срабатывания защиты - проверяем время
    if(micros() - thisRelayTriggeredTime >= RELAY_WANT_DATA_AFTER)
    {      
      // время ожидания прошло
      // проверяем - если данные в одном из списков есть - ничего не делаем.
      // если ни в одном из списков нет данных - значит, это авария.
      // в любом другом случае флаг аварии выставится после того, как будет принято решение
      // о том, что пачки импульсов закончились.
      
      noInterrupts();
       onRelayTriggeredTimer = false;
       relayTriggeredTime = micros();
       hasAlarm = !(list1.size() || list2.size() || list3.size());
       
       if(hasAlarm)
       {
        // есть тревога, надо подождать окончания прерываний
        thisOnInterruptSeriesTimer = true;
        thisLastInterruptTime = micros();

        onInterruptSeriesTimer = true;
        lastInterruptTime = micros();

        timeBeforeInterruptsBegin = micros() - thisRelayTriggeredTime;
       }
      interrupts();
      

      // выставляем флаг аварии, в зависимости от наличия данных в списках
      if(hasAlarm)
      {
        // сделал именно так, поскольку флаг аварии сбрасывать нельзя, плюс могут понадобиться дополнительные действия
        Feedback.alarm(true);
      }
    } // if
    
  } // if


    if(!thisOnInterruptSeriesTimer || inProcess)
      return;
  
      if(!(micros() - thisLastInterruptTime > INTERRUPT_MAX_IDLE_TIME))
      {
        return;
      }
    noInterrupts();

      inProcess = true;
      onInterruptSeriesTimer = false;
      
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

     EthalonCompareNumber compareNumber1,compareNumber2,compareNumber3;
     InterruptTimeList ethalonData1, ethalonData2, ethalonData3;
     
    bool needToLog = false;

    // теперь смотрим - надо ли нам самим чего-то обрабатывать?
    if(copyList1.size() > 1)
    {
      DBG("INTERRUPT #1 HAS SERIES OF DATA: ");
      DBGLN(copyList1.size());

      // зажигаем светодиод "ТЕСТ"
      Feedback.testDiode();

      needToLog = true;
        
       // здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные
       compareRes1 = EthalonComparer::Compare(copyList1, 0,compareNumber1, ethalonData1);

       if(compareRes1 == COMPARE_RESULT_MatchEthalon)
        {}
       else if(compareRes1 == COMPARE_RESULT_MismatchEthalon || compareRes1 == COMPARE_RESULT_RodBroken)
       {
          Feedback.failureDiode();
          Feedback.alarm();
       }
    }
    
    if(copyList2.size() > 1)
    {
      DBG("INTERRUPT #2 HAS SERIES OF DATA: ");
      DBGLN(copyList2.size());

      // зажигаем светодиод "ТЕСТ"
      Feedback.testDiode();

      needToLog = true;
       
       // здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные
       compareRes2 = EthalonComparer::Compare(copyList2, 1,compareNumber2, ethalonData2);
       
       if(compareRes2 == COMPARE_RESULT_MatchEthalon)
        {}
       else if(compareRes2 == COMPARE_RESULT_MismatchEthalon || compareRes2 == COMPARE_RESULT_RodBroken)
       {
        Feedback.failureDiode();
        Feedback.alarm();
       }
    }
    
    if(copyList3.size() > 1)
    {
      DBG("INTERRUPT #3 HAS SERIES OF DATA: ");
      DBGLN(copyList3.size());

      // зажигаем светодиод "ТЕСТ"
      Feedback.testDiode();

      needToLog = true;
       
       // здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные
       compareRes3 = EthalonComparer::Compare(copyList3, 2,compareNumber3, ethalonData3);

       if(compareRes3 == COMPARE_RESULT_MatchEthalon)
        {}
       else if(compareRes3 == COMPARE_RESULT_MismatchEthalon || compareRes3 == COMPARE_RESULT_RodBroken)
       {
        Feedback.failureDiode();
        Feedback.alarm();
       }
       
    }

    if(needToLog)
    {
      // надо записать в лог дату срабатывания системы
      InterruptHandlerClass::writeToLog(copyList1, copyList2, copyList3, compareRes1, compareRes2, compareRes3,compareNumber1,compareNumber2,compareNumber3, ethalonData1, ethalonData2, ethalonData3);     
    } // needToLog
    

    // если в каком-то из списков есть данные - значит, одно из прерываний сработало,
    // в этом случае мы должны сообщить обработчику, что данные есть. При этом мы
    // не в ответе за то, что делает сейчас обработчик - пускай сам разруливает ситуацию
    // так, как нужно ему.

    bool wantToInformSubscriber = ( hasAlarm || (copyList1.size() > 1) || (copyList2.size() > 1) || (copyList3.size() > 1) );

    if(wantToInformSubscriber)
    {       
      if(subscriber)
      {
        noInterrupts();
        uint32_t thisTm = timeBeforeInterruptsBegin;
        bool thisHasRelayTriggeredTime = hasRelayTriggeredTime;
        
        timeBeforeInterruptsBegin = 0;
        hasRelayTriggeredTime = false;
        relayTriggeredTime = micros();
        interrupts();

        subscriber->OnTimeBeforeInterruptsBegin(thisTm, thisHasRelayTriggeredTime);
                
        subscriber->OnInterruptRaised(copyList1, 0, compareRes1);
        subscriber->OnInterruptRaised(copyList2, 1, compareRes2);      
        subscriber->OnInterruptRaised(copyList3, 2, compareRes3);
        
         // сообщаем обработчику, что данные в каком-то из списков есть
         subscriber->OnHaveInterruptData();
      }
      else
      {
        noInterrupts();
        timeBeforeInterruptsBegin = 0;
        relayTriggeredTime = micros();
        interrupts();
      }
      
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
