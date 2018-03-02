#include "ConfigPin.h"
#include <Arduino.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ConfigPin::ConfigPin()
{
  
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ConfigPin::setup()
{
	pinMode(led_ready, OUTPUT);                         // Индикация светодиодом "Готов"
	digitalWrite(led_ready, LOW);                       // Индикация светодиодом "Готов"
	pinMode(led_failure, OUTPUT);                       // Индикация светодиодом "Авария"
	digitalWrite(led_failure, LOW);                     // Индикация светодиодом "Авария"
	pinMode(led_test, OUTPUT);                          // Индикация светодиодом "Тест"
	digitalWrite(led_test, LOW);                        // Индикация светодиодом "Тест"

	pinMode(button1, INPUT);                            // Кнопка №1
	digitalWrite(button1, INPUT_PULLUP);                // Кнопка №1  подключить резисторы
	pinMode(button2, INPUT);                            // Кнопка №2
	digitalWrite(button2, INPUT_PULLUP);                // Кнопка №2  подключить резисторы
	pinMode(button3, INPUT);                            // Кнопка №3
	digitalWrite(button3, INPUT_PULLUP);                // Кнопка №3  подключить резисторы
	pinMode(button4, INPUT);                            // Кнопка №4
	digitalWrite(button4, INPUT_PULLUP);                // Кнопка №4  подключить резисторы

	pinMode(relay_protection1, INPUT);                  // Вход "Релейная защита N1"
	digitalWrite(relay_protection1, INPUT_PULLUP);      // Вход "Релейная защита N1", подключить резисторы
	pinMode(relay_protection2, INPUT);                  // Вход "Релейная защита N2"
	digitalWrite(relay_protection2, INPUT_PULLUP);      // Вход "Релейная защита N2", подключить резисторы

	pinMode(inductive_sensor1, INPUT);                  // Вход индуктивного датчика №1
	digitalWrite(inductive_sensor1, INPUT_PULLUP);      // Вход индуктивного датчика №1, подключить резисторы
	pinMode(inductive_sensor2, INPUT);                  // Вход индуктивного датчика №2
	digitalWrite(inductive_sensor2, INPUT_PULLUP);      // Вход индуктивного датчика №2, подключить резисторы
	pinMode(inductive_sensor3, INPUT);                  // Вход индуктивного датчика №3
	digitalWrite(inductive_sensor3, INPUT_PULLUP);      // Вход индуктивного датчика №3, подключить резисторы

	pinMode(out_asu_tp1, OUTPUT);                       // Выход на АСУ ТП №1
	digitalWrite(out_asu_tp1, LOW);                     // Выход на АСУ ТП №1
	pinMode(out_asu_tp2, OUTPUT);                       // Выход на АСУ ТП №2
	digitalWrite(out_asu_tp2, LOW);                     // Выход на АСУ ТП №2
	pinMode(out_asu_tp3, OUTPUT);                       // Выход на АСУ ТП №3
	digitalWrite(out_asu_tp3, LOW);                     // Выход на АСУ ТП №3
	pinMode(out_asu_tp4, OUTPUT);                       // Выход на АСУ ТП №4
	digitalWrite(out_asu_tp4, LOW);                     // Выход на АСУ ТП №4


	pinMode(Upr_RS485, OUTPUT);                         // Выход управлениея шиной RS485  
	digitalWrite(Upr_RS485, LOW);                       // Выход управлениея шиной RS485  

														// CAN шина
	pinMode(ID0_Out, OUTPUT);                           // Выход управлениея CAN шиной   
	digitalWrite(ID0_Out, LOW);                         // Выход управлениея CAN шиной   
	pinMode(ID1_Out, OUTPUT);                           // Выход управлениея CAN шиной   
	digitalWrite(ID1_Out, LOW);                         // Выход управлениея CAN шиной   

	pinMode(IDE0_In, INPUT);                            // Вход управлениея CAN шиной   
	digitalWrite(IDE0_In, INPUT_PULLUP);                // Вход управлениея CAN шиной, подключить резисторы
	pinMode(IDE1_In, INPUT);                            // Вход управлениея CAN шиной   
	digitalWrite(IDE1_In, INPUT_PULLUP);                // Вход управлениея CAN шиной, подключить резисторы

	pinMode(PPS_Out, OUTPUT);                           // Выход управлениея CAN шиной   
	digitalWrite(PPS_Out, LOW);                         // Выход управлениея CAN шиной   

	pinMode(TFT_reset, OUTPUT);                         // Выход pin сброса TFT дисплея.  
	digitalWrite(TFT_reset, HIGH);                      // Выход pin сброса TFT дисплея. 
	delay(10);
	digitalWrite(TFT_reset, HIGH);                      // Сброс TFT дисплея. 
	delay(10);
	digitalWrite(TFT_reset, HIGH);                      // Выход pin сброса TFT дисплея. 


}
