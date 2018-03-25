//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "FileUtils.h"
#include "ConfigPin.h"
#include "DS3231.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SdFat SD;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDInit::sdInitFlag = false;
bool SDInit::sdInitResult = false;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FileUtils
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setFileDateTime(uint16_t* date, uint16_t* time) 
{
  DS3231Time tm = RealtimeClock.getTime();

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(tm.year, tm. month, tm. dayOfMonth);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(tm.hour, tm. minute, tm. second);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FileUtils::CountFiles(const String& dirName)
{
  if(!SDInit::sdInitResult)
    return 0;

  int result = 0;

  if(!SD.exists(dirName.c_str()))
    return result;
  
  SD.chdir(dirName.c_str());
  SD.vwd()->rewind();
   
  SdFile file;

  while(file.openNext(SD.vwd(), O_READ)) 
  {
    if(!file.isHidden())
      result++;

    file.close();
  }  

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDInit::InitSD()
{
  if(SDInit::sdInitFlag)
    return SDInit::sdInitResult;

  SDInit::sdInitFlag = true;
  SDInit::sdInitResult = SD.begin(SD_CS_PIN,SPI_HALF_SPEED);
  SdFile::dateTimeCallback(setFileDateTime);
  
  return SDInit::sdInitResult;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
