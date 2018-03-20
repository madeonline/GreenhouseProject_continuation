#include "InterruptHandler.h"
#include "InterruptScreen.h"
#include "ConfigPin.h"
#include "InfoDiodes.h"
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
 // InterruptHandler.handleInterrupt(1);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void Interrupt3Handler()
{
 // InterruptHandler.handleInterrupt(2);
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

    interrupts();

    // теперь смотрим - надо ли нам самим чего-то обрабатывать?
    if(copyList1.size() > 1)
    {
      DBGLN("INTERRUPT #1 HAS SERIES OF DATA!");

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();
        
       //TODO: здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные !!!
    }
    
    if(copyList2.size() > 1)
    {
      DBGLN("INTERRUPT #2 HAS SERIES OF DATA!");

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();
       
       //TODO: здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные !!!
    }
    
    if(copyList3.size() > 1)
    {
      DBGLN("INTERRUPT #3 HAS SERIES OF DATA!");

      // зажигаем светодиод "ТЕСТ"
      InfoDiodes.test();
       
       //TODO: здесь мы можем обрабатывать список сами - в нём ЕСТЬ данные !!!
    }
    

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

