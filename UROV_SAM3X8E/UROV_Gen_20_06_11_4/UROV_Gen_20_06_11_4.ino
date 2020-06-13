#include <Arduino.h>
#include "CONFIG.h"
#include "UTFTMenu.h"
#include "DS3231.h"               // подключаем часы
#include "ConfigPin.h"
#include "AT24CX.h"
#include "InterruptHandler.h"
#include "DelayedEvents.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// подключаем наши экраны
#include "Screen1.h"              // Главный экран
#include "Screen2.h"              // Вызов меню настроек
#include "Screen3.h"              //
#include "Screen4.h"              // Вызов меню установки времени и даты
#include "Screen5.h"              // Вызов установки времени
#include "Screen6.h"              // Вызов установки даты
#include "InterruptScreen.h"      // экран с графиком прерывания
#include "Buttons.h"              // наши железные кнопки
#include "Feedback.h"             // обратная связь (диоды и прочее)
#include "FileUtils.h"
#include "Settings.h"
#include "CoreCommandBuffer.h"
#include <Wire.h>
#include "Endstops.h"
#include "Relay.h"
#include "DS18B20Query.h"
#include "Buzzer.h"
#include "CreateEncoderChartScreen.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t screenIdleTimer = 0;
bool setupDone = false;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void screenAction(AbstractTFTScreen* screen)
{
   // какое-то действие на экране произошло.
   // тут просто сбрасываем таймер ничегонеделанья.
   screenIdleTimer = millis();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkTemperatureAlert()
{
  DS18B20Temperature t1 = TempSensors.getTemperature(0);
  DS18B20Temperature t2 = TempSensors.getTemperature(1);

  int8_t alarmBorder =  Settings.getTemperatureAlertBorder();

  bool hasAlarm = false;
  
  if(t1.hasData())
  {
    int t = t1.Whole;
    if(t1.Negative)
    {
      t = -t;
    }

    if(t >= alarmBorder)
    {
      hasAlarm = true;
    }
  }

  if(t2.hasData())
  {
    int t = t2.Whole;
    if(t2.Negative)
    {
      t = -t;
    }

    if(t >= alarmBorder)
    {
      hasAlarm = true;
    }
  }

  if(hasAlarm) // есть превышение по перегреву, выключаем шунты
  {
      Relay_Shunt1.off(); // выключаем первый шунт
      Relay_Shunt2.off(); // выключаем второй шунт   
      Relay_LineALL.off(); // выключаем общую линию
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(SERIAL_SPEED);
  while(!Serial);

  DBGLN(F("Init I2C..."));
  // на первом I2C у нас память висит
  Wire.begin();
  Wire.setClock(I2C_SPEED);

  // на втором I2C могут висеть часы
  #if DS3231_WIRE_NUMBER == 1
  Wire1.begin();
  Wire1.setClock(I2C_SPEED);
  #endif
  
  ConfigPin::setI2CPriority(5);
  DBGLN(F("I2C inited."));

  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
  Serial.setInterruptPriority(2);

  ConfigPin::setup();

  DBGLN(F("Init relays..."));
  Relay_LineALL.begin();
  Relay_LineA.begin();
  Relay_LineB.begin();
  Relay_LineC.begin();
  Relay_Shunt1.begin();
  Relay_Shunt2.begin();
  DBGLN(F("Relays inited."));
 
  
  DBGLN(F("Init settings..."));
  Settings.begin();
  DBGLN(F("Settings inited."));

  TempSensors.begin();
  
  DBGLN(F("Init RTC..."));
  RealtimeClock.begin(DS3231_WIRE_NUMBER);           // запускаем их на шине I2C 1 (SDA1, SCL1);
 // RealtimeClock.setTime(0,1,11,1,7,2,2018);


  DBGLN(F("Init endstops..."));
  SetupEndstops();

#ifndef _SD_OFF

  DBGLN(F("INIT SD..."));
  if (SDInit::InitSD())
  {
	  DBGLN(F("SD inited."));
  }
  else
  {
	  DBGLN(F("ОШИБКА ИНИЦИАЛИЗАЦИИ SD!!"));
  }
#endif // !_SD_OFF
  

  DBGLN(F("Init screen..."));
  Screen.setup();

  DBGLN(F("Add screen1...")); 
  Screen.addScreen(Screen1::create());           // первый экран покажется по умолчанию

#ifndef _SCREEN_2_OFF
  DBGLN(F("Add screen2..."));
  // добавляем второй экран
  Screen.addScreen(Screen2::create());
#endif // !_SCREEN_2_OFF

#ifndef _SCREEN_3_OFF
  DBGLN(F("Add screen3..."));
  // добавляем третий экран. Переход в меню настройки
  Screen.addScreen(Screen3::create());
#endif // !_SCREEN_3_OFF

#ifndef _SCREEN_4_OFF
  DBGLN(F("Add screen4..."));
  // добавляем четвертый экран. Меню установки даты и времени
  Screen.addScreen(Screen4::create());
#endif // !_SCREEN_4_OFF

#ifndef _SCREEN_5_OFF
  DBGLN(F("Add screen5..."));
  // добавляем 5 экран. Установка времени
  Screen.addScreen(Screen5::create());
#endif // !_SCREEN_5_OFF

#ifndef _SCREEN_6_OFF
  DBGLN(F("Add screen6..."));
  // добавляем 6 экран. Установка даты
  Screen.addScreen(Screen6::create());
#endif // !_SCREEN_6_OFF


  Screen.addScreen(CreateEncoderChartScreen::create());

  DBGLN(F("Add interrupt screen..."));
  // добавляем экран с графиком прерываний
  Screen.addScreen(InterruptScreen::create());

  // переключаемся на первый экран
  Screen.switchToScreen("Main");

  // настраиваем обратную связь (информационные диоды и пр.)
  Feedback.begin();

  // настраиваем железные кнопки
  Buttons.begin();

  // поднимаем наши прерывания
  InterruptHandler.begin();

  screenIdleTimer = millis();
  Screen.onAction(screenAction);

  DBGLN(F("Inited."));

  Serial.print(F("UROV "));
  Serial.println(SOFTWARE_VERSION);

  Buzzer.begin();
  Buzzer.buzz();


  setupDone = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{

  checkTemperatureAlert(); // проверяем по перегреву

#ifndef _DELAYED_EVENT_OFF
  CoreDelayedEvent.update();
#endif // _DELAYED_EVENT_OFF

  Settings.update();

  TempSensors.update();
  
  // обновляем кнопки
  Buttons.update();

  Screen.update();

  InterruptHandler.update();

  // проверяем, какой экран активен. Если активен главный экран - сбрасываем таймер ожидания. Иначе - проверяем, не истекло ли время ничегонеделанья.
  AbstractTFTScreen* activeScreen = Screen.getActiveScreen();
  if(activeScreen == mainScreen)
  {
    screenIdleTimer = millis();
  }
  else
  {
      if(millis() - screenIdleTimer > RESET_TO_MAIN_SCREEN_DELAY)
      {
		  DBGLN(F("ДОЛГОЕ БЕЗДЕЙСТВИЕ, ПЕРЕКЛЮЧАЕМСЯ НА ГЛАВНЫЙ ЭКРАН!"));
        screenIdleTimer = millis();
        Screen.switchToScreen(mainScreen);
      }
  } // else


#ifndef _COM_COMMANDS_OFF
  // обрабатываем входящие команды
  CommandHandler.handleCommands();
#endif // _COM_COMMANDS_OFF



}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool nestedYield = false;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _YIELD_OFF
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void yield()
{  
  if(nestedYield || !setupDone)
    return;
    
 nestedYield = true;
  
   // обновляем прерывания
   InterruptHandler.update();

#ifndef _DELAYED_EVENT_OFF
   CoreDelayedEvent.update();
#endif // _DELAYED_EVENT_OFF

   Buttons.update();


 nestedYield = false;
 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // _YIELD_OFF
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

