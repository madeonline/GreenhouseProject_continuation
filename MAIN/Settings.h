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

    // работа с дельтами импульсов по каналам
    uint8_t getChannelDelta(uint8_t channelNum);
    void setChannelDelta(uint8_t channelNum, uint8_t val);

    // работа с моторесурсом системы
    uint32_t getMotoresource(uint8_t channelNum);
    void setMotoresource(uint8_t channelNum, uint32_t val);
    
    uint32_t getMotoresourceMax(uint8_t channelNum);
    void setMotoresourceMax(uint8_t channelNum, uint32_t val);

    DS3231Temperature getTemperature() { return coreTemp; }

    void set3V3RawVoltage(uint16_t raw);
    void set5VRawVoltage(uint16_t raw);
    void set200VRawVoltage(uint16_t raw);

    VoltageData get3V3Voltage() { return voltage3V3; }
    VoltageData get5Vvoltage() { return voltage5V; }
    VoltageData get200Vvoltage() {return voltage200V; }

    // состояние индуктивных датчиков
    uint8_t getInductiveSensorState(uint8_t channelNum);
    
  private:

    AT24C64* eeprom;
    DS3231Temperature coreTemp;
    uint32_t timer, inductiveSensorsTimer;

    void updateInductiveSensors();
    uint8_t inductiveSensorState1, inductiveSensorState2, inductiveSensorState3;

    VoltageData voltage3V3, voltage5V, voltage200V;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern SettingsClass Settings;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
