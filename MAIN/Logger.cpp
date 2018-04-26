//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Logger.h"
#include "DS3231.h"
#include "CONFIG.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LoggerClass Logger;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LoggerClass::LoggerClass()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool LoggerClass::openWorkFile()
{
  closeWorkFile();


 // пишем в лог-файл дату/время срабатывания системы
  SD.mkdir(LOGS_DIRECTORY);

  DS3231Time tm = RealtimeClock.getTime();

  // формируем имя файла ггггммдд.log. (год,месяц,день)
  String logFileName;
  
  logFileName = LOGS_DIRECTORY;
  if(!logFileName.endsWith("/"))
    logFileName += "/";
  
  logFileName += tm.year;
  if(tm.month < 10)
    logFileName += '0';
  logFileName += tm.month;

 if(tm.dayOfMonth < 10)
  logFileName += '0';
 logFileName += tm.dayOfMonth;

  logFileName += F(".LOG");

  //DBG(F("WRITE INFO TO: "));
  //DBGLN(logFileName);

  workFile.open(logFileName.c_str(),FILE_WRITE);  
  return workFile.isOpen();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void LoggerClass::closeWorkFile()
{
  if(workFile.isOpen())
    workFile.close();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void LoggerClass::write(uint8_t* data,size_t dataLength)
{
  if(!openWorkFile())
    return;

  workFile.write(data,dataLength);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void LoggerClass::write(const String& line)
{
 // пишем в лог-файл дату/время срабатывания системы
  SD.mkdir(LOGS_DIRECTORY);

  DS3231Time tm = RealtimeClock.getTime();

  // формируем имя файла ггггммдд.log. (год,месяц,день)
  String logFileName;
  
  logFileName = LOGS_DIRECTORY;
  if(!logFileName.endsWith("/"))
    logFileName += "/";
  
  logFileName += tm.year;
  if(tm.month < 10)
    logFileName += '0';
  logFileName += tm.month;

 if(tm.dayOfMonth < 10)
  logFileName += '0';
 logFileName += tm.dayOfMonth;

  logFileName += F(".LOG");

  //DBG(F("WRITE INFO TO: "));
  //DBGLN(logFileName);

  SdFile file;
  file.open(logFileName.c_str(),FILE_WRITE);
  
  if(file.isOpen())
  {

    DBG(F("LOG >> "));
    DBGLN(line);
    
    file.print(line);
    file.close();
  }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void LoggerClass::writeLine(const String& line)
{
  if(line.length())
    write(line);
    
  write("\r\n");

}
*/
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

