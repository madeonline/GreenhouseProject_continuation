#include "InterruptHandler.h"
#include "InterruptScreen.h"
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
  InterruptHandler.handleInterrupt(3);
}
//--------------------------------------------------------------------------------------------------------------------------------------
InterruptHandlerClass::InterruptHandlerClass()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::begin()
{
 // attachInterrupt(digitalPinToInterrupt(INTERRUPT1_PIN),Interrupt1Handler,RISING);
 //attachInterrupt(digitalPinToInterrupt(INTERRUPT2_PIN),Interrupt2Handler,RISING);
 attachInterrupt(digitalPinToInterrupt(INTERRUPT3_PIN),Interrupt3Handler,RISING);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::update()
{
  // обновляем статус прерываний. По условиям - если данные есть, и после последнего получения данных прошло N времени - нам надо показать экран
  // с графиком прерываний, сохранить график сработавшего прерывания, и очистить это дело.
  uint8_t handleResult = handleList(0);
  handleResult += handleList(1);
  handleResult += handleList(2);

  if(handleResult > 0)
  {
    DBGLN(F("WANT TO SEE INTERRUPT SCREEN!!!"));
    // тут переключаемся на экран с графиками
    if(ScreenInterrupt)
      ScreenInterrupt->showChart();
  }

}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::sendToInterruptScreen(const InterruptTimeList& list, uint8_t listNumber)
{
  DBG(F("Want to send interrupt data to screen, list number="));
  DBGLN(listNumber);

  if(ScreenInterrupt)
    ScreenInterrupt->setList(list,listNumber);

}
//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t InterruptHandlerClass::handleList(uint8_t interruptNumber)
{

  InterruptTimeList* list = NULL;
  uint8_t listNum = 0;
  
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
  unsigned long lastDataAt = (*list)[list->size()-1];
  interrupts();

  uint8_t result = 0;
  if(micros() - lastDataAt > INTERRUPT_MAX_IDLE_TIME)
  {
    // импульсы на входе закончились, надо список импульсов отправить в экран, а локальный - почистить.
    // для этого делаем копию импульсов
    noInterrupts();
    InterruptTimeList copyList = *list;
    list->empty();
    interrupts();

    result = 1;
    // здесь можем безопасно отправлять на экран
    sendToInterruptScreen(copyList, listNum);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void InterruptHandlerClass::handleInterrupt(uint8_t interruptNumber)
{
  // запоминаем время, когда произошло прерывание, в нужный список
  switch(interruptNumber)
  {
    case 0:
    {
      list1.push_back(micros());
    }
    break;
    
    case 1:
    {
      list2.push_back(micros());
    }
    break;
    case 2:
    {
      list3.push_back(micros());
    }
    break;
  } // switch
}
//--------------------------------------------------------------------------------------------------------------------------------------

