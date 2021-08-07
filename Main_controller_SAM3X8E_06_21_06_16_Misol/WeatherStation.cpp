#include "WeatherStation.h"
#include "LogicManageModule.h"
#include "EEPROMSettingsModule.h"
#include "AbstractModule.h"

// Библиотека iarduino_I2C_connect разработана для удобства соединения нескольких arduino по шине I2C
// Данная Arduino является ведущим устройством на шине I2C

// Подключаем библиотеки:
#include <Wire.h>                     // подключаем библиотеку для работы с шиной I2C
#include <iarduino_I2C_connect.h>     // подключаем библиотеку для соединения arduino по шине I2C

// Объявляем переменные и константы:
iarduino_I2C_connect I2C2;            // объявляем переменную для работы c библиотекой iarduino_I2C_connect

byte stationID = 0;           // ID станции
byte crc = 0;                 // Принятая контрольная сумма пакета
byte CRC_Calc = 0;            // Расчетная контрольная суммв пакета
int tempI2C = 0;              // Температура воздуха
byte humI2C = 0;              // Влажность воздуха
int windI2C = 0;              // Скорость ветра
int gust_wind = 0;            // Порыв ветра             
byte wind_dirI2C = 0;         // Направление ветра
byte rainI2C = 0;             // Счетчик дождя
byte REG_Array1[16];          // объявляем массив, данные которого будут доступны для чтения/записи по шине I2C

/*
Пакет MISOL WS0232
REG_Array1[0];    // ID станции   
REG_Array1[1];    // 
REG_Array1[2];    //
REG_Array1[3];    //
REG_Array1[4];    // Старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1) + направление ветра
REG_Array1[5];    // Младший байт значения температуры
REG_Array1[6];    // Влажность воздуха
REG_Array1[7];    // Старший байт скорости ветра
REG_Array1[8];    // Младший байт скорости ветра
REG_Array1[9];    // 
REG_Array1[10];   //
REG_Array1[11];   // Передает состояние счетчика импульсов коромысла.
REG_Array1[12];   // принятую контрольную сумму из пакета
REG_Array1[13];   // контрольную сумму расчитанную пакета
REG_Array1[14];   // Отправляем состояние флага ведомому (адрес ведомого 0x01, номер регистра 14, состояние флага)

//==========================================================================================================
Пакет MISOL WN5300CA

REG_Array1[0] = SecurityCode;      // ID метеостанции. Код безопасности
REG_Array1[1] = TempBitError;      // ошибки показаний температуры
REG_Array1[2] = Temperatue >> 8;   // Старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1) + направление ветра
REG_Array1[3] = Temperatue;        // Младший байт значения температуры
REG_Array1[4] = Humidity;          // Влажность воздуха
REG_Array1[5] = WindSpeed;         // Скорость ветра
REG_Array1[6] = Gust;              // Порывы ветра
REG_Array1[7] = RainCounter >> 8;  // Старший байт счётчик осадков
REG_Array1[8] = RainCounter;       // Младший байт счётчик осадков
REG_Array1[9] = WindDirBitError;   // Бит ошибки показаний направления ветра
REG_Array1[10] = LowBattBit;       // Бит разряженной батареи передатчика
REG_Array1[11] = WindDir;          // Направление ветра
REG_Array1[12] = calc_REG_Array(); // Записать расчетную контрольную сумму в массив
REG_Array1[13] = 0;                //
REG_Array1[14] = true;             // Записать подтверждение готовности пакета к передаче


*/

float temp = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
uint32_t  readTimeoutMisol = 120000;     // Отсутствие информации от станции в течении 2 минут

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
void WeatherStationClass::setup_WS0232(int16_t _ID_Misol)
{
	if (_ID_Misol != 255) 
	{
		_ID_Misol_WS0232 = _ID_Misol;
	}


   Humidity = NO_TEMPERATURE_DATA;
   HumidityDecimal = 0;

   Temperature = NO_TEMPERATURE_DATA;
   TemperatureDecimal = 0;
   Wire.begin();
 
   currentMillis = millis();

   #ifdef MISOL_DEBUG
	   Serial.println("Misol WS0232 setup OK!");
	   Serial.print("Misol ID -");
	   Serial.println(_ID_Misol_WS0232,HEX);
   #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void WeatherStationClass::setup_WN5300CA(int16_t _ID_Misol)
{
	if (_ID_Misol != 255)
	{
		_ID_Misol_WN5300CA = _ID_Misol;
	}


	Humidity = NO_TEMPERATURE_DATA;
	HumidityDecimal = 0;

	Temperature = NO_TEMPERATURE_DATA;
	TemperatureDecimal = 0;
	Wire.begin();

	currentMillis = millis();

    #ifdef MISOL_DEBUG
		Serial.println("Misol WN5300CA setup OK!");
		Serial.print("Misol ID -");
		Serial.println(_ID_Misol_WN5300CA, HEX);
    #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void WeatherStationClass::update()
{

	flag = I2C2.readByte(0x01, 14);             // Считываем состояние флага наличия новых данных

	currentMillis = millis();
	if (currentMillis - previousMillis >= readTimeoutMisol) // контролируем наличие данных с метеостанции в течении 2 минут.
	{
		previousMillis = currentMillis;
		Humidity = NO_TEMPERATURE_DATA;
		HumidityDecimal = 0;

		Temperature = NO_TEMPERATURE_DATA;
		TemperatureDecimal = 0;

		uint32_t windSpeed = 0;
		// просим модуль логики сохранить данные по скорости ветра
		LogicManageModule->SetWindSpeed(windSpeed);

		CompassPoints windDirection = cpUnknown;


     #ifdef MISOL_DEBUG
		Serial.print(F("Humidity = "));
		Serial.println(Humidity);
		Serial.print(F("Temperature = "));
		Serial.println(Temperature);
      #endif


	} // if


	if (flag == 1) //если были данные
	{
		previousMillis = currentMillis;
		flag = false;                           // Готов к расшифровке пакета. Повторная проверка готовности пакета не требуется
		I2C2.writeByte(0x01, 14, flag);         // Отправляем состояние флага ведомому (адрес ведомого 0x01, номер регистра 9, состояние флага)
		byte var = 0;

		while (var < 5)       // Пять попыток получить информацию с приемника Misol
		{
			for (int i = 0; i < 14; i++)
			{
				REG_Array1[i] = I2C2.readByte(0x01, i);        // Считываем пакет из приемника метеостанции
				delay(10);

			}
			crc = REG_Array1[12];                              // Считываем принятую контрольную сумму из пакета
			if (crc == calc_REG_Array())                      // Если данные верны - завершить попытки чтения данных
			{
				#ifdef MISOL_DEBUG
					Serial.print("count receive - ");
					Serial.println(var);

					Serial.print("crc1 - ");
					Serial.println(crc);
					Serial.print("calc_REG_Array - ");
					Serial.println(calc_REG_Array());
					Serial.print("Misol WS0232 ID -");
					Serial.println(_ID_Misol_WS0232, HEX);
					Serial.print("Misol WN5300CA ID -");
					Serial.println(_ID_Misol_WN5300CA, HEX);
				#endif
					stationID = REG_Array1[0];              // Считываем ID станции
				break;
			}
			var++;
			delay(50);
		}


		if (stationID == _ID_Misol_WS0232 && crc == calc_REG_Array())  // Принят пакет от станции MISOL WS0232
		{ //первый байт должен быть 0xF5 (ID станции, может варьироваться от модели и верный CRC
			//ID станции возможно необходимо настраивать под конкретную?
			bool tempValid = true;
			byte tempI2C_temp1 = REG_Array1[4];         // Считываем старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1)
			byte tempI2C_temp2 = tempI2C_temp1 & 0x0F;  // удалить старшие биты
			tempI2C = tempI2C_temp2 << 8;               // сдвигаем полученный байт на 8 бит влево, т.к.он старший
			tempI2C += REG_Array1[5];                   // Младший байт значения температуры

			if (tempI2C != 255)
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

			humI2C = REG_Array1[6];  // Влажность воздуха

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

			windI2C = REG_Array1[7] << 8;  // Старший байт скорости ветра
			windI2C += REG_Array1[8];      // Младший байт скорости ветра

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

			byte wind_dirI2C_temp1 = REG_Array1[4];
			byte wind_dirI2C_temp2 = wind_dirI2C_temp1 >> 4;
			wind_dirI2C = wind_dirI2C_temp2 & 0x0F;

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

	
			uint8_t rain = REG_Array1[11];  //Передает состояние счетчика импульсов коромысла. Для определения наличия дождя нужно сравнивать предыдущие данные и текущие.


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
				Serial.print(Humidity);
				Serial.println("%");
				Serial.print("Wind: ");
				Serial.print(wind);
				Serial.println("m/s");
				Serial.print("Direction: ");

				switch (wind_dirI2C)
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

			for (int i = 0; i < 15; i++)
			{
				REG_Array1[i] = 0;        // Очистить пакет из приемника метеостанции
			}
		}
		if (stationID == _ID_Misol_WN5300CA && crc == calc_REG_Array())   // Принят пакет от станции MISOL WN5300CA
		{ //первый байт должен быть 0x6D (ID станции, может варьироваться от модели и верный CRC
			//ID станции возможно необходимо настраивать под конкретную?
			bool tempValid = true;

			TempBitError = REG_Array1[1];       // ошибки показаний температуры
			tempI2C = REG_Array1[2]<<8;         // Считываем старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1)
			tempI2C += REG_Array1[3];           // Младший байт значения температуры

			// Функция получения температуры в гр. С
	
			if (TempBitError == false)
			{
				temp = ((float)(tempI2C)-400)/10.0;
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

			humI2C = REG_Array1[4];  // Влажность воздуха

			if (humI2C != 0xFF)       // валидная влажность
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

			float wind = 0;
			bool windValid = true;

			gust_wind = REG_Array1[6] ;     //  байт порыва ветра
			windI2C   = REG_Array1[5];      //  байт скорости ветра

			if (windI2C != 0xFF)
			{
				wind = (float)windI2C*0.34;
			}
			else
			{
				windValid = false;
			}

			if (windValid) // если данные по скорости ветра валидны
			{
				// метеостанция передаёт нам скорость ветра в километрах в час
				// а у нас скорость ветра хранится в сотых долях м/с
				// поэтому - конвертируем значение в сотые доли
				uint32_t windSpeed = wind * 100;

				// просим модуль логики сохранить данные по скорости ветра
				LogicManageModule->SetWindSpeed(windSpeed);

			}// if(windValid)

			//=================направление ветра==============================

			  wind_dirI2C = REG_Array1[11];


			      // case 0x0: return "N";    break;
				   //case 0x1: return "NNE";  break;
				   //case 0x2: return "NE";   break;
				   //case 0x3: return "NEE";  break;
				   //case 0x4: return "E";    break;
				   //case 0x5: return "EES";  break;
				   //case 0x6: return "ES";   break;
				   //case 0x7: return "ESS";  break;
				   //case 0x8: return "S";    break;
				   //case 0x9: return "SSW";  break;
				   //case 0xA: return "SW";   break;
				   //case 0xB: return "SWW";  break;
				   //case 0xC: return "W";    break;
				   //case 0xD: return "WWN";  break;
				   //case 0xE: return "WN";   break;
				   //case 0xF: return "WNN";  break;


			if (wind_dirI2C != 0xFF) // если данные по направлению ветра валидны
			{
				CompassPoints windDirection = cpUnknown;
				switch (wind_dirI2C)
				{
				case 0: /*Serial.print("N")*/ windDirection = cpNorth; break;
				case 1: /*Serial.print("NNE")*/ windDirection = cpNorth; break;
				case 2: /*Serial.print("NE")*/ windDirection = cpNorth; break;
				case 3: /*Serial.print("NEE")*/ windDirection = cpNorth; break;

				case 4: /*Serial.print("E")*/ windDirection = cpEast; break;
				case 5: /*Serial.print("SEE")*/ windDirection = cpEast; break;
				case 6: /*Serial.print("ES")*/ windDirection = cpEast; break;
				case 7: /*Serial.print("SSE")*/ windDirection = cpEast; break;

				case 8: /*Serial.print("S")*/ windDirection = cpSouth; break;
				case 9: /*Serial.print("SSW")*/ windDirection = cpSouth; break;
				case 10: /*Serial.print("SW")*/ windDirection = cpSouth; break;
				case 11: /*Serial.print("SWW")*/ windDirection = cpSouth; break;

				case 12: /*Serial.print("W")*/ windDirection = cpWest; break;
				case 13: /*Serial.print("NWW")*/ windDirection = cpWest; break;
				case 14: /*Serial.print("WN")*/ windDirection = cpWest; break;
				case 15: /*Serial.print("NWN")*/ windDirection = cpWest; break;

				default: windDirection = cpUnknown; break;
				} // switch

				// просим модуль логики сохранить данные по направлению ветра
				LogicManageModule->SetWindDirection(windDirection);

			} // if(wind_dir != 0xFF)

				//========== Дождь================================= 

				uint16_t rain = REG_Array1[7] << 8;  // Старший байт. Передает состояние счетчика импульсов коромысла. Для определения наличия дождя нужно сравнивать предыдущие данные и текущие.
			             rain = REG_Array1[8];       // Младший байт. Передает состояние счетчика импульсов коромысла. Для определения наличия дождя нужно сравнивать предыдущие данные и текущие.
					     rain = rain * 0.3; // 

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
			Serial.print(Humidity);
			Serial.println("%");
			Serial.print("Wind: ");
			Serial.print(wind);
			Serial.println("m/s");
			Serial.print("Direction: ");

			switch (wind_dirI2C)
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

			for (int i = 0; i < 15; i++)
			{
				REG_Array1[i] = 0;        // Очистить пакет из приемника метеостанции
			}
		}
  	}
}


int WeatherStationClass::calc_REG_Array()
{
	uint8_t tmp_byte = 0;
	uint16_t tmp_result = 0;
		for (uint8_t j = 0; j < 12; j++)
		{
			tmp_byte = 0;
			tmp_byte = REG_Array1[j];
			tmp_result = tmp_result + tmp_byte;
		}
	return (tmp_result & 0xFF);
}

double WeatherStationClass::getTemperature()
{
	if (TempBitError == 0)
	{
	
		return ((double)tempI2C - 400) / 10.0;
		//return ((double)Temperatue_WN5300CA - 400) / 10.0;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------

