#pragma once

#include <Arduino.h>
#include "TinyVector.h"
//--------------------------------------------------------------------------------------------------------------------------------------
// класс для накопления команды из потока
//--------------------------------------------------------------------------------------------------------------------------------------
class CoreCommandBuffer
{
private:
  Stream* pStream;
  String* strBuff;
public:
  CoreCommandBuffer(Stream* s);

  bool hasCommand();
  const String& getCommand() {return *strBuff;}
  void clearCommand() {delete strBuff; strBuff = new String(); }
  Stream* getStream() {return pStream;}

};
//--------------------------------------------------------------------------------------------------------------------------------------
extern CoreCommandBuffer Commands;
//--------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<char*> CommandArgsVec;
//--------------------------------------------------------------------------------------------------------------------------------------
class CommandParser
{
  private:
    CommandArgsVec arguments;
  public:
    CommandParser();
    ~CommandParser();

    void clear();
    bool parse(const String& command, bool isSetCommand);
    const char* getArg(size_t idx) const;
    size_t argsCount() const {return arguments.size();}
};
//--------------------------------------------------------------------------------------------------------------------------------------
class CommandHandlerClass
{
  public:
  
    CommandHandlerClass();
    
    void handleCommands();
    void processCommand(const String& command,Stream* outStream);


 private:
  void onUnknownCommand(const String& command, Stream* outStream);
  
  bool getFILE(const char* commandPassed, const CommandParser& parser, Stream* pStream);
  bool getLS(const char* commandPassed, const CommandParser& parser, Stream* pStream);
  
  bool getPIN(const char* commandPassed, const CommandParser& parser, Stream* pStream);
  bool setPIN(CommandParser& parser, Stream* pStream);
  int16_t getPinState(uint8_t pin);

  int getFreeMemory();
  bool getFREERAM(const char* commandPassed, Stream* pStream);

  bool setDATETIME(const char* param);
  bool getDATETIME(const char* commandPassed, Stream* pStream);
  void setCurrentDateTime(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t minute, uint8_t second);

  bool printBackSETResult(bool isOK, const char* command, Stream* pStream);
    
};
//--------------------------------------------------------------------------------------------------------------------------------------
extern CommandHandlerClass CommandHandler;
//--------------------------------------------------------------------------------------------------------------------------------------

