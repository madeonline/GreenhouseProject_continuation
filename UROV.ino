#include <Arduino.h>
#include "UTFTMenu.h"
#include "DS3231.h"       // подключаем часы
#include "ConfigPin.h"
#include "AT24CX.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// подключаем наши экраны
#include "Screen1.h"      // Главный экран
#include "Screen2.h"      // Вызов меню настроек
#include "Screen3.h"      //
#include "Screen4.h"      // Вызов меню установки времени и даты
#include "Screen5.h"      // Вызов установки времени
#include "Screen6.h"      // Вызов установки даты
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  
  ConfigPin::setup();
 
  Serial.println("Init ADC...");
  RealtimeClock.begin(1);           // запускаем их на шине I2C 1 (SDA1, SCL1);
 // RealtimeClock.setTime(0,1,11,1,7,2,2018);

  Screen.setup();


  Serial.println("Init screen...");
  Screen.setup();

  Serial.println("Add screen1..."); 

  Screen.addScreen(Screen1::create());           // первый экран покажется по умолчанию
  Serial.println("Add screen2...");

  // добавляем второй экран
  Screen.addScreen(Screen2::create());
  Serial.println("Add screen3...");
  // добавляем третий экран. Переход в меню настройки
  Screen.addScreen(Screen3::create());
  Serial.println("Add screen4...");
  // добавляем четвертый экран. Меню установки даты и времени
  Screen.addScreen(Screen4::create());
  Serial.println("Add screen5...");
  // добавляем 5 экран. Установка времени
  Screen.addScreen(Screen5::create());
  Serial.println("Add screen6...");
  // добавляем 6 экран. Установка даты
  Screen.addScreen(Screen6::create());

  // переключаемся на первый экран
  Screen.switchToScreen("Main");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{

  Screen.update();

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

