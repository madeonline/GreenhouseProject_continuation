//--------------------------------------------------------------------------------------------------
#include "InfoDiodes.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------
InfoDiodesList InfoDiodes;
//--------------------------------------------------------------------------------------------------
InfoDiodesList::InfoDiodesList()
{
  
}
//--------------------------------------------------------------------------------------------------
void InfoDiodesList::begin()
{
  pinMode(LED_READY, OUTPUT);
  pinMode(LED_FAILURE, OUTPUT);
  pinMode(LED_TEST, OUTPUT);

  ready(false);
  failure(false);
  test(false);
}
//--------------------------------------------------------------------------------------------------
void InfoDiodesList::ready(bool on)
{
  digitalWrite(LED_READY, on ? LED_ON_LEVEL : !LED_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void InfoDiodesList::failure(bool on)
{
  digitalWrite(LED_FAILURE, on ? LED_ON_LEVEL : !LED_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void InfoDiodesList::test(bool on)
{
  digitalWrite(LED_TEST, on ? LED_ON_LEVEL : !LED_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------

