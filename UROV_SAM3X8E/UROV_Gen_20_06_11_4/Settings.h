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
typedef enum
{
	rpBroken, // штанга поломана или находится в промежуточной позиции
	rpUp, // в верхней позиции
	rpDown // в нижней позиции
} RodDirection; // позиция штанги				
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SettingsClass
{
public:

	SettingsClass();

	void begin();

	void update();

  int8_t getTemperatureAlertBorder();

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
	VoltageData get200Vvoltage() { return voltage200V; }

	// состояние индуктивных датчиков
	//DEPRECATED: uint8_t getInductiveSensorState(uint8_t channelNum);
	//DEPRECATED: void setInductiveSensorState(uint8_t channelNum, uint8_t val);

	String getUUID(const char* passedUUID);

	uint32_t getTransformerLowBorder();
	void setTransformerLowBorder(uint32_t val);

	uint32_t getTransformerHighBorder();
	void setTransformerHighBorder(uint32_t val);

	uint32_t getRelayDelay();
	void setRelayDelay(uint32_t rDelay);

	uint16_t getACSDelay();
	void setACSDelay(uint16_t rDelay);

	RodDirection getRodDirection() { return rodDirection; }
	void setRodDirection(RodDirection val) { rodDirection = val; }


  uint8_t read(int addr);
  void write(int addr, uint8_t val);
    
  private:

	  RodDirection rodDirection;

    AT24C64* eeprom;
    DS3231Temperature coreTemp;
    uint32_t timer;

	//DEPRECATED: uint8_t inductiveSensorState1, inductiveSensorState2, inductiveSensorState3;

    VoltageData voltage3V3, voltage5V, voltage200V;

    uint32_t relayDelay;
    uint16_t acsDelay;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern SettingsClass Settings;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
