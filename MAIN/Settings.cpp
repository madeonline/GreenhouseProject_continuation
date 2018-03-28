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
  inductiveSensorsTimer = INDUCTIVE_SENSORS_UPDATE_INTERVAL;
  
  voltage3V3.raw = voltage5V.raw = voltage200V.raw = 0;
  voltage3V3.voltage = voltage5V.voltage = voltage200V.voltage = 0;

  inductiveSensorState1 = inductiveSensorState2 = inductiveSensorState3 = HIGH;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::getInductiveSensorState(uint8_t channelNum)
{
  switch(channelNum)
  {
    case 0:
      return inductiveSensorState1;
    
    case 1:
      return inductiveSensorState2;

    case 2:
      return inductiveSensorState3;
  }

  return LOW;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::begin()
{
  eeprom = new AT24C64();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::set3V3RawVoltage(uint16_t raw)
{
  voltage3V3.raw = raw;  
  voltage3V3.voltage = voltage3V3.raw*(2.4 / 4096 * 2);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::set5VRawVoltage(uint16_t raw)
{
  voltage5V.raw = raw; 
  voltage5V.voltage = voltage5V.raw*(2.4 / 4096 * 2);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::set200VRawVoltage(uint16_t raw)
{
  voltage200V.raw = raw; 
  voltage200V.voltage = voltage200V.raw*(2.4 / 4096 * 100);    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::update()
{
  uint32_t now = millis();
  
  if(now - timer > DATA_MEASURE_THRESHOLD)
  {
    timer = now;
    coreTemp = RealtimeClock.getTemperature();  
  }
  
  if(now - inductiveSensorsTimer > INDUCTIVE_SENSORS_UPDATE_INTERVAL)
  {
    timer = now;
    updateInductiveSensors();
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::updateInductiveSensors()
{
  inductiveSensorState1 = digitalRead(inductive_sensor1);
  inductiveSensorState2 = digitalRead(inductive_sensor2);
  inductiveSensorState3 = digitalRead(inductive_sensor3);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::getMotoresource(uint8_t channelNum)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = MOTORESOURCE_STORE_ADDRESS1;
    break;
    
    case 1:
    addr = MOTORESOURCE_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_STORE_ADDRESS3;
    break;
  }

  uint32_t result = 0;
  uint8_t* writePtr = (uint8_t*)&result;
  eeprom->read(addr,writePtr,sizeof(uint32_t));

  if(result == 0xFFFFFFFF)
  {
    result = 0;
    setMotoresource(channelNum,result);
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setMotoresource(uint8_t channelNum, uint32_t val)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = MOTORESOURCE_STORE_ADDRESS1;
    break;
    
    case 1:
    addr = MOTORESOURCE_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_STORE_ADDRESS3;
    break;
  }

    uint8_t* writePtr = (uint8_t*)&val;
    eeprom->write(addr,writePtr,sizeof(uint32_t));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::getMotoresourceMax(uint8_t channelNum)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS1;
    break;
    
    case 1:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS3;
    break;
  }

  uint32_t result = 0;
  uint8_t* writePtr = (uint8_t*)&result;
  eeprom->read(addr,writePtr,sizeof(uint32_t));

  if(result == 0xFFFFFFFF)
  {
    result = 0;
    setMotoresourceMax(channelNum,result);
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setMotoresourceMax(uint8_t channelNum, uint32_t val)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS1;
    break;
    
    case 1:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS3;
    break;
  }

    uint8_t* writePtr = (uint8_t*)&val;
    eeprom->write(addr,writePtr,sizeof(uint32_t));
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
uint8_t SettingsClass::getChannelDelta(uint8_t channelNum)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = CHANNEL_PULSES_DELTA_ADDRESS1;
    break;
    
    case 1:
    addr = CHANNEL_PULSES_DELTA_ADDRESS2;
    break;
    
    case 2:
    addr = CHANNEL_PULSES_DELTA_ADDRESS3;
    break;
  }

  uint8_t result = eeprom->read(addr);

  if(result == 0xFF)
  {
    result = 0;
    setChannelDelta(channelNum,result);
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setChannelDelta(uint8_t channelNum, uint8_t val)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
    case 0:
    addr = CHANNEL_PULSES_DELTA_ADDRESS1;
    break;
    
    case 1:
    addr = CHANNEL_PULSES_DELTA_ADDRESS2;
    break;
    
    case 2:
    addr = CHANNEL_PULSES_DELTA_ADDRESS3;
    break;
  }

  eeprom->write(addr,val);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

