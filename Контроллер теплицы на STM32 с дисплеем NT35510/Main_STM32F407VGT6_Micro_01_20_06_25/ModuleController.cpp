#include "ModuleController.h"
#include "InteropStream.h"
#include "PowerManager.h"
#include "Utils.h"

#include "UniversalSensors.h"
#include "AlertModule.h"
#if defined(USE_WIFI_MODULE) || defined(USE_SMS_MODULE)
#include "CoreTransport.h"
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
PublishStruct PublishSingleton;
ModuleController* MainController = NULL;
SdFatSdio SDFat;
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_DS3231_REALTIME_CLOCK
void setFileDateTime(uint16_t* date, uint16_t* time) 
{
  RealtimeClock rtc = MainController->GetClock();
  RTCTime tm = rtc.getTime();

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(tm.year, tm. month, tm. dayOfMonth);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(tm.hour, tm. minute, tm. second);
}
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
void FileUtils::RemoveFiles(const String& dirName, bool recursive)
{
  const char* dirP = dirName.c_str();
  
  if(!SDFat.exists(dirP))
  {
    yield();
    return;
  }

  SdFile root;
  if(!root.open(dirP,FILE_READ))
  {
    yield();
    return;
  }

  root.rewind();
  yield();


  SdFile entry;
  while(entry.openNext(&root,FILE_READ))
  {
    yield();
    
    if(entry.isDir())
    {
      if(recursive)
      {
        String subPath = dirName + "/";
        subPath += FileUtils::GetFileName(entry);        
        FileUtils::RemoveFiles(subPath,recursive);

        if(!SDFat.rmdir(subPath.c_str()))
        {
        }
        else
        {
          entry.close();
        }
      }
    }
    else
    {
     String subPath = dirName + "/" + FileUtils::GetFileName(entry);

      if(!SDFat.remove(subPath.c_str()))
      {
      }
      else
      {
        entry.close();
      }
      yield();
    }
  } // while


  root.close();
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
int FileUtils::CountFiles(const String& dirName, bool recursive)
{
  int result = 0;
  const char* dirP = dirName.c_str();
  
  if(!SDFat.exists(dirP))
    return result;

  SdFile root;
  if(!root.open(dirP,O_READ))
    return result;

  yield();
  root.rewind();

  SdFile entry;
  while(entry.openNext(&root,O_READ))
  {
    yield();
    if(entry.isDir())
    {
      if(recursive)
      {
        String subPath = dirName + "/";
        subPath += FileUtils::GetFileName(entry);
        result += FileUtils::CountFiles(subPath,recursive);      
      }
    }
    else
    {      
      result++;
    }
    entry.close();
  } // while


  root.close();
  return result;

}
//--------------------------------------------------------------------------------------------------------------------------------------
String FileUtils::GetFileName(SdFile& f)
{
      char nameBuff[50] = {0};
      f.getName(nameBuff,50);
      yield();
      return nameBuff;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void FileUtils::readLine(SdFile& f, String& result)
{
  if(!f.isOpen())
    return;
    
    while(1)
    {
      int iCh = f.read();
      
      if(iCh == -1)
        break;

      yield();

      char ch = (char) iCh;

      if(ch == '\r')
        continue;

      if(ch == '\n')
        break;

      result += ch;
    }  
}
//--------------------------------------------------------------------------------------------------------------------------------
ModuleController::ModuleController() : cParser(NULL)
#ifdef USE_LOG_MODULE
,logWriter(NULL)
#endif
{
  reservationResolver = NULL;
  httpQueryProviders[0] = NULL;
  httpQueryProviders[1] = NULL;
  PublishSingleton.Text.reserve(SHARED_BUFFER_LENGTH); // 500 байт для ответа от модуля должно хватить.
}
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_DS3231_REALTIME_CLOCK
RealtimeClock& ModuleController::GetClock()
{
  return _rtc;
}
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::begin()
{
 // тут можно написать код, который выполнится непосредственно перед началом работы
  
 UniDispatcher.Setup(); // настраиваем диспетчера универсальных датчиков

 #ifdef USE_FEEDBACK_MANAGER
 FeedbackManager.Setup();
 #endif
 
}
//--------------------------------------------------------------------------------------------------------------------------------------
OneState* ModuleController::GetReservedState(AbstractModule* sourceModule, ModuleStates sensorType, uint8_t sensorIndex)
{
  if(!reservationResolver)
  {
    return NULL;
  }

  return reservationResolver->GetReservedState(sourceModule,sensorType, sensorIndex);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Setup()
{  
// Serial.println("S 1");
  
  MainController = this;
  settings.setup();

#ifdef USE_DS3231_REALTIME_CLOCK

  #ifdef USE_INTERNAL_CLOCK
    _rtc.begin();
  #else
    _rtc.begin(DS3231_WIRE_NUMBER);
  #endif

SdFile::dateTimeCallback(setFileDateTime);
#endif


//Serial.println("S 2");

#ifdef SD_USED


 #if TARGET_BOARD == STM32_BOARD
  delay(50);

  sdCardInitFlag = SDFat.begin();//(SDCARD_CS_PIN, SPI_HALF_SPEED); // пробуем инициализировать SD-модуль
  if(!sdCardInitFlag)
  {
    delay(50);
    sdCardInitFlag = SDFat.begin();//(SDCARD_CS_PIN, SPI_QUARTER_SPEED); // пробуем инициализировать SD-модуль
  }
  
 #else

  WORK_STATUS.PinMode(SDCARD_CS_PIN,OUTPUT);
  WORK_STATUS.PinWrite(SDCARD_CS_PIN,HIGH);
    
    delay(50);
    sdCardInitFlag = SDFat.begin(SDCARD_CS_PIN); // пробуем инициализировать SD-модуль
    if(!sdCardInitFlag)
    {
      delay(50);
      sdCardInitFlag = SDFat.begin(SDCARD_CS_PIN, SPI_HALF_SPEED);
      if(!sdCardInitFlag)
        {
          delay(50);
          sdCardInitFlag = SDFat.begin(SDCARD_CS_PIN, SPI_QUARTER_SPEED); // пробуем инициализировать SD-модуль
        }
    }

  WORK_STATUS.PinMode(SDCARD_CS_PIN,OUTPUT,false);
  WORK_STATUS.PinMode(MOSI,OUTPUT,false);
  WORK_STATUS.PinMode(MISO,INPUT,false);
  WORK_STATUS.PinMode(SCK,OUTPUT,false);
    
 #endif
 
#endif // SD_USED

//Serial.println("S 3");

  #if defined(USE_MCP23S17_EXTENDER) && COUNT_OF_MCP23S17_EXTENDERS > 0

    WORK_STATUS.PinMode(MOSI,OUTPUT,false);
    WORK_STATUS.PinMode(MISO,INPUT,false);
    WORK_STATUS.PinMode(SCK,OUTPUT,false);
  
    WORK_STATUS.InitMcpSPIExtenders();
  #endif
  #if defined(USE_MCP23017_EXTENDER) && COUNT_OF_MCP23017_EXTENDERS > 0
    WORK_STATUS.InitMcpI2CExtenders();
  #endif 
  

//Serial.println("S 4");  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Log(AbstractModule* mod, const String& message)
{
 #ifdef USE_LOG_MODULE
  if(!logWriter)
    return;

  LogAction la;
  la.RaisedModule = mod;
  la.Message = message;
  logWriter->WriteAction(la); // пишем в лог
 #else
  UNUSED(mod);
  UNUSED(message);
 #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::RegisterModule(AbstractModule* mod)
{
  if(mod)
  {
    mod->Setup(); // настраиваем
    modules.push_back(mod);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::streamWrite(Stream* s, const String& str)
{
    for(size_t i=0;i<str.length();i++)
    {
      #ifdef USE_WIFI_MODULE
        ESP.readFromStream();
      #endif

     #ifdef USE_SMS_MODULE
     // и модуль GSM тоже тут обновим
     SIM800.readFromStream();
     #endif       
      
      s->write(str[i]);
     
    } // for
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::PublishToCommandStream(AbstractModule* module,const Command& sourceCommand)
{

  Stream* ps = sourceCommand.GetIncomingStream();
 
  // Публикуем в переданный стрим
  if(!ps)
  { 
    PublishSingleton.Flags.Busy = false; // освобождаем структуру
    return;
  }

     //ps->print(PublishSingleton.Flags.Status ? OK_ANSWER : ERR_ANSWER);
     streamWrite(ps,PublishSingleton.Flags.Status ? OK_ANSWER : ERR_ANSWER);

     streamWrite(ps,COMMAND_DELIMITER);
     //ps->print(COMMAND_DELIMITER);

    if(PublishSingleton.Flags.AddModuleIDToAnswer && module) // надо добавить имя модуля в ответ
    {
       streamWrite(ps,module->GetID());
       //ps->print(module->GetID());

       streamWrite(ps,PARAM_DELIMITER);
       //ps->print(PARAM_DELIMITER);
    }

     streamWrite(ps,PublishSingleton.Text);
     streamWrite(ps,NEWLINE);
     //ps->println(PublishSingleton.Text);

   
   PublishSingleton.Flags.Busy = false; // освобождаем структуру
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Publish(AbstractModule* module,const Command& sourceCommand)
{
  PublishToCommandStream(module,sourceCommand); 
}
//--------------------------------------------------------------------------------------------------------------------------------------
AbstractModule* ModuleController::GetModuleByID(const String& id)
{
  size_t sz = modules.size();
  for(size_t i=0;i<sz;i++)
  { 
    AbstractModule* mod = modules[i];
    if(!strcmp(mod->GetID(),id.c_str()) )
      return mod;
  } // for
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::ProcessModuleCommand(const Command& c, AbstractModule* mod)
{

if(!mod) // ничего не передали, надо искать модуль
  mod =  GetModuleByID(c.GetTargetModuleID());
  
 if(!mod)
 {
  // Сообщаем в тот поток, откуда пришел запрос.
  PublishSingleton.Flags.AddModuleIDToAnswer = false;
  PublishSingleton.Flags.Status = false;
  PublishSingleton = UNKNOWN_MODULE;
  PublishToCommandStream(mod,c);
  return;
 }
 
 // нашли модуль
 PublishSingleton.Reset(); // очищаем структуру для публикации
 PublishSingleton.Flags.Busy = true; // говорим, что структура занята для публикации
 mod->ExecCommand(c,true);//c.GetIncomingStream() != NULL); // выполняем его команду
 
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Alarm(AlertRule* rule)
{
  #ifdef USE_ALARM_DISPATCHER
    alarmDispatcher.Alarm(rule);
  #else
    UNUSED(rule);
  #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::RemoveAlarm(AlertRule* rule)
{
  #ifdef USE_ALARM_DISPATCHER
    alarmDispatcher.RemoveAlarm(rule);
  #else
    UNUSED(rule);
  #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::UpdateModules(CallbackUpdateFunc func)
{  
  
  // обновляем менеджер питания
  PowerManager.update();
  
  
 #ifdef USE_FEEDBACK_MANAGER
 FeedbackManager.Update(); // обновляем состояние менеджера обратной связи
 #endif
  
  size_t sz = modules.size();
  for(size_t i=0;i<sz;i++)
  { 
    AbstractModule* mod = modules[i];

    #ifdef DEBUG_MEASURE_UPDATE_TIME
    uint32_t start = millis();
    #endif
    
      // ОБНОВЛЯЕМ СОСТОЯНИЕ МОДУЛЕЙ
      mod->Update();

   #ifdef DEBUG_MEASURE_UPDATE_TIME
   uint32_t end = millis() - start;
   Serial.print("Module \"");
   Serial.print(mod->GetID());
   Serial.print("\" update time, ms: ");
   Serial.println(end);
   #endif
   

    if(func) // вызываем функцию после обновления каждого модуля
      func(mod);
  
  } // for
}
//--------------------------------------------------------------------------------------------------------------------------------------
#if (TARGET_BOARD == STM32_BOARD) && defined(PROTECT_ENABLED)
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::saveMACChallenge(const String& mac)
{
  macChallenge = mac;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool ModuleController::testMACChallenge()
{
  String sMacResponse = String(MAC_RESPONSE);
  return (sMacResponse == macChallenge);
}
//--------------------------------------------------------------------------------------------------------------------------------------
#endif // (TARGET_BOARD == STM32_BOARD) && defined(PROTECT_ENABLED)
//--------------------------------------------------------------------------------------------------------------------------------------
bool ModuleController::checkReg()
{
  #if (TARGET_BOARD == STM32_BOARD) && defined(PROTECT_ENABLED)
    return testMACChallenge();    
  #endif

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
