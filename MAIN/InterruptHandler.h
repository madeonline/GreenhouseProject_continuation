#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include "CONFIG.h"
#include "TinyVector.h"
//--------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<uint32_t> InterruptTimeList;
//--------------------------------------------------------------------------------------------------------------------------------------
class InterruptHandlerClass
{
 public:
  InterruptHandlerClass();

   void begin();
   void update();

   void handleInterrupt(uint8_t interruptNumber);

private:

  // списки времён срабатываний прерываний на наших портах
  InterruptTimeList list1;
  InterruptTimeList list2;
  InterruptTimeList list3;

  uint8_t handleList(uint8_t interruptNumber);
  void sendToInterruptScreen(const InterruptTimeList& list, uint8_t listNumber);

};
//--------------------------------------------------------------------------------------------------------------------------------------
extern InterruptHandlerClass InterruptHandler;
//--------------------------------------------------------------------------------------------------------------------------------------

