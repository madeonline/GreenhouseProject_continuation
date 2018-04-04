//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "EthalonComparer.h"
#include "CONFIG.h"
#include "ConfigPin.h"
#include "Settings.h"
#include "FileUtils.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EthalonCompareResult EthalonComparer::Compare(InterruptTimeList& list, uint8_t channelNumber)
{
    if(list.size() < 2)
      return COMPARE_RESULT_NoSourcePulses; // нет исходных данных в списке


  // сравниваем кол-во импульсов на канал
  uint16_t channelPulses = Settings.getChannelPulses(channelNumber);
  uint16_t channelDelta = Settings.getChannelDelta(channelNumber);

  uint16_t minPulses = channelPulses - channelDelta;
  uint16_t maxPulses = channelPulses + channelDelta;

  if(!(list.size() >= minPulses && list.size() <= maxPulses))
    return COMPARE_RESULT_MismatchEthalon; // результат не соответствует эталону, поскольку кол-во импульсов расходится с настройками


  // смотрим позицию штанги
  RodPosition rodPos = ConfigPin::getRodPosition();
  if(rpBroken == rodPos)
    return COMPARE_RESULT_RodBroken;      // штанга поломана
      
  // загружаем файл эталона
  
   String fileName;
   fileName = ETHALONS_DIRECTORY;
   fileName += ETHALON_NAME_PREFIX;
  
   fileName += channelNumber;
  
  if(rodPos == rpUp) // штанга в верхней позиции, значит, она поднималась
    fileName += ETHALON_UP_POSTFIX;
  else
    fileName += ETHALON_DOWN_POSTFIX;
  
  fileName += ETHALON_FILE_EXT;
  
  if(!SD.exists(fileName.c_str()))
    return COMPARE_RESULT_NoEthalonFound; // не найдено эталона для канала
   
  InterruptTimeList ethalon;
  SdFile file;
  file.open(fileName.c_str(),FILE_READ);
  
  if(file.isOpen())
  {
    uint32_t curRec;
    while(1)
    {
      int readResult = file.read(&curRec,sizeof(curRec));
      if(readResult == -1 || size_t(readResult) < sizeof(curRec))
        break;
  
        ethalon.push_back(curRec);
    }
    file.close();
  }
  else
    return COMPARE_RESULT_NoEthalonFound; // не найдено эталона для канала
  
  // прочитали эталон, теперь можно сравнивать. Кол-во импульсов у нас находится в пределах настроенной дельты,
  // и нам надо сравнить импульсы, находящиеся в списке list, с импульсами, находящимися в списке ethalon.
  // ВОПРОС: как это грамотно сделать?


  return COMPARE_RESULT_MatchEthalon;    // результат соответствует эталону
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

