#include "ConfigPin.h"
#include <Arduino.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ConfigPin::ConfigPin()
{
  
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RodPosition ConfigPin::getRodPosition()
{
  uint8_t rodPinState1 = digitalRead(ROD_POSITION_PIN1);
  uint8_t rodPinState2 = digitalRead(ROD_POSITION_PIN2);

  // если на обеих датчиках одинаковое состояние - поломка штанги
  if(rodPinState1 == rodPinState2)
    return rpBroken;

  // если сработал датчик на пине 11 и не сработал на пине 12 - штанга вверху
  if(ROD_POSITION_TRIGGERED == rodPinState1 && ROD_POSITION_TRIGGERED == !rodPinState2)
    return rpUp;

  // иначе - штанга внизу
  return rpDown;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ConfigPin::setI2CPriority(uint8_t priority)
{
  
  NVIC_DisableIRQ(WIRE_ISR_ID);
  NVIC_ClearPendingIRQ(WIRE_ISR_ID);
  NVIC_SetPriority(WIRE_ISR_ID, priority);
  NVIC_EnableIRQ(WIRE_ISR_ID);

#if defined (__arm__) && defined (__SAM3X8E__) 
  NVIC_DisableIRQ(WIRE1_ISR_ID);
  NVIC_ClearPendingIRQ(WIRE1_ISR_ID);
  NVIC_SetPriority(WIRE1_ISR_ID, priority);
  NVIC_EnableIRQ(WIRE1_ISR_ID);
#endif  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ConfigPin::setup()
{
  
	pinMode(ROD_POSITION_PIN1, INPUT);                  // Вход "Релейная защита N1"
	digitalWrite(ROD_POSITION_PIN1, INPUT_PULLUP);      // Вход "Релейная защита N1", подключить резисторы
  
	pinMode(ROD_POSITION_PIN2, INPUT);                  // Вход "Релейная защита N2"
	digitalWrite(ROD_POSITION_PIN2, INPUT_PULLUP);      // Вход "Релейная защита N2", подключить резисторы

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
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

