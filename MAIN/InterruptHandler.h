#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include "CONFIG.h"
#include "TinyVector.h"
//--------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<uint32_t> InterruptTimeList;
//--------------------------------------------------------------------------------------------------------------------------------------
// структура-подписчик обработчика прерываний. Имея такой интерфейс - всегда можно переназначить вывод результатов серий измерений
// от одного обработчика прерываний в разных подписчиков, например: в обычном режиме показывается график при срабатывании прерываний,
// в режиме записи эталона - показывается другой экран, который регистрирует себя в качестве временного обработчика результатов
// серий прерываний, и что-то там с ними делает; по выходу с экрана обработчиком результатов прерываний опять назначается экран с 
// графиками. Т.е. имеем гибкий инструмент, кмк.
//--------------------------------------------------------------------------------------------------------------------------------------
// результат сравнения списка прерываний с эталоном
//--------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  COMPARE_RESULT_NoSourcePulses, // нет исходных данных в списке
  COMPARE_RESULT_NoEthalonFound, // не найдено эталона для канала
  COMPARE_RESULT_RodBroken,      // штанга поломана
  COMPARE_RESULT_MatchEthalon,    // результат соответствует эталону
  COMPARE_RESULT_MismatchEthalon, // результат не соответствует эталону
  
} EthalonCompareResult;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct InterruptEventSubscriber
{
  // вызывается, когда прерывания на нужном номере завершены, и накоплена статистика
  virtual void OnInterruptRaised(const InterruptTimeList& list, uint8_t interruptNumber, EthalonCompareResult result) = 0;

  // вызывается, когда есть хотя бы один список с прерываниями - закончен
  virtual void OnHaveInterruptData() = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------
class InterruptHandlerClass
{
 public:
  InterruptHandlerClass();

   void begin();
   void update();

   void setSubscriber(InterruptEventSubscriber* h);

private:

  static void normalizeList(InterruptTimeList& list);

  static void writeLogRecord(uint8_t channelNumber, const InterruptTimeList& _list, EthalonCompareResult compareResult);
  static void writeToLog(const InterruptTimeList& lst1, const InterruptTimeList& lst2, const InterruptTimeList& lst3, EthalonCompareResult res1, EthalonCompareResult res2, EthalonCompareResult res3);
  static void writeRodPositionToLog(uint8_t channelNumber);
};
//--------------------------------------------------------------------------------------------------------------------------------------
extern InterruptHandlerClass InterruptHandler;
//--------------------------------------------------------------------------------------------------------------------------------------

