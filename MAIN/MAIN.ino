#include <Arduino.h>
#include "CONFIG.h"
#include "UTFTMenu.h"
#include "DS3231.h"       // подключаем часы
#include "ConfigPin.h"
#include "AT24CX.h"
#include "InterruptHandler.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// подключаем наши экраны
#include "Screen1.h"      // Главный экран
#include "Screen2.h"      // Вызов меню настроек
#include "Screen3.h"      //
#include "Screen4.h"      // Вызов меню установки времени и даты
#include "Screen5.h"      // Вызов установки времени
#include "Screen6.h"      // Вызов установки даты
#include "InterruptScreen.h"      // экран с графиком прерывания


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  
  ConfigPin::setup();
 
  DBGLN(F("Init ADC..."));
  RealtimeClock.begin(1);           // запускаем их на шине I2C 1 (SDA1, SCL1);
 // RealtimeClock.setTime(0,1,11,1,7,2,2018);

  Screen.setup();


  DBGLN(F("Init screen..."));
  Screen.setup();

  DBGLN(F("Add screen1...")); 

  Screen.addScreen(Screen1::create());           // первый экран покажется по умолчанию
  DBGLN(F("Add screen2..."));

  // добавляем второй экран
  Screen.addScreen(Screen2::create());
  DBGLN(F("Add screen3..."));
  // добавляем третий экран. Переход в меню настройки
  Screen.addScreen(Screen3::create());
  DBGLN(F("Add screen4..."));
  // добавляем четвертый экран. Меню установки даты и времени
  Screen.addScreen(Screen4::create());
  DBGLN(F("Add screen5..."));
  // добавляем 5 экран. Установка времени
  Screen.addScreen(Screen5::create());
  DBGLN(F("Add screen6..."));
  // добавляем 6 экран. Установка даты
  Screen.addScreen(Screen6::create());
  DBGLN(F("Add interrupt screen..."));
  // добавляем экран с графиком прерываний
  Screen.addScreen(InterruptScreen::create());

  // переключаемся на первый экран
  Screen.switchToScreen("Main");

  // поднимаем наши прерывания - РАСКОММЕНТИРОВАТЬ И ЗАКОММЕНТИРОВАТЬ ТЕСТОВЫЙ КОД НИЖЕ!!!
  InterruptHandler.begin();



  ////////////////////////////////////////////////////////////////////////////////
  //// ТЕСТОВЫЙ КОД ПРОВЕРКИ ГРАФИКОВ ПРЕРЫВАНИЙ
  ////////////////////////////////////////////////////////////////////////////////
  //InterruptTimeList it1;
  //it1.push_back(10);
  //it1.push_back(40);
  //it1.push_back(50);
  //it1.push_back(80);
  //it1.push_back(100);
  //it1.push_back(150);
  //it1.push_back(220);
  //it1.push_back(260);
  //it1.push_back(300);
  //it1.push_back(320);
  //it1.push_back(330);
  //it1.push_back(335);
  //it1.push_back(350);
  //it1.push_back(400);
  //it1.push_back(420);
  //it1.push_back(480);
  //it1.push_back(560);
  //it1.push_back(800);
  //it1.push_back(850);
  //it1.push_back(1000);
  //it1.push_back(1200);
  //it1.push_back(1250);
  //it1.push_back(1400);
  //it1.push_back(1440);
  //it1.push_back(1725);
  //it1.push_back(1800);
  //it1.push_back(2091);
  //it1.push_back(2222);
  //it1.push_back(2453);
  //it1.push_back(2921);

  //InterruptTimeList it2;
  //it2.push_back(30);
  //it2.push_back(200);
  //it2.push_back(300);
  //it2.push_back(350);
  //it2.push_back(380);


  //InterruptTimeList it3;
  //it3.push_back(5);
  //it3.push_back(15);
  //it3.push_back(48);
  //it3.push_back(60);
  //it3.push_back(120);
  //it3.push_back(232);
  //it3.push_back(500);

  //ScreenInterrupt->setList(it1,0);
  //ScreenInterrupt->setList(it2,1);
  //ScreenInterrupt->setList(it3,2);
  //ScreenInterrupt->showChart();
  //////////////////////////////////////////////////////////////////////////////
  // ЗАКОНЧЕН
  //////////////////////////////////////////////////////////////////////////////

  DBGLN(F("Inited."));

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  Screen.update();

  // обновляем прерывания
  InterruptHandler.update();

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

