//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "EthalonComparer.h"
#include "CONFIG.h"
#include "ConfigPin.h"
#include "Settings.h"
#include "FileUtils.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EthalonCompareResult EthalonComparer::Compare(InterruptTimeList& list, uint8_t channelNumber, EthalonCompareNumber& compareNumber, InterruptTimeList& ethalonData)
{
    DBGLN("");
    DBG(F("Сравниваем список импульсов #"));
    DBG(channelNumber);
    DBGLN(F(" с эталоном..."));

    compareNumber = ecnNoEthalon;
    ethalonData.clear();
    
    if(list.size() < 2)
    {
      DBGLN(F("пустой исходный список, нет ничего к сравлению"));
      return COMPARE_RESULT_NoSourcePulses; // нет исходных данных в списке
    }


  // сравниваем кол-во импульсов на канал
  uint16_t channelPulses = Settings.getChannelPulses(channelNumber);
  uint16_t channelDelta = Settings.getChannelDelta(channelNumber);

  uint16_t minPulses = channelPulses - channelDelta;
  uint16_t maxPulses = channelPulses + channelDelta;

  if(!(list.size() >= minPulses && list.size() <= maxPulses))
  {
    DBGLN(F("кол-во импульсов в списке выходит за границы допустимых - несовпадание с эталоном!"));
    return COMPARE_RESULT_MismatchEthalon; // результат не соответствует эталону, поскольку кол-во импульсов расходится с настройками
  }

  // смотрим позицию штанги
  RodDirection rodPos = Settings.getRodDirection();// (channelNumber);
  
 #ifndef IGNORE_ROD_POSITION
    if(rpBroken == rodPos)
    {
      DBGLN(F("детектирована поломка штанги!"));
      return COMPARE_RESULT_RodBroken;      // штанга поломана
    }
 #endif
      
  // загружаем файл эталона
  
   String fileName;
   fileName = ETHALONS_DIRECTORY;
   fileName += ETHALON_NAME_PREFIX;
  
   fileName += channelNumber;

  #ifndef IGNORE_ROD_POSITION  
    if(rodPos == rpUp) // штанга в верхней позиции, значит, она поднималась
    {
      fileName += ETHALON_UP_POSTFIX;
      switch(channelNumber)
      {
        case 0:
          compareNumber = ecnE1up;
        break;
        case 1:
          compareNumber = ecnE2up;
        break;
        case 2:
          compareNumber = ecnE3up;
        break;
      }
    }
    else
    {
      fileName += ETHALON_DOWN_POSTFIX;
      switch(channelNumber)
      {
        case 0:
          compareNumber = ecnE1down;
        break;
        case 1:
          compareNumber = ecnE2down;
        break;
        case 2:
          compareNumber = ecnE3down;
        break;
      }
    }
  #else
  {
    fileName += ETHALON_UP_POSTFIX;
      switch(channelNumber)
      {
        case 0:
          compareNumber = ecnE1up;
        break;
        case 1:
          compareNumber = ecnE2up;
        break;
        case 2:
          compareNumber = ecnE3up;
        break;
      }    
  }
  #endif
  
  fileName += ETHALON_FILE_EXT;
  
  if(!SD.exists(fileName.c_str()))
  {
    DBGLN(F("не найден файл эталона!"));

    compareNumber = ecnNoEthalon;
    return COMPARE_RESULT_NoEthalonFound; // не найдено эталона для канала
  }
   
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
  
        ethalonData.push_back(curRec);
    }
    file.close();
  }
  else
  {
    DBGLN(F("не удалось открыть файл эталона!"));

    compareNumber = ecnNoEthalon;
    return COMPARE_RESULT_NoEthalonFound; // не найдено эталона для канала
  }
  
  // прочитали эталон, теперь можно сравнивать. Кол-во импульсов у нас находится в пределах настроенной дельты,
  // и нам надо сравнить импульсы, находящиеся в списке list, с импульсами, находящимися в списке ethalon.


  DBG(F("Кол-во импульсов эталона: "));
  DBGLN(ethalonData.size());

  DBG(F("Кол-во собранных импульсов: "));
  DBGLN(list.size());

  // для начала вычисляем, сколько импульсов сравнивать
  size_t toCompare = min(ethalonData.size(),list.size());

  DBG(F("Импульсов к сравнению: "));
  DBGLN(toCompare);
  
  // потом проходим по каждому импульсу
  for(size_t i=1;i<toCompare;i++)
  {
    uint32_t ethalonPulseDuration = ethalonData[i] - ethalonData[i-1];
    uint32_t passedPulseDuration = list[i] - list[i-1];

    DBG("#");
    DBG(i-1);
    DBG(", ");
    DBG(ethalonPulseDuration);
    DBG("=");
    DBGLN(passedPulseDuration);
    
    uint32_t lowVal;
    uint32_t highVal = ethalonPulseDuration + ETHALON_COMPARE_DELTA;

    if(ethalonPulseDuration >= ETHALON_COMPARE_DELTA)
      lowVal = ethalonPulseDuration - ETHALON_COMPARE_DELTA;
    else
      lowVal = 0;

    // и если длительность между импульсами различается от эталонной больше, чем на нужную дельту - эталон не совпадает
    if(!(passedPulseDuration >= lowVal && passedPulseDuration <= highVal))
    {
      DBGLN(F("найдено несовпадение с эталоном!"));
      return COMPARE_RESULT_MismatchEthalon;
    }
  }

  DBGLN(F("полное сомпадение с эталоном!"));
  DBGLN("");
  return COMPARE_RESULT_MatchEthalon;    // результат соответствует эталону
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

