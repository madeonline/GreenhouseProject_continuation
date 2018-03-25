#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "AT24CX.h"
#include "DS3231.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  int raw;
  float voltage;
  
} VoltageData;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SettingsClass
{
  public:

    SettingsClass();

    void begin();

    void update();

    // возвращает настройку кол-ва импульсов на канал
    uint16_t getChannelPulses(uint8_t channelNum);

    // сохраняет настройку кол-ва импульсов на канал
    void setChannelPulses(uint8_t channelNum, uint16_t val);

    // работа с моторесурсом системы
    uint32_t getMotoresource();
    void setMotoresource(uint32_t val);

    DS3231Temperature getTemperature() { return coreTemp; }

    VoltageData get3V3Voltage() { return voltage3V3; }
    VoltageData get5Vvoltage() { return voltage5V; }
    VoltageData get200Vvoltage() {return voltage200V; }
    
  private:

    AT24C64* eeprom;
    DS3231Temperature coreTemp;
    uint32_t timer;


    VoltageData voltage3V3, voltage5V, voltage200V;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern SettingsClass Settings;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
