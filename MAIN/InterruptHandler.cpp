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
void Interrupt1Handler()
{
  InterruptHandler.handleInterrupt(0);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt2Handler()
{
  InterruptHandler.handleInterrupt(1);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt3Handler()
{
  InterruptHandler.handleInterrupt(2);
}
//--------------------------------------------------------------------------------------------------------------------------------------
InterruptHandlerClass::InterruptHandlerClass()
{
  subscriber = NULL;
  list1LastDataAt = 0;
  list2LastDataAt = 0;
  list3LastDataAt = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::begin()
{
  attachInterrupt(digitalPinToInterrupt(INTERRUPT1_PIN),Interrupt1Handler,RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT2_PIN),Interrupt2Handler,RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT3_PIN),Interrupt3Handler,RISING);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::normalizeList(InterruptTimeList& list)
{
  if(list.size() < 2)
    return;

  // нормализуем список относительно первого значения
  uint32_t first = list[0];
  list[0] = 0;

  for(size_t i=1;i<list.size();i++)
  {
    list[i] = (list[i] - first);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::writeToLog(const InterruptTimeList& lst1, const InterruptTimeList& lst2, const InterruptTimeList& lst3)
{

  // пишем время срабатывания прерывания
  DS3231Time tm = RealtimeClock.getTime();
  String line;
  line = F("[INTERRUPT]");
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


  // теперь смотрим, в каких списках есть данные, подсчитываем общее время движения планки, в микросекундах, и пишем в файл
  if(lst1.size() > 1)
  {
    line = "[LINE_MOVE_TIME_1]";
    uint32_t moveTime = lst1[lst1.size()-1] - lst1[0];
    line += moveTime;

    Logger.writeLine(line);

    // пишем кол-во срабатываний системы
    uint32_t motoresource = Settings.getMotoresource(0);
    motoresource++;
    Settings.setMotoresource(0,motoresource);
  
    line = "[MOTORESOURCE_1]";
    line += motoresource;
  
    Logger.writeLine(line);    
  } // if

  if(lst2.size() > 1)
  {
    line = "[LINE_MOVE_TIME_2]";
    uint32_t moveTime = lst2[lst2.size()-1] - lst2[0];
    line += moveTime;

    Logger.writeLine(line);

    // пишем кол-во срабатываний системы
    uint32_t motoresource = Settings.getMotoresource(1);
    motoresource++;
    Settings.setMotoresource(1,motoresource);
  
    line = "[MOTORESOURCE_2]";
    line += motoresource;
  
    Logger.writeLine(line);       
  } // if

  if(lst3.size() > 1)
  {
    line = "[LINE_MOVE_TIME_3]";
    uint32_t moveTime = lst3[lst3.size()-1] - lst3[0];
    line += moveTime;

    Logger.writeLine(line);

    // пишем кол-во срабатываний системы
    uint32_t motoresource = Settings.getMotoresource(2);
    motoresource++;
    Settings.setMotoresource(2,motoresource);
  
    line = "[MOTORESOURCE_3]";
    line += motoresource;
  
    Logger.writeLine(line);       
  } // if
  
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::update()
{
  
  uint32_t now = micros();

  // ситуация следующая - если у нас взведён хотя бы один флаг ожидания окончания списка - мы ждём, пока этот список не заполнится.
  // если у нас ни одного флага уже не взведено, списки заполнены по принципу давности прихода последнего значения,
  // и хотя бы в одном списке есть данные - мы отправляем все три списка обработчику, и чистим их локально.
  // по другому мы сделать не можем, т.к. работаем по прерываниям, и всегда может быть ситуация, когда заполняется какой-то список.
  // при этом нам надо быть уверенными в целостности данных, т.е. мы не можем отправлять обработчику данные, когда нам заблагорассудится,
  // поскольку он там с ними может что-то делать (например, мееедленно отрисовывать просчитанные ранее точки).
  // однако, по факту заполнения любого одного списка мы должны немедленно прореагировать на это дело, например, записать на SD, и 
  // после всего этого - почистить локальный список, чтобы он был готов к принятию новой порции данных.

  // резюмируем: вызывать OnHaveInterruptData у обработчика можно ТОЛЬКО тогда, когда все три списка не изменяются длительное время - это раз.
  // два: вызывать sendDataToHandler можно СРАЗУ по факту заполнения списка, здесь же - можно обрабатывать факт срабатывания.
  // три: по факту мы должны дождаться заполнения всех трёх списков прежде, чем вызывать OnHaveInterruptData.


  // если во всех трёх списках давно не было данных - считаем, что сбор данных закончен.
  if( (now - list1LastDataAt) > INTERRUPT_MAX_IDLE_TIME &&
      (now - list2LastDataAt) > INTERRUPT_MAX_IDLE_TIME &&
      (now - list3LastDataAt) > INTERRUPT_MAX_IDLE_TIME
  )
  {
    noInterrupts();

      // здесь обновляем время "сработки" списков, чтобы часто не дёргать эту проверку в случае, когда долго не срабатывают прерывания
      list1LastDataAt = now;
      list2LastDataAt = now;
      list3LastDataAt = now;
    
      InterruptTimeList copyList1 = list1;
      list1.clear();
  
      InterruptTimeList copyList2 = list2;
      list2.clear();
  
      InterruptTimeList copyList3 = list3;
      list3.clear();

      normalizeList(copyList1);
      normalizeList(copyList2);
      normalizeList(copyList3);

    interrupts();

    bool needToLog = false;

    // теперь смотрим - надо ли нам самим чего-то обрабатывать?
    if(copyList1.size() > 1)
    {
      DBGLN("INTERRUPT #1 HAS SERIES OF DATA!");

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();

      needToLog = true;
        
       //TODO: здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные !!!
    }
    
    if(copyList2.size() > 1)
    {
      DBGLN("INTERRUPT #2 HAS SERIES OF DATA!");

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();

      needToLog = true;
       
       //TODO: здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные !!!
    }
    
    if(copyList3.size() > 1)
    {
      DBGLN("INTERRUPT #3 HAS SERIES OF DATA!");

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();

      needToLog = true;
       
       //TODO: здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные !!!
    }

    if(needToLog)
    {
      // надо записать в лог дату срабатывания системы
      writeToLog(copyList1, copyList2, copyList3);
      
    } // needToLog
    

    // если в каком-то из списков есть данные - значит, одно из прерываний сработало,
    // в этом случае мы должны сообщить обработчику, что данные есть. При этом мы
    // не в ответе за то, что делает сейчас обработчик - пускай сам правильно разруливает ситуацию.

    bool wantToInformSubscriber = subscriber && ( (copyList1.size() > 1) || (copyList2.size() > 1) || (copyList3.size() > 1) );

    if(wantToInformSubscriber)
    {
      DBGLN(F("InterruptHandlerClass - wantToInformHandler"));
      
      DBGLN(F("InterruptHandlerClass - call OnInterruptRaised 0"));
      subscriber->OnInterruptRaised(copyList1, 0);

      DBGLN(F("InterruptHandlerClass - call OnInterruptRaised 1"));
      subscriber->OnInterruptRaised(copyList2, 1);
      
      DBGLN(F("InterruptHandlerClass - call OnInterruptRaised 2"));
      subscriber->OnInterruptRaised(copyList3, 2);

      DBGLN(F("InterruptHandlerClass - call OnHaveInterruptData"));
      
       // сообщаем обработчику, что данные в каком-то из списков есть
       subscriber->OnHaveInterruptData();
      
    }    
      
  }

}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::setSubscriber(InterruptEventSubscriber* h)
{  
  // устанавливаем подписчика результатов прерываний.
  subscriber = h;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::handleInterrupt(uint8_t interruptNumber)
{
  
  // запоминаем время, когда произошло прерывание, в нужный список
  uint32_t now = micros();
  
  switch(interruptNumber)
  {
    case 0:
    {
      list1.push_back(now);
      list1LastDataAt = now;
    }
    break;
    
    case 1:
    {
      list2.push_back(now);
      list2LastDataAt = now;
    }
    break;
    case 2:
    {
      list3.push_back(now);
      list3LastDataAt = now;
    }
    break;
  } // switch
}
//--------------------------------------------------------------------------------------------------------------------------------------

