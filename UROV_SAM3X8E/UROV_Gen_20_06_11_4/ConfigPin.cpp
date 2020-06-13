#include "ConfigPin.h"
#include "CONFIG.h"
#include <Arduino.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ConfigPin::ConfigPin()
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ConfigPin::setI2CPriority(uint8_t priority)
{

  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
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

	pinMode(rele_protect1, OUTPUT);                    // Реле RL0 формирование сингнала срабатывания защиты
	digitalWrite(rele_protect1, !RELAY_ON_LEVEL);                  // Реле RL0 формирование сингнала срабатывания защиты

	pinMode(rele_protect2, OUTPUT);                    // Реле RL7 формирование сингнала срабатывания защиты (резерв)
	digitalWrite(rele_protect2, !RELAY_ON_LEVEL);                  // Реле RL7 формирование сингнала срабатывания защиты (резерв)

	pinMode(enkoderA, OUTPUT);                         // формирователь сигналов энкодера линия А    
	digitalWrite(enkoderA, LOW);                       // формирователь сигналов энкодера линия А    
	pinMode(enkoderB, OUTPUT);                         // формирователь сигналов энкодера линия В 
	digitalWrite(enkoderB, LOW);                       // формирователь сигналов энкодера линия В 

	pinMode(strob1, INPUT);                            //  Вход строб1 сигнала (резерв)  
	digitalWrite(strob1, INPUT_PULLUP);                //  Вход строб2 сигнала (резерв)  

	pinMode(strob2, INPUT);                            //  Вход троб сигнала (резерв)  
	digitalWrite(strob2, INPUT_PULLUP);                //  Вход троб сигнала (резерв)  

	//pinMode(TFT_reset, OUTPUT);                        // Выход pin сброса TFT дисплея.  
	//digitalWrite(TFT_reset, HIGH);                     // Выход pin сброса TFT дисплея. 
	//delay(10);



}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

