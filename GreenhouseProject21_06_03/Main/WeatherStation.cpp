#include "WeatherStation.h"
#include "LogicManageModule.h"
#include "EEPROMSettingsModule.h"
#include "AbstractModule.h"

// Библиотека iarduino_I2C_connect разработана для удобства соединения нескольких arduino по шине I2C
// Данная Arduino является ведущим устройством на шине I2C

// Подключаем библиотеки:
#include <Wire.h>                                     // подключаем библиотеку для работы с шиной I2C
#include <iarduino_I2C_connect.h>                     // подключаем библиотеку для соединения arduino по шине I2C
// Объявляем переменные и константы:
iarduino_I2C_connect I2C2;                            // объявляем переменную для работы c библиотекой iarduino_I2C_connect

byte stationID = 0;
byte crc = 0;
byte CRC_Calc = 0;
int tempI2C = 0;
byte humI2C = 0;
int windI2C = 0;
byte wind_dirI2C = 0;
byte rainI2C = 0;

float temp = 0;
unsigned long prevtime;
uint32_t  readTimeoutMisol = 1000 * 120;  // Отсутствие информации от станции в течении 2 минут

//--------------------------------------------------------------------------------------------------------------------------------------
WeatherStationClass WeatherStation;
//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t flag = 0;
uint8_t lastRainPulse = 0xFF;
//--------------------------------------------------------------------------------------------------------------------------------------
WeatherStationClass::WeatherStationClass()
{
 
}
//--------------------------------------------------------------------------------------------------------------------------------------
void WeatherStationClass::setup(int16_t _pin)
{

   Humidity = NO_TEMPERATURE_DATA;
   HumidityDecimal = 0;

   Temperature = NO_TEMPERATURE_DATA;
   TemperatureDecimal = 0;
   Wire.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void WeatherStationClass::update()
{
	
  flag = I2C2.readByte(0x01, 9);             // Считываем состояние флага наличия новых данных
	//   flag = 1;

  if (micros() - prevtime > readTimeoutMisol)
  {

	  Humidity = NO_TEMPERATURE_DATA;
	  HumidityDecimal = 0;

	  Temperature = NO_TEMPERATURE_DATA;
	  TemperatureDecimal = 0;

	  uint32_t windSpeed = 0;
	  // просим модуль логики сохранить данные по скорости ветра
	  LogicManageModule->SetWindSpeed(windSpeed);

	  CompassPoints windDirection = cpUnknown;
	  

#ifdef MISOL_DEBUG
	  DEBUG_LOG(F("Humidity = "));
	  DEBUG_LOGLN(Humidity);
	  DEBUG_LOG(F("Temperature = "));
	  DEBUG_LOGLN(Temperature);
#endif


  } // if

  if (flag == 1) //если были данные
  {

	prevtime = micros();
	stationID = I2C2.readByte(0x01, 0);        // Считываем ID станции
	crc = I2C2.readByte(0x01, 10);             // Считываем принятую контрольную сумму из пакета
	CRC_Calc = I2C2.readByte(0x01, 11);        // Считываем контрольную сумму расчитанную пакета

	tempI2C = I2C2.readByte(0x01, 1) << 8;     // Считываем старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1), сдвигаем полученный байт на 8 бит влево, т.к. он старший
	tempI2C += I2C2.readByte(0x01, 2);         // Считываем младший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 2), добавляя его значение к ранее полученному старшему байту

	humI2C = I2C2.readByte(0x01, 3);           // Считываем байт влажности

	windI2C = I2C2.readByte(0x01, 4) << 8;     // Считываем старший байт значения скорости ветра ведомого (адрес ведомого 0x01, номер регистра 4), сдвигаем полученный байт на 8 бит влево, т.к. он старший
	windI2C += I2C2.readByte(0x01, 5);         // Считываем младший байт значения скорости ветра ведомого (адрес ведомого 0x01, номер регистра 5), добавляя его значение к ранее полученному старшему байту

	wind_dirI2C = I2C2.readByte(0x01, 6);      // Считываем байт направления ветра
	rainI2C = I2C2.readByte(0x01, 7);          // Считываем байт счетчика импульсов коромысла дождя

	if (stationID == 0xF5 && crc == CRC_Calc)
	{ //первый байт должен быть 0xF5 (ID станции, может варьироваться от модели и верный CRC
	  //ID станции возможно необходимо настраивать под конкретную?

		bool tempValid = true;
		if (I2C2.readByte(0x01, 2) != 255)
		{
			temp = (float)(tempI2C) / 10.0;
		}
		else
		{
			tempValid = false;
		}

		if (tempValid) // валидная температура
		{
			int32_t iTemp = temp * 100;
			Temperature = iTemp / 100;
			TemperatureDecimal = abs(iTemp % 100);

		}
		else // невалидная температура
		{
			Temperature = NO_TEMPERATURE_DATA;
			TemperatureDecimal = 0;
		}

		//Влажность
		uint8_t hum = 0;
		uint8_t hum1 = 0;



		if (humI2C != 0xFF) // валидная влажность
		{
			Humidity = humI2C;
			HumidityDecimal = 0;
		}
		else // невалидная влажность
		{
			Humidity = NO_TEMPERATURE_DATA;
			HumidityDecimal = 0;
		}


		//скорость ветра
		uint16_t ch_wind = 0;
		uint16_t ch_wind1 = 0;
		float wind = 0;
		bool windValid = true;

		if (windI2C != 0xFF)
		{
			wind = (float)windI2C / 588;
		}
		else
		{
			windValid = false;
		}

		if (windValid) // если данные по скорости ветра валидны
		{
			// метеостанция передаёт нам скорость ветра в метрах в секунду
			// а у нас скорость ветра хранится в сотых долях м/с
			// поэтому - конвертируем значение в сотые доли
			uint32_t windSpeed = wind * 100;

			// просим модуль логики сохранить данные по скорости ветра
			LogicManageModule->SetWindSpeed(windSpeed);

		}// if(windValid)

		//=================направление ветра==============================

		if (wind_dirI2C != 0xFF) // если данные по направлению ветра валидны
		{
			CompassPoints windDirection = cpUnknown;
			switch (wind_dirI2C)
			{
			case 0: /*Serial.print("N")*/ windDirection = cpNorth; break;
			case 2: /*Serial.print("NE")*/ windDirection = cpNorth; break;
			case 4: /*Serial.print("E")*/ windDirection = cpEast; break;
			case 6: /*Serial.print("SE")*/ windDirection = cpEast; break;
			case 8: /*Serial.print("S")*/ windDirection = cpSouth; break;
			case 10: /*Serial.print("SW")*/ windDirection = cpSouth; break;
			case 12: /*Serial.print("W")*/ windDirection = cpWest; break;
			case 14: /*Serial.print("NW")*/ windDirection = cpWest; break;
			default: windDirection = cpUnknown; break;
			} // switch

			// просим модуль логики сохранить данные по направлению ветра
			LogicManageModule->SetWindDirection(windDirection);

		} // if(wind_dir != 0xFF)

		   //========== Дождь================================= 

		uint8_t rain = rainI2C;  //Передает состояние счетчика импульсов коромысла. Для определения наличия дождя нужно сравнивать предыдущие данные и текущие.

		if (rain != 0xFF) // если данные по дождю валидны
		{
			bool hasRain = false;
			if (lastRainPulse == 0xFF) // ещё не сохраняли последнее значение с метеостанции
			{
				lastRainPulse = rain; // сохраняем его
			}

			if (rain != lastRainPulse) // если значения не равны - идёт дождь
			{
				lastRainPulse = rain;
				hasRain = true;
			}
			// просим модуль логики сохранить флаг дождя
			LogicManageModule->SetHasRain(hasRain);
		} // if(rain != 0xFF)

#ifdef MISOL_DEBUG

	  Serial.print("\n");

	  Serial.print("Temperature: ");
	  Serial.print((temp), 1);
	  Serial.println("C");
	  Serial.print("Humidity: ");
	  Serial.print(hum);
	  Serial.println("%");
	  Serial.print("Wind: ");
	  Serial.print(wind);
	  Serial.println("m/s");
	  Serial.print("Direction: ");
	  switch (wind_dir)
	  {
	  case 0: Serial.print("N"); break;
	  case 2: Serial.print("NE"); break;
	  case 4: Serial.print("E"); break;
	  case 6: Serial.print("SE"); break;
	  case 8: Serial.print("S"); break;
	  case 10: Serial.print("SW"); break;
	  case 12: Serial.print("W"); break;
	  case 14: Serial.print("NW"); break;
	  default: Serial.print("Error"); break;
	  }

	  Serial.print("\n");

	  Serial.print("Rain pulse: ");  // счетчик дождя
	  Serial.println(rain);

	  Serial.print("\n\n");
#endif

	  flag = false;                            // Данные получены и зарегистрированы
      I2C2.writeByte(0x01, 9, flag);           // Отправляем состояние флага ведомому (адрес ведомого 0x01, номер регистра 9, состояние флага)
	} //if (stationID == 0xF5 && crc == calc_CRC())
  }  // if (flag == 1) //если были данные
}
//--------------------------------------------------------------------------------------------------------------------------------------

