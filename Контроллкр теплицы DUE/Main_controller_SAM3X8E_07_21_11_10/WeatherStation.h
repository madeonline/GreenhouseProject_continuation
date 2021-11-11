#pragma once

#include <Arduino.h>
//--------------------------------------------------------------------------------------------------------------------------------------
class WeatherStationClass
{
  private:
      int16_t _ID_Misol;
	  int16_t _ID_Misol_WS0232 = 255;
	  int16_t _ID_Misol_WN5300CA = 255;
 	  int calc_REG_Array();
	  double getTemperature();
	  byte TempBitError;          //  Бит(-ы, почему-то 2) ошибки показаний температуры
	  unsigned long currentMillis = 0;

  public:
    WeatherStationClass();

    void setup_WS0232(int16_t _ID_Misol);
	void setup_WN5300CA(int16_t _ID_Misol);

    void update();

  int8_t Humidity;            // целое значение влажности
  uint8_t HumidityDecimal;    // значение влажности после запятой
  int8_t Temperature;         // целое значение температуры
  uint8_t TemperatureDecimal; // значение температуры после запятой
    

};
//--------------------------------------------------------------------------------------------------------------------------------------
extern WeatherStationClass WeatherStation;
