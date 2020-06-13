#include "DueTimer.h"
#include "Buzzer.h"
//--------------------------------------------------------------------------------------------------------------------------------------
BuzzerClass Buzzer;
//--------------------------------------------------------------------------------------------------------------------------------------
BuzzerClass::BuzzerClass()
{
  active = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::begin()
{
  pinMode(BUZZER_PIN,OUTPUT);
  digitalWrite(BUZZER_PIN,!BUZZER_LEVEL);
  
  BUZZER_TIMER.attachInterrupt(buzzOffHandler);
  BUZZER_TIMER.setPeriod(1000ul*BUZZER_DURATION);

}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::stop()
{
  buzzLevel(false);
  BUZZER_TIMER.stop();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::buzz()
{
  if(active)
    return;

  active = true;
  buzzLevel(true);

  BUZZER_TIMER.start();
  //CoreDelayedEvent.raise(BUZZER_DURATION,buzzOffHandler,this);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::buzzLevel(bool on)
{
  digitalWrite(BUZZER_PIN, on ? BUZZER_LEVEL : !BUZZER_LEVEL);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::buzzOffHandler()//(void* param)
{
  BUZZER_TIMER.stop();
  Buzzer.buzzLevel(false);
  Buzzer.active = false;
  
  /*
  BuzzerClass* bc = (BuzzerClass*) param;
  bc->buzzLevel(false);
  bc->active = false;
  */
}
//--------------------------------------------------------------------------------------------------------------------------------------
