//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Settings.h"
#include "CONFIG.h"
#include "ConfigPin.h"
#include "ADCSampler.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SettingsClass Settings;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SettingsClass::SettingsClass()
{
  eeprom = NULL;
  timer = DATA_MEASURE_THRESHOLD;
  
  voltage3V3.raw = voltage5V.raw = voltage200V.raw = 0;
  voltage3V3.voltage = voltage5V.voltage = voltage200V.voltage = 0;

  //DEPRECATED: inductiveSensorState1 = inductiveSensorState2 = inductiveSensorState3 = 1;

  relayDelay = RELAY_WANT_DATA_AFTER;
  acsDelay = ACS_SIGNAL_DELAY;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int8_t SettingsClass::getTemperatureAlertBorder()
{
  return TEMPERATURE_ALERT_BORDER; // при превышении этой уставки температуры будут выключаться РЕЛЕ ШУНТОВ
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::read(int addr)
{
  return eeprom->read(addr);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::write(int addr, uint8_t val)
{
  eeprom->write(addr,val); 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

String SettingsClass::getUUID(const char* passedUUID)
{
    String savedUUID;
    uint16_t addr = UUID_STORE_ADDRESS;
    uint8_t header1 = eeprom->read(addr); addr++;
    uint8_t header2 = eeprom->read(addr); addr++;
    uint8_t header3 = eeprom->read(addr); addr++;

    if(!(header1 == RECORD_HEADER1 && header2 == RECORD_HEADER2 && header3 == RECORD_HEADER3))
    {
      savedUUID = passedUUID;

      addr = UUID_STORE_ADDRESS;
      eeprom->write(addr,RECORD_HEADER1); addr++;
      eeprom->write(addr,RECORD_HEADER2); addr++;
      eeprom->write(addr,RECORD_HEADER3); addr++;

      uint8_t written = 0;
      for(size_t i=0;i<savedUUID.length();i++)
      {
        eeprom->write(addr,savedUUID[i]); 
        addr++;
        written++;
      }

      for(int i=written;i<32;i++)
      {
         eeprom->write(addr,'\0'); 
         addr++;
      }

      return savedUUID;
    }

    // есть сохранённый GUID, читаем его
    for(int i=0;i<32;i++)
    {
      savedUUID += (char) eeprom->read(addr); addr++;
    }

    return savedUUID;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
//DEPRECATED:
void SettingsClass::setInductiveSensorState(uint8_t channelNum, uint8_t val)
{
  switch(channelNum)
  {
    case 0:
      inductiveSensorState1 = val ? 1 : 0;
    
    case 1:
      inductiveSensorState2 = val ? 1 : 0;

    case 2:
      inductiveSensorState3 = val ? 1 : 0;
  }  
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
*/
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::begin()
{
  eeprom = new AT24C64();

  uint8_t* writePtr = (uint8_t*)&relayDelay;
  eeprom->read(RELAY_DELAY_STORE_ADDRESS,writePtr,sizeof(uint32_t));
  
  if(relayDelay == 0xFFFFFFFF)
    relayDelay = RELAY_WANT_DATA_AFTER;

  writePtr = (uint8_t*)&acsDelay;
  eeprom->read(ACS_DELAY_STORE_ADDRESS,writePtr,sizeof(uint16_t));
  
  if(acsDelay == 0xFFFF)
    acsDelay = ACS_SIGNAL_DELAY;    
  
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
#ifndef _CORE_TEMP_OFF

  uint32_t now = millis();
  
  if(now - timer > DATA_MEASURE_THRESHOLD)
  {
    timer = now;
    coreTemp = RealtimeClock.getTemperature();  
  }

#endif // !_CORE_TEMP_OFF  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::getRelayDelay()
{
  return relayDelay;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setRelayDelay(uint32_t val)
{
  relayDelay = val;
  uint8_t* writePtr = (uint8_t*)&val;
  eeprom->write(RELAY_DELAY_STORE_ADDRESS,writePtr,sizeof(uint32_t)); 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::getACSDelay()
{
  return acsDelay;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setACSDelay(uint16_t val)
{
  acsDelay = val;
  uint8_t* writePtr = (uint8_t*)&val;
  eeprom->write(ACS_DELAY_STORE_ADDRESS,writePtr,sizeof(uint16_t)); 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::getTransformerLowBorder()
{
  uint32_t result = 0;
  uint8_t* writePtr = (uint8_t*)&result;
  eeprom->read(TRANSFORMER_LOW_BORDER_STORE_ADDRESS,writePtr,sizeof(uint32_t));
  
  if(result == 0xFFFFFFFF)
    result = TRANSFORMER_LOW_DEFAULT_BORDER;

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::getTransformerHighBorder()
{
  uint32_t result = 0;
  uint8_t* writePtr = (uint8_t*)&result;
  eeprom->read(TRANSFORMER_HIGH_BORDER_STORE_ADDRESS,writePtr,sizeof(uint32_t));
  
  if(result == 0xFFFFFFFF)
    result = TRANSFORMER_HIGH_DEFAULT_BORDER;

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setTransformerLowBorder(uint32_t val)
{
  uint8_t* writePtr = (uint8_t*)&val;
  eeprom->write(TRANSFORMER_LOW_BORDER_STORE_ADDRESS,writePtr,sizeof(uint32_t)); 

   adcSampler.setLowBorder(val);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setTransformerHighBorder(uint32_t val)
{
  uint8_t* writePtr = (uint8_t*)&val;
  eeprom->write(TRANSFORMER_HIGH_BORDER_STORE_ADDRESS,writePtr,sizeof(uint32_t));

   adcSampler.setHighBorder(val);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::getMotoresource(uint8_t channelNum)
{
  uint16_t addr = 0;
  switch(channelNum)
  {
	  //DEPRECATED: case 0:
	default:
		addr = MOTORESOURCE_STORE_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = MOTORESOURCE_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_STORE_ADDRESS3;
    break;
	*/
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
	  //DEPRECATED: case 0:
	default:
		addr = MOTORESOURCE_STORE_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = MOTORESOURCE_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_STORE_ADDRESS3;
    break;
	*/
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
	  //DEPRECATED: case 0:
	default:
		addr = MOTORESOURCE_MAX_STORE_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS3;
    break;
	*/
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
	  //DEPRECATED: case 0:
	default:
		addr = MOTORESOURCE_MAX_STORE_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = MOTORESOURCE_MAX_STORE_ADDRESS3;
    break;
	*/
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
	  //DEPRECATED: case 0:
	default:
		addr = COUNT_PULSES_STORE_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = COUNT_PULSES_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = COUNT_PULSES_STORE_ADDRESS3;
    break;
	*/
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
	  //DEPRECATED: case 0:
	default:
		addr = COUNT_PULSES_STORE_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = COUNT_PULSES_STORE_ADDRESS2;
    break;
    
    case 2:
    addr = COUNT_PULSES_STORE_ADDRESS3;
    break;
	*/
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
	  //DEPRECATED: case 0:
	default:
		addr = CHANNEL_PULSES_DELTA_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = CHANNEL_PULSES_DELTA_ADDRESS2;
    break;
    
    case 2:
    addr = CHANNEL_PULSES_DELTA_ADDRESS3;
    break;
	*/
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
	  //DEPRECATED: case 0:
	default:
		addr = CHANNEL_PULSES_DELTA_ADDRESS1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
    addr = CHANNEL_PULSES_DELTA_ADDRESS2;
    break;
    
    case 2:
    addr = CHANNEL_PULSES_DELTA_ADDRESS3;
    break;
	*/
  }

  eeprom->write(addr,val);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

