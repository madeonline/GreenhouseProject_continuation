#include "StatModule.h"
#include "ModuleController.h"
//--------------------------------------------------------------------------------------------------------------------------------------
#if (TARGET_BOARD == STM32_BOARD)
extern "C" char* sbrk(int i);
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
// выводит свободную память
//--------------------------------------------------------------------------------------------------------------------------------------
int freeRam() 
{
#if (TARGET_BOARD == STM32_BOARD)

    char top = 't';
    return &top - reinterpret_cast<char*>(sbrk(0));
 #else
  #error "Unknown target board!"
 #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void StatModule::Setup()
{
  // настройка модуля статистики тут
  uptime = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void StatModule::Update()
{ 
  // обновление модуля статистики тут
  static uint32_t _timer = 0;
  uint32_t now = millis();
  uint32_t dt = now - _timer;
  _timer = now;
  
  uptime += dt;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool  StatModule::ExecCommand(const Command& command, bool wantAnswer)
{
  if(wantAnswer) PublishSingleton = UNKNOWN_COMMAND;
  PublishSingleton.Flags.AddModuleIDToAnswer = false;

  size_t argsCount = command.GetArgsCount();
  
  if(command.GetType() == ctSET) 
  {
      if(wantAnswer) 
        PublishSingleton = NOT_SUPPORTED;
  }
  else
  if(command.GetType() == ctGET) //получить статистику
  {

    if(!argsCount) // нет аргументов
    {
      if(wantAnswer) PublishSingleton = PARAMS_MISSED;
    }
    else
    {
        String t = command.GetArg(0);

        if(t == FREERAM_COMMAND) // запросили данные о свободной памяти
        {
         PublishSingleton.Flags.Status = true;
          if(wantAnswer) 
          {
            PublishSingleton = FREERAM_COMMAND; 
            PublishSingleton << PARAM_DELIMITER << freeRam();
          }
        }
        else
        if(t == UPTIME_COMMAND) // запросили данные об аптайме
        {
          PublishSingleton.Flags.Status = true;
          if(wantAnswer) 
          {
            PublishSingleton = UPTIME_COMMAND; 
            PublishSingleton << PARAM_DELIMITER <<  (unsigned long) uptime/1000;
          }
        }
     #ifdef USE_DS3231_REALTIME_CLOCK   
        else if(t == CURDATETIME_COMMAND)
        {
           RealtimeClock rtc = MainController->GetClock();
           RTCTime tm = rtc.getTime();
           if(wantAnswer) 
           {
             PublishSingleton = rtc.getDayOfWeekStr(tm);
             PublishSingleton << F(" ") << (rtc.getDateStr(tm)) << F(" ");
             // Глюк компилятора? Если поставить все команды в одну строку - вместо времени ещё раз выведется дата! 
             PublishSingleton << rtc.getTimeStr(tm);
           }
          PublishSingleton.Flags.Status = true;
        }
      #endif  
        else
        {
          // неизвестная команда
        } // else

    }// have arguments
    
  } // if
 
 // отвечаем на команду
    MainController->Publish(this,command);
    
  return PublishSingleton.Flags.Status;
}
//--------------------------------------------------------------------------------------------------------------------------------------
