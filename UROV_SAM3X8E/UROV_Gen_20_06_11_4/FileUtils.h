#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <SdFat.h>
#include <Arduino.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern SdFat SD;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SDInit
{
  public:
    static bool sdInitFlag;
    static bool sdInitResult;
    static bool InitSD();
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileUtils
{
  public:

    static int CountFiles(const String& dirName);
    static void SendToStream(Stream* s, const String& fileName);
    static void SendToStream(Stream& s, const String& fileName);
    static void printFilesNames(const String& dirName, bool recursive, Stream* outStream);
    static String getFileName(SdFile &f);
    static void deleteFile(const String& fileName);
    static uint32_t getFileSize(const String& fileName);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
