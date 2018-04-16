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
void FileUtils::deleteFile(const String& fileName)
{
  if(!SDInit::sdInitResult)
    return;
      
  SD.remove(fileName.c_str());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t FileUtils::getFileSize(const String& fileName)
{
  if(!SDInit::sdInitResult)
    return 0;
      
  uint32_t result = 0;
  SdFile f;
  if(f.open(fileName.c_str(),O_READ))
  {
    result = f.fileSize();
    f.close();
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FileUtils::CountFiles(const String& dirName)
{
  if(!SDInit::sdInitResult)
    return 0;

  SdFile root;
  if(!root.open(dirName.c_str(),O_READ))
    return 0;

  int result = 0;
  root.rewind();

  SdFile entry;
  while(entry.openNext(&root,O_READ))
  {
    entry.close();
    result++;  
  }

  root.close();
  return result;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileUtils::SendToStream(Stream& s, const String& fileName)
{
  FileUtils::SendToStream(&s,fileName);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileUtils::SendToStream(Stream* s, const String& fileName)
{
  if(!SDInit::sdInitResult)
    return;
      
  SdFile file;
  file.open(fileName.c_str(),FILE_READ);

    if(file.isDir())
    {
      file.close();
      return;
    }  

  if(file.isOpen())
  {
     while(1)
    {
      int iCh = file.read();
      if(iCh == -1)
        break;

        s->write((uint8_t) iCh);
    }

    file.close();
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String FileUtils::getFileName(SdFile &f)
{
    char nameBuff[50] = {0};
    f.getName(nameBuff,50);
    return nameBuff;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileUtils::printFilesNames(const String& dirName, bool recursive, Stream* outStream)
{  
  if(!SDInit::sdInitResult)
    return;
      
  const char* dirP = dirName.c_str(); 

  SdFile root;
  if(!root.open(dirP,O_READ))
    return;

  root.rewind();

  SdFile entry;
  while(entry.openNext(&root,O_READ))
  {
    if(entry.isDir())
    {
      String currentDirName =  FileUtils::getFileName(entry);
      outStream->print(currentDirName);
      outStream->println(F("\t<DIR>"));
      
      if(recursive)
      {
        String subPath = dirName + "/";
        subPath += currentDirName;
        FileUtils::printFilesNames(subPath,recursive, outStream);      
      }
    }
    else
    {      
      outStream->println(FileUtils::getFileName(entry));
    }
    entry.close();
  } // while


  root.close();
 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDInit
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

