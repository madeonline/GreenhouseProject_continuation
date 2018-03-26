//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Settings.h"
#include "CONFIG.h"
#include "ConfigPin.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SettingsClass Settings;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SettingsClass::SettingsClass()
{
  eeprom = NULL;
  timer = DATA_MEASURE_THRESHOLD;
  voltage3V3.raw = voltage5V.raw = voltage200V.raw = 0;
  voltage3V3.voltage = voltage5V.voltage = voltage200V.voltage = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::begin()
{
  eeprom = new AT24C64();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::update()
{
  uint32_t now = millis();
  if(now - timer < DATA_MEASURE_THRESHOLD)
    return;

  timer = now;
  coreTemp = RealtimeClock.getTemperature();

  //TODO: РАЗОБРАТЬСЯ С analogRead !!!!
  voltage3V3.raw = random(2800,2900); // analogRead(power_3V3);
  voltage5V.raw = random(3900,4000); // analogRead(power_5V0);
  voltage200V.raw = random(3500,3600); // analogRead(power_200);

  voltage3V3.voltage = voltage3V3.raw*(2.4 / 4096 * 2);
  voltage5V.voltage = voltage5V.raw*(2.4 / 4096 * 2);
  voltage200V.voltage = voltage200V.raw*(2.4 / 4096 * 100);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::getMotoresource()
{
  uint32_t result = 0;
  uint8_t* writePtr = (uint8_t*)&result;
  eeprom->read(MOTORESOURCE_STORE_ADDRESS,writePtr,sizeof(uint32_t));

  if(result == 0xFFFFFFFF)
  {
    result = 0;
    setMotoresource(result);
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setMotoresource(uint32_t val)
{
  uint8_t* writePtr = (uint8_t*)&val;
  eeprom->write(MOTORESOURCE_STORE_ADDRESS,writePtr,sizeof(uint32_t));  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::getChannelPulses(uint8_t channelNum)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = COUNT_PULSES_STORE_ADDRESS1;
    break;
    
    case 1:
    addr = COUNT_PULSES_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = COUNT_PULSES_STORE_ADDRESS3;
    break;
  }

  uint16_t result = 0;
  uint8_t* writePtr = (uint8_t*)&result;
  eeprom->read(addr,writePtr,sizeof(uint16_t));

  if(result == 0xFFFF)
  {
    result = 0;
    setChannelPulses(channelNum,result);
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setChannelPulses(uint8_t channelNum, uint16_t val)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = COUNT_PULSES_STORE_ADDRESS1;
    break;
    
    case 1:
    addr = COUNT_PULSES_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = COUNT_PULSES_STORE_ADDRESS3;
    break;
  }

    uint8_t* writePtr = (uint8_t*)&val;
    eeprom->write(addr,writePtr,sizeof(uint16_t));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

