#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include "FileUtils.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  recordInterruptInfoBegin,   // начало записи по прерыванию в логе

    recordInterruptTime,        // время прерывания
    recordSystemTemperature,    // температура системы

    recordInterruptRecordBegin,// начало записи для канала
    
      recordChannelNumber,       // запись номера канала
      recordRodPosition,         // позиция штанги для канала
      recordMoveTime,            // время движения штанги для канала
      recordMotoresource,        // моторесурс канала
      recordEthalonNumber,       // номер эталона, с которым сравнивали
      recordCompareResult,       // результат сравнения с эталоном

      recordInterruptDataBegin,  // начало данных прерывания
      recordInterruptDataEnd,    // конец данных прерывания

      recordEthalonDataFollow,  // следом идут данные эталона, с которым сравнивали

    recordInterruptRecordEnd, // конец записи для канада
  
  recordInterruptInfoEnd,    // конец записи по прерыванию

  recordRelayTriggeredTime, // время срабатывания защиты
    
} LogRecordTypes;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class LoggerClass
{
  public:

    LoggerClass();

    //void writeLine(const String& line);
    //void write(const String& data);

    void write(uint8_t* data,size_t dataLength);

  private:

    SdFile workFile;
    bool openWorkFile();
    void closeWorkFile();

};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern LoggerClass Logger;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
