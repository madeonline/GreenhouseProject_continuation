#include "InterruptHandler.h"
#include "InterruptScreen.h"
#include "ConfigPin.h"
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
  interrupt1Raised = false;
  interrupt2Raised = false;
  interrupt3Raised = false;

  bPaused = false;
  handler = NULL;
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
  // обновляем статус прерываний. По условиям - если данные есть, и после последнего получения данных прошло N времени - нам надо показать экран
  // с графиком прерываний, сохранить график сработавшего прерывания, и очистить это дело.

  // поскольку мы перешли на интерфейсы, нас не волнует, какой экран и чего там запросил - мы просто передаём результаты обработчику, который
  // подписался на события вызовом нашего метода setHandler.

  if(interrupt1Raised)
  {
    interrupt1Raised = false;
    list1.push_back(micros());
  }

  if(interrupt2Raised)
  {
    interrupt2Raised = false;
    list2.push_back(micros());
  }

  if(interrupt3Raised)
  {
    interrupt3Raised = false;
    list3.push_back(micros());
  }


  uint8_t handleResult = handleList(0);
  handleResult += handleList(1);
  handleResult += handleList(2);

  if(handleResult > 0)
  {

    DBGLN(F("InterruptHandler: NOTIFY HANDLER WITH EVENT..."));
    
    // сообщаем обработчику результатов, что какие-то результаты есть
    if(handler)
      handler->OnHaveInterruptData();
  }

}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::setHandler(InterruptEventHandler* h)
{
  // устанавливаем обработчика результатов прерываний.
  // при этом очищаем локальные списки, т.к. будет установлен новый обработчик.
  pause();

  handler = h;
  list1.clear();
  list2.clear();
  list3.clear();

  resume();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::sendDataToHandler(const InterruptTimeList& list, uint8_t listNumber)
{
  DBG(F("InterruptHandler, list done="));
  DBGLN(listNumber);

  if(!handler)
    return;

  handler->OnInterruptRaised(list, listNumber);

}
//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t InterruptHandlerClass::handleList(uint8_t interruptNumber)
{

  InterruptTimeList* list = NULL;
  uint8_t listNum = 0;

  // ВОТ ЭТО ТУТ ЗАЧЕМ?
  //delayMicroseconds(50000);
  
  switch(interruptNumber)
  {
    case 0:
    {
      list = &list1;
      listNum = 0;
    }
    break;
    
    case 1:
    {
      list = &list2;
      listNum = 1;
    }
    break;
    case 2:
    {
      list = &list3;
      listNum = 2;
    }
    break;
  } // switch

  if(!list)
    return 0;


  noInterrupts();

  if(!list->size())
  {
    interrupts();
    return 0;
  }
  
  // в списке есть записи, проверяем - давно ли туда поступало последнее значение?
  uint32_t lastDataAt = (*list)[list->size()-1];
  interrupts();

  uint8_t result = 0;
  if(micros() - lastDataAt > INTERRUPT_MAX_IDLE_TIME)
  {

    // импульсы на входе закончились, надо список импульсов отправить в экран, а локальный - почистить.
    // для этого делаем копию импульсов
    noInterrupts();
    InterruptTimeList copyList = *list;
    list->clear();
    interrupts();

    result = 1;
    // здесь можем безопасно отправлять на экран
    sendDataToHandler(copyList, listNum);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::handleInterrupt(uint8_t interruptNumber)
{

  if(bPaused) // на паузе
    return;
  
  // запоминаем время, когда произошло прерывание, в нужный список
  switch(interruptNumber)
  {
    case 0:
    {
      interrupt1Raised = true;
    }
    break;
    
    case 1:
    {
      interrupt2Raised = true;
    }
    break;
    case 2:
    {
      interrupt3Raised = true;
    }
    break;
  } // switch
}
//--------------------------------------------------------------------------------------------------------------------------------------

