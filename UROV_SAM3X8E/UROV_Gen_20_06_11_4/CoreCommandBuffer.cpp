#include "CoreCommandBuffer.h"
#include "CONFIG.h"
#include "FileUtils.h"
#include <avr/pgmspace.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include "DS3231.h"
#include "Settings.h"
//--------------------------------------------------------------------------------------------------------------------------------------
// список поддерживаемых команд
//--------------------------------------------------------------------------------------------------------------------------------------
const char DATETIME_COMMAND[] PROGMEM = "DATETIME"; // получить/установить дату/время на контроллер
const char FREERAM_COMMAND[] PROGMEM = "FREERAM"; // получить информацию о свободной памяти
const char PIN_COMMAND[] PROGMEM = "PIN"; // установить уровень на пине
const char LS_COMMAND[] PROGMEM = "LS"; // отдать список файлов
const char FILE_COMMAND[] PROGMEM = "FILE"; // отдать содержимое файла
const char FILESIZE_COMMAND[] PROGMEM = "FILESIZE"; // отдать размер файла
const char DELFILE_COMMAND[] PROGMEM = "DELFILE"; // удалить файл
const char UPLOADFILE_COMMAND[] PROGMEM = "UPL"; // загрузить файл
const char MOTORESOURCE_CURRENT_COMMAND[] PROGMEM = "RES_CUR"; // получить текущий моторесурс по каналам
const char MOTORESOURCE_MAX_COMMAND[] PROGMEM = "RES_MAX"; // получить максимальный моторесурс по каналам
const char PULSES_COMMAND[] PROGMEM = "PULSES"; // получить импульсы по каналам
const char DELTA_COMMAND[] PROGMEM = "DELTA"; // получить дельты по каналам
const char INDUCTIVE_COMMAND[] PROGMEM = "IND"; // получить состояние индуктивных датчиков
const char VOLTAGE_COMMAND[] PROGMEM = "VDATA"; // получить вольтаж на входах
const char UUID_COMMAND[] PROGMEM = "UUID"; // получить уникальный идентификатор контроллера
const char TBORDERMAX_COMMAND[] PROGMEM = "TBORDERMAX"; // верхний порог токового трансформатора
const char TBORDERMIN_COMMAND[] PROGMEM = "TBORDERMIN"; // нижний порог токового трансформатора
const char TBORDERS_COMMAND[] PROGMEM = "TBORDERS"; // пороги токового трансформатора
const char RDELAY_COMMAND[] PROGMEM = "RDELAY"; // время задержки после срабатывания реле до начала импульсов
//--------------------------------------------------------------------------------------------------------------------------------------
CoreCommandBuffer Commands(&Serial);
//--------------------------------------------------------------------------------------------------------------------------------------
CoreCommandBuffer::CoreCommandBuffer(Stream* s) : pStream(s)
{
    strBuff = new String();
	strBuff->reserve(201);
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreCommandBuffer::hasCommand()
{
  if(!(pStream && pStream->available()))
    return false;

    char ch;
    while(pStream->available())
    {
      ch = (char) pStream->read();

      if(ch == '\r')
        continue;
            
      if(ch == '\n')
      {
        return strBuff->length() > 0; // вдруг лишние управляющие символы придут в начале строки?
      } // if

      *strBuff += ch;
      // не даём вычитать больше символов, чем надо - иначе нас можно заспамить
      if(strBuff->length() >= 200)
      {
         clearCommand();
         return false;
      } // if
    } // while

    return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
CommandParser::CommandParser()
{
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
CommandParser::~CommandParser()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CommandParser::clear()
{
  for(size_t i=0;i<arguments.size();i++)
  {
    delete [] arguments[i];  
  }

  arguments.empty();
  //while(arguments.size())
  //  arguments.pop();
}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* CommandParser::getArg(size_t idx) const
{
  if(arguments.size() && idx < arguments.size())
    return arguments[idx];

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandParser::parse(const String& command, bool isSetCommand)
{
  clear();
    // разбиваем на аргументы
    const char* startPtr = command.c_str() + strlen_P(isSetCommand ? (const char* )CORE_COMMAND_SET : (const char*) CORE_COMMAND_GET);
    size_t len = 0;

    while(*startPtr)
    {
      const char* delimPtr = strchr(startPtr,CORE_COMMAND_PARAM_DELIMITER);
            
      if(!delimPtr)
      {
        len = strlen(startPtr);
        char* newArg = new char[len + 1];
        memset(newArg,0,len+1);
        strncpy(newArg,startPtr,len);
        arguments.push_back(newArg);        

        return arguments.size();
      } // if(!delimPtr)

      size_t len = delimPtr - startPtr;

     
      char* newArg = new char[len + 1];
      memset(newArg,0,len+1);
      strncpy(newArg,startPtr,len);
      arguments.push_back(newArg);

      startPtr = delimPtr + 1;
      
    } // while      

  return arguments.size();
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
// CommandHandlerClass
//--------------------------------------------------------------------------------------------------------------------------------------
CommandHandlerClass CommandHandler;
//--------------------------------------------------------------------------------------------------------------------------------------
CommandHandlerClass::CommandHandlerClass()
{
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CommandHandlerClass::handleCommands()
{
  if(Commands.hasCommand())
  {    

    String command = Commands.getCommand();

    if(command.startsWith(CORE_COMMAND_GET) || command.startsWith(CORE_COMMAND_SET))
    {
      Stream* pStream = Commands.getStream();
      processCommand(command,pStream);
    }
    

    Commands.clearCommand(); // очищаем буфер команд
  
  } // if(Commands.hasCommand())  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CommandHandlerClass::processCommand(const String& command,Stream* pStream)
{
    bool commandHandled = false;

    if(command.startsWith(CORE_COMMAND_SET))
    {
      // команда на установку свойств

      CommandParser cParser;
      if(cParser.parse(command,true))
      {
        const char* commandName = cParser.getArg(0);

        if(!strcmp_P(commandName, PIN_COMMAND))
        {
            // запросили установить уровень на пине SET=PIN|13|ON, SET=PIN|13|1, SET=PIN|13|OFF, SET=PIN|13|0, SET=PIN|13|ON|2000 
            if(cParser.argsCount() > 2)
            {
              commandHandled = setPIN(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // PIN_COMMAND        

        else
        if(!strcmp_P(commandName, DATETIME_COMMAND)) // DATETIME
        {
          if(cParser.argsCount() > 1)
          {
          // запросили установку даты/времени, приходит строка вида 25.12.2017 12:23:49
            const char* paramPtr = cParser.getArg(1);
            commandHandled = printBackSETResult(setDATETIME(paramPtr),commandName,pStream);
          }
          else
          {
            // недостаточно параметров
            commandHandled = printBackSETResult(false,commandName,pStream);
          }
                    
        } // DATETIME
        else
        if(!strcmp_P(commandName, DELFILE_COMMAND))
        {
            if(cParser.argsCount() > 1)
            {
              commandHandled = setDELFILE(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // DELFILE_COMMAND
        else
        if(!strcmp_P(commandName,UPLOADFILE_COMMAND))
        {
            if(cParser.argsCount() > 2)
            {
              commandHandled = setUPLOADFILE(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // UPLOADFILE_COMMAND                  
        else
        if(!strcmp_P(commandName, PULSES_COMMAND))
        {
            if(cParser.argsCount() > 1)
            {
              commandHandled = setPULSES(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // PULSES_COMMAND               
        else
        if(!strcmp_P(commandName, RDELAY_COMMAND))
        {
            if(cParser.argsCount() > 2)
            {
              commandHandled = setRDELAY(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // RDELAY_COMMAND               
        else
        if(!strcmp_P(commandName, TBORDERMAX_COMMAND))
        {
            if(cParser.argsCount() > 1)
            {
              commandHandled = setTBORDERMAX(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // TBORDERMAX_COMMAND               
        else
        if(!strcmp_P(commandName, TBORDERMIN_COMMAND))
        {
            if(cParser.argsCount() > 1)
            {
              commandHandled = setTBORDERMIN(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // TBORDERMIN_COMMAND           
        else
        if(!strcmp_P(commandName, TBORDERS_COMMAND))
        {
            if(cParser.argsCount() > 2)
            {
              commandHandled = setTBORDERS(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // TBORDERS_COMMAND           
        else
        if(!strcmp_P(commandName, DELTA_COMMAND))
        {
            if(cParser.argsCount() > 1)
            {
              commandHandled = setDELTA(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // DELTA_COMMAND               
        else
        if(!strcmp_P(commandName, MOTORESOURCE_CURRENT_COMMAND))
        {
            // запросили установить текущий моторесурс SET=RES_CUR|0
            if(cParser.argsCount() > 1)
            {
              commandHandled = setMOTORESOURCE_CURRENT(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // MOTORESOURCE_CURRENT_COMMAND               
        else
        if(!strcmp_P(commandName, MOTORESOURCE_MAX_COMMAND))
        {
            // запросили установить текущий моторесурс SET=RES_CUR|0
            if(cParser.argsCount() > 1)
            {
              commandHandled = setMOTORESOURCE_MAX(cParser, pStream);
            }
            else
            {
              // недостаточно параметров
              commandHandled = printBackSETResult(false,commandName,pStream);
            }
        } // MOTORESOURCE_MAX_COMMAND               
      
      //TODO: тут разбор команды !!!
      
      } // if(cParser.parse(command,true))
      
    } // SET COMMAND
    else
    if(command.startsWith(CORE_COMMAND_GET))
    {
      // команда на получение свойств
      CommandParser cParser;
      
      if(cParser.parse(command,false))
      {
        const char* commandName = cParser.getArg(0);
        if(!strcmp_P(commandName, DATETIME_COMMAND))
        {
          commandHandled = getDATETIME(commandName,pStream);
                    
        } // DATETIME_COMMAND
        else
        if(!strcmp_P(commandName, PIN_COMMAND))
        {
            commandHandled = getPIN(commandName,cParser,pStream);                    
          
        } // PIN_COMMAND
        else
        if(!strcmp_P(commandName, PULSES_COMMAND))
        {
            commandHandled = getPULSES(commandName,cParser,pStream);                    
          
        } // PULSES_COMMAND       
        else
        if(!strcmp_P(commandName, RDELAY_COMMAND))
        {
            commandHandled = getRDELAY(commandName,cParser,pStream);                    
          
        } // RDELAY_COMMAND       
        else
        if(!strcmp_P(commandName, TBORDERMAX_COMMAND))
        {
            commandHandled = getTBORDERMAX(commandName,cParser,pStream);                    
          
        } // TBORDERMAX_COMMAND       
        else
        if(!strcmp_P(commandName, TBORDERMIN_COMMAND))
        {
            commandHandled = getTBORDERMIN(commandName,cParser,pStream);                    
          
        } // TBORDERMIN_COMMAND       
        else
        if(!strcmp_P(commandName, TBORDERS_COMMAND))
        {
            commandHandled = getTBORDERS(commandName,cParser,pStream);                    
          
        } // TBORDERS_COMMAND
		/*
		//DEPRECATED:
        else
        if(!strcmp_P(commandName, INDUCTIVE_COMMAND))
        {
            commandHandled = getINDUCTIVE(commandName,cParser,pStream);                    
          
        } // INDUCTIVE_COMMAND       
		*/
        else
        if(!strcmp_P(commandName, VOLTAGE_COMMAND))
        {
            commandHandled = getVOLTAGE(commandName,cParser,pStream);                    
          
        } // VOLTAGE_COMMAND       
        else
        if(!strcmp_P(commandName, DELTA_COMMAND))
        {
            commandHandled = getDELTA(commandName,cParser,pStream);                    
          
        } // DELTA_COMMAND       
        else
        if(!strcmp_P(commandName, UUID_COMMAND))
        {
            commandHandled = getUUID(commandName,cParser,pStream);                    
          
        } // UUID_COMMAND       
        else
        if(!strcmp_P(commandName, MOTORESOURCE_CURRENT_COMMAND))
        {
            commandHandled = getMOTORESOURCE_CURRENT(commandName,cParser,pStream);                    
          
        } // MOTORESOURCE_CURRENT_COMMAND       
        else
        if(!strcmp_P(commandName, MOTORESOURCE_MAX_COMMAND))
        {
            commandHandled = getMOTORESOURCE_MAX(commandName,cParser,pStream);                    
          
        } // MOTORESOURCE_MAX_COMMAND       
        else      
        if(!strcmp_P(commandName, FREERAM_COMMAND))
        {
          commandHandled = getFREERAM(commandName,pStream);
        } // FREERAM_COMMAND
        else
        if(!strcmp_P(commandName, LS_COMMAND)) // LS
        {
            // запросили получить список файлов в папке, GET=LS|FolderName
            commandHandled = getLS(commandName,cParser,pStream);                    
        } // LS        
        else
        if(!strcmp_P(commandName, FILE_COMMAND)) // FILE
        {
            // запросили получить файл, GET=FILE|FilePath
            commandHandled = getFILE(commandName,cParser,pStream);                    
        } // LS        
        else
        if(!strcmp_P(commandName, FILESIZE_COMMAND)) // FILESIZE
        {
            // запросили размер файла, GET=FILESIZE|FilePath
            commandHandled = getFILESIZE(commandName,cParser,pStream);                    
        } // LS        
                
        //TODO: тут разбор команды !!!
        
      } // if(cParser.parse(command,false))
      
    } // GET COMMAND
    
    if(!commandHandled)
      onUnknownCommand(command, pStream);  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CommandHandlerClass::onUnknownCommand(const String& command, Stream* outStream)
{
    outStream->print(CORE_COMMAND_ANSWER_ERROR);
    outStream->println(F("UNKNOWN_COMMAND"));  
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setUPLOADFILE(CommandParser& parser, Stream* pStream)
{
/*
  for(size_t i=0;i<parser.argsCount();i++)
    pStream->println(parser.getArg(i));
*/  
  // в первом параметре - длина данных
  int dataLen = atoi(parser.getArg(1));

  // во втором и последующих - имя файла вместе с путём
  String filePath;
  for(size_t i=2;i<parser.argsCount();i++)
  {
    if(filePath.length())
      filePath += "/";

     filePath += parser.getArg(i);
  }

  uint16_t reading_timeout = 5000;
  bool wantBreak = false;
  
 if(SDInit::sdInitResult)
 {
    String dirOnly;
    int idx = filePath.lastIndexOf("/");
    if(idx != -1)
      dirOnly = filePath.substring(0,idx);

    if(dirOnly.length())
    {
      SD.mkdir(dirOnly.c_str());
    }

    SdFile f;
    f.open(filePath.c_str(),FILE_WRITE | O_TRUNC);

     uint32_t startReadingTime = millis();
       
     for(int i=0;i<dataLen;i++)
      {
        while(!pStream->available())
        {
          if(millis() - startReadingTime > reading_timeout)
          {
            wantBreak = true;
            break;
          }
        }
        if(wantBreak)
          break;
          
        startReadingTime = millis();        
        uint8_t curByte = pStream->read();
        
        if(f.isOpen())
          f.write(curByte);
      }
            
      if(f.isOpen())
        f.close(); 
 } // if(SDInit::sdInitResult)
 else
 {
  // не удалось инициализировать SD - просто пропускаем данные файла
      uint32_t startReadingTime = millis();
      
      for(int i=0;i<dataLen;i++)
      {
        while(!pStream->available())
        {
          if(millis() - startReadingTime > reading_timeout)
          {
            wantBreak = true;
            break;
          }
        }

        if(wantBreak)
          break;
          
        startReadingTime = millis(); 
        pStream->read();        
      }  
 }

  if(wantBreak)
    pStream->print(CORE_COMMAND_ANSWER_ERROR);
  else
    pStream->print(CORE_COMMAND_ANSWER_OK);
    
  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setDELFILE(CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() > 1)
  {
    String fileName;

    for(size_t i=1;i<parser.argsCount();i++)
    {
      if(fileName.length())
        fileName += F("/");

      fileName += parser.getArg(i);
    }
    
    FileUtils::deleteFile(fileName);

    pStream->print(CORE_COMMAND_ANSWER_OK);
    pStream->print(parser.getArg(0));
    pStream->print(CORE_COMMAND_PARAM_DELIMITER);
    pStream->println(CORE_COMMAND_DONE);    

    return true;

  }
  return false;    
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getFILESIZE(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() > 1)
  {
    String fileName;

    for(size_t i=1;i<parser.argsCount();i++)
    {
      if(fileName.length())
        fileName += F("/");

      fileName += parser.getArg(i);
    }

    pStream->print(CORE_COMMAND_ANSWER_OK);
    pStream->print(parser.getArg(0));
    pStream->print(CORE_COMMAND_PARAM_DELIMITER);
    pStream->println(FileUtils::getFileSize(fileName));    
    
    return true;
  }
  return false;  
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getFILE(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  String endOfFile = CORE_END_OF_DATA;
  if(parser.argsCount() > 1)
  {
    String fileName;

    for(size_t i=1;i<parser.argsCount();i++)
    {
      if(fileName.length())
        fileName += F("/");

      fileName += parser.getArg(i);
    }
    
    FileUtils::SendToStream(pStream, fileName);
    pStream->println(endOfFile);
  }
  else
  {
    pStream->println(endOfFile);
  }
  return true;  
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getLS(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  String folderName = F("/");
  
  if(parser.argsCount() > 1)
  {
    folderName = "";
    for(size_t i=1;i<parser.argsCount();i++)
    {
      if(folderName.length())
        folderName += F("/");

      folderName += parser.getArg(i);
    }    
  }

  FileUtils::printFilesNames(folderName,false,pStream);
  pStream->println(CORE_END_OF_DATA);
  
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getVOLTAGE(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->print(Settings.get3V3Voltage().raw);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  pStream->print(Settings.get5Vvoltage().raw);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  pStream->println(Settings.get200Vvoltage().raw);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*
//DEPRECATED:
bool CommandHandlerClass::getINDUCTIVE(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->print(Settings.getInductiveSensorState(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  pStream->print(Settings.getInductiveSensorState(1));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  pStream->println(Settings.getInductiveSensorState(2));

  return true;
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getUUID(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 2)
    return false;  

  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->println(Settings.getUUID(parser.getArg(1)));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getRDELAY(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->print(Settings.getRelayDelay()/1000);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(Settings.getACSDelay());


  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setRDELAY(CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 3)
    return false;
  
  uint32_t curBorder = atoi(parser.getArg(1))*1000;
  uint16_t curAcsDelay = atoi(parser.getArg(2));

  Settings.setRelayDelay(curBorder);
  Settings.setACSDelay(curAcsDelay);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getTBORDERMAX(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->println(Settings.getTransformerHighBorder());

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setTBORDERMAX(CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 2)
    return false;
  
  uint32_t curBorder = atoi(parser.getArg(1));

  Settings.setTransformerHighBorder(curBorder);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getTBORDERMIN(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->println(Settings.getTransformerLowBorder());

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setTBORDERMIN(CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 2)
    return false;
  
  uint32_t curBorder = atoi(parser.getArg(1));

  Settings.setTransformerLowBorder(curBorder);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getTBORDERS(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->print(Settings.getTransformerLowBorder());
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  pStream->println(Settings.getTransformerHighBorder());

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setTBORDERS(CommandParser& parser, Stream* pStream)
{

  if(parser.argsCount() < 3)
    return false;
  
  uint32_t lowBorder = atoi(parser.getArg(1));
  uint32_t highBorder = atoi(parser.getArg(2));

  Settings.setTransformerLowBorder(lowBorder);
  Settings.setTransformerHighBorder(highBorder);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);


  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getDELTA(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->println(Settings.getChannelDelta(0));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->print(Settings.getChannelDelta(1));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->println(Settings.getChannelDelta(2));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setDELTA(CommandParser& parser, Stream* pStream)
{

  if(parser.argsCount() < 2)
    return false;
  
  uint8_t resCurrent1 = atoi(parser.getArg(1));
  //DEPRECATED: uint8_t resCurrent2 = atoi(parser.getArg(2));
  //DEPRECATED: uint8_t resCurrent3 = atoi(parser.getArg(3));

  Settings.setChannelDelta(0,resCurrent1);
  //DEPRECATED: Settings.setChannelDelta(1,resCurrent2);
  //DEPRECATED: Settings.setChannelDelta(2,resCurrent3);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);


  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getPULSES(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->println(Settings.getChannelPulses(0));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->print(Settings.getChannelPulses(1));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->println(Settings.getChannelPulses(2));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setPULSES(CommandParser& parser, Stream* pStream)
{

  if(parser.argsCount() < 2)
    return false;
  
  uint16_t resCurrent1 = atoi(parser.getArg(1));
  //DEPRECATED: uint16_t resCurrent2 = atoi(parser.getArg(2));
  //DEPRECATED: uint16_t resCurrent3 = atoi(parser.getArg(3));

  Settings.setChannelPulses(0,resCurrent1);
  //DEPRECATED: Settings.setChannelPulses(1,resCurrent2);
  //DEPRECATED: Settings.setChannelPulses(2,resCurrent3);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);


  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getMOTORESOURCE_MAX(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->println(Settings.getMotoresourceMax(0));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->print(Settings.getMotoresourceMax(1));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->println(Settings.getMotoresourceMax(2));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setMOTORESOURCE_MAX(CommandParser& parser, Stream* pStream)
{

  if(parser.argsCount() < 2)
    return false;
  
  uint32_t resCurrent1 = atoi(parser.getArg(1));
  //DEPRECATED: uint32_t resCurrent2 = atoi(parser.getArg(2));
  //DEPRECATED: uint32_t resCurrent3 = atoi(parser.getArg(3));

  Settings.setMotoresourceMax(0,resCurrent1);
  //DEPRECATED: Settings.setMotoresourceMax(1,resCurrent2);
  //DEPRECATED: Settings.setMotoresourceMax(2,resCurrent3);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);


  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getMOTORESOURCE_CURRENT(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 1)
    return false;  


  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  
  pStream->println(Settings.getMotoresource(0));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->print(Settings.getMotoresource(1));
  //DEPRECATED: pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  //DEPRECATED: pStream->println(Settings.getMotoresource(2));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setMOTORESOURCE_CURRENT(CommandParser& parser, Stream* pStream)
{

  if(parser.argsCount() < 2)
    return false;
  
  uint32_t resCurrent1 = atoi(parser.getArg(1));
  //DEPRECATED: uint32_t resCurrent2 = atoi(parser.getArg(2));
  //DEPRECATED: uint32_t resCurrent3 = atoi(parser.getArg(3));

  Settings.setMotoresource(0,resCurrent1);
  //DEPRECATED: Settings.setMotoresource(1,resCurrent2);
  //DEPRECATED: Settings.setMotoresource(2,resCurrent3);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(CORE_COMMAND_DONE);


  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getPIN(const char* commandPassed, const CommandParser& parser, Stream* pStream)
{
  if(parser.argsCount() < 2)
    return false;  

   int16_t pinNumber = atoi(parser.getArg(1));   
   int16_t pinState = getPinState(pinNumber);

  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->print(pinNumber);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(pinState ? F("ON") : F("OFF"));   

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setPIN(CommandParser& parser, Stream* pStream)
{

  if(parser.argsCount() < 3)
    return false;
  
  int16_t pinNumber = atoi(parser.getArg(1));
  const char* level = parser.getArg(2);
  
  bool isHigh = !strcasecmp_P(level,(const char*) F("ON")) || *level == '1';

  pinMode(pinNumber,OUTPUT);
  digitalWrite(pinNumber,isHigh);

  pStream->print(CORE_COMMAND_ANSWER_OK);

  pStream->print(parser.getArg(0));
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->print(pinNumber);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->println(level);


  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
int16_t CommandHandlerClass::getPinState(uint8_t pin)
{
  return digitalRead(pin);
}
//--------------------------------------------------------------------------------------------------------------------------------------
int CommandHandlerClass::getFreeMemory()
{
    struct mallinfo mi = mallinfo();
    char* heapend = _sbrk(0);
    register char* stack_ptr asm("sp");

    return (stack_ptr - heapend + mi.fordblks);

}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getFREERAM(const char* commandPassed, Stream* pStream)
{
  if(commandPassed)
  {
      pStream->print(CORE_COMMAND_ANSWER_OK);
      pStream->print(commandPassed);
      pStream->print(CORE_COMMAND_PARAM_DELIMITER);    
  }

  pStream->println(getFreeMemory());

  return true;
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::setDATETIME(const char* param)
{
    // разбираем параметр на составные части
    int8_t day = 0;
    int8_t month = 0;
    int16_t year = 0;
    int8_t hour = 0;
    int8_t minute = 0;
    int8_t second = 0;

    // буфер под промежуточные данные
    char workBuff[5] = {0};
    char* writePtr = workBuff;

    // извлекаем день
    const char* delim = strchr(param,'.');
    if(!delim || (delim - param > 4))
      return false;

    while(param < delim)
      *writePtr++ = *param++;
    *writePtr = 0;
    writePtr = workBuff;

    day = atoi(workBuff);

    param = delim+1; // перемещаемся на следующий компонент - месяц
    delim = strchr(param,'.');
    if(!delim || (delim - param > 4))
      return false;

    while(param < delim)
      *writePtr++ = *param++;
    *writePtr = 0;
    writePtr = workBuff;

    month = atoi(workBuff);

    param = delim+1; // перемещаемся на следующий компонент - год
    delim = strchr(param,' ');
    if(!delim || (delim - param > 4))
      return false;

    while(param < delim)
      *writePtr++ = *param++;
    *writePtr = 0;
    writePtr = workBuff;

    year = atoi(workBuff);

    param = delim+1; // перемещаемся на следующий компонент - час
    delim = strchr(param,':');
    if(!delim || (delim - param > 4))
      return false;

    while(param < delim)
      *writePtr++ = *param++;
    *writePtr = 0;
    writePtr = workBuff;

    hour = atoi(workBuff);

    param = delim+1; // перемещаемся на следующий компонент - минута
    delim = strchr(param,':');
    if(!delim || (delim - param > 4))
      return false;

    while(param < delim)
      *writePtr++ = *param++;
    *writePtr = 0;
    writePtr = workBuff;

    minute = atoi(workBuff);

    param = delim+1; // перемещаемся на следующий компонент - секунда

    while(*param && writePtr < &(workBuff[4]))
      *writePtr++ = *param++;
    *writePtr = 0;

    second = atoi(workBuff);

    setCurrentDateTime(day, month, year,hour,minute,second);
    
  return true;
 
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CommandHandlerClass::setCurrentDateTime(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t minute, uint8_t second)
{
   // вычисляем день недели
    int16_t dow;
    uint8_t mArr[12] = {6,2,2,5,0,3,5,1,4,6,2,4};
    dow = (year % 100);
    dow = dow*1.25;
    dow += day;
    dow += mArr[month-1];
    
    if (((year % 4)==0) && (month<3))
     dow -= 1;
     
    while (dow>7)
     dow -= 7;  


   RealtimeClock.setTime(second, minute, hour, dow, day, month, year);
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::getDATETIME(const char* commandPassed, Stream* pStream)
{
  // запросили получение времени/даты

  DS3231Time tm = RealtimeClock.getTime();
  String dateStr = RealtimeClock.getDateStr(tm);
  String timeStr = RealtimeClock.getTimeStr(tm);
  
  pStream->print(CORE_COMMAND_ANSWER_OK);
  pStream->print(commandPassed);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);
  pStream->print(dateStr);
  pStream->print(" ");
  pStream->println(timeStr);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CommandHandlerClass::printBackSETResult(bool isOK, const char* command, Stream* pStream)
{
  if(isOK)
    pStream->print(CORE_COMMAND_ANSWER_OK);
  else
    pStream->print(CORE_COMMAND_ANSWER_ERROR);

  pStream->print(command);
  pStream->print(CORE_COMMAND_PARAM_DELIMITER);

  if(isOK)
    pStream->println(F("OK"));
  else
    pStream->println(F("BAD_PARAMS"));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------



