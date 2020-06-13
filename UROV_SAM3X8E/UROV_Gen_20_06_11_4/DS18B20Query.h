#pragma once

#include <Arduino.h>
#include "TinyVector.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push,1)
typedef struct _ds_temp
{
  bool Negative;
  int Whole;
  int Fract;

  _ds_temp()
  {
    reset();
  }

  bool operator==(const _ds_temp& rhs)
  {
    return Negative == rhs.Negative && Whole == rhs.Whole && Fract == rhs.Fract;
  }

  bool operator !=(const _ds_temp& rhs)
  {
    return !operator==(rhs);
  }

  bool hasData() const
  {
    return (Whole != NO_TEMPERATURE_DATA);
  }

  void reset()
  {
    Negative = false;
    Whole = NO_TEMPERATURE_DATA;
    Fract = 0;     
  }

  operator String() const;
  
} DS18B20Temperature;
#pragma pack(pop)
//--------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  temp9bit = 0x1F,
  temp10bit = 0x3F,
  temp11bit = 0x5F,
  temp12bit = 0x7F
  
} DS18B20Resolution;
//--------------------------------------------------------------------------------------------------------------------------------------
// класс диспетчера линии DS18B20
//--------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push,1)
typedef struct
{
	uint8_t pin;
	uint8_t index;
	uint8_t address[8];
	
} DS18B20Binding;
#pragma pack(pop)
//--------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<DS18B20Binding> DS18B20BindingList;
//--------------------------------------------------------------------------------------------------------------------------------------
class DS18B20Dispatcher
{
	private:
		
		Vector<uint8_t> conversionList;
		bool alreadyConverted(uint8_t pin);

		Vector<uint8_t> resolutionList;
		bool alreadySetResolution(uint8_t pin);

		Vector<uint8_t> scanList;
		bool alreadyScanned(uint8_t pin);
		
		DS18B20BindingList scanResults;
		DS18B20BindingList savedBindings;
		
		int findBinding(uint8_t sensorIndex, uint8_t pin);
		bool inScanResults(uint8_t pin, const DS18B20Binding& binding);
		
		void resetBinding(int recordIndex);
		void saveBinding(int recordIndex);
		
		int findFree(uint8_t pin, int& scanResultsIndex);
		
	public:
		DS18B20Dispatcher();
		
		void begin();
		void addBinding(uint8_t pin, uint8_t sensorIndex);
		
		void beginConversion();
		void startConversion(uint8_t pin);
		
		void beginSetResolution();
		void setResolution(uint8_t pin,DS18B20Resolution res);
		
		void beginScan();
		void scan(uint8_t pin);
		
		bool getTemperature(uint8_t sensorIndex, uint8_t pin, DS18B20Temperature& result);
	
};
//--------------------------------------------------------------------------------------------------------------------------------------
extern DS18B20Dispatcher DS18B20LineManager;
//--------------------------------------------------------------------------------------------------------------------------------------
class TempSensorsClass
{
  public:
    TempSensorsClass();
    void begin();
    void update();

    size_t getSensorsCount();
    DS18B20Temperature getTemperature(size_t sensorIndex);

   private:

    uint32_t lastMillis;
    Vector<DS18B20Temperature> temperatures;
};
//--------------------------------------------------------------------------------------------------------------------------------------
extern TempSensorsClass TempSensors;

