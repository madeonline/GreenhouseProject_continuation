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
byte REG_Array1[15];                        // объявляем массив, данные которого будут доступны для чтения/записи по шине I2C

float temp = 0;
unsigned long prevtime = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
uint32_t  readTimeoutMisol = 120000;  // Отсутствие информации от станции в течении 1 минут

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
   prevtime = micros();
#ifdef MISOL_DEBUG
   Serial.println("Misol setup OK!");
#endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void WeatherStationClass::update()
{

	flag = I2C2.readByte(0x01, 14);             // Считываем состояние флага наличия новых данных
	  //   flag = 1;
	unsigned long currentMillis = millis();
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

		byte var = 0;

		while (var < 5)       // Пять попыток получить информацию с приемника Misol
		{
			for (int i = 0; i < 15; i++)
			{
				REG_Array1[i] = I2C2.readByte(0x01, i);        // Считываем пакет из приемника метеостанции
				delay(10);
			}
			crc = REG_Array1[12];                              // Считываем принятую контрольную сумму из пакета
			if (crc == calc_REG_Array())                       // Если данные верны - завершить попытки чтения данных
			{
               #ifdef MISOL_DEBUG
				Serial.print("var - ");
				Serial.println(var);
				#endif
				break;
			}
			var++;
			delay(50);
		}

		stationID = REG_Array1[0];              // Считываем ID станции
		crc = REG_Array1[12];                   // Считываем принятую контрольную сумму из пакета
		CRC_Calc = REG_Array1[13];              // Считываем контрольную сумму расчитанную пакета


		if (crc != calc_REG_Array())
		{
			for (int i = 0; i < 15; i++)
			{
				REG_Array1[i] = I2C2.readByte(0x01, i);        // Считываем пакет из приемника метеостанции
				delay(10);
			}
		}
        #ifdef MISOL_DEBUG
		Serial.print("crc - ");
		Serial.println(crc);

		Serial.print("calc_REG_Array - ");
		Serial.println(calc_REG_Array());
		#endif

		if (stationID == 0xF5 && crc == calc_REG_Array())
  		{ //первый байт должен быть 0xF5 (ID станции, может варьироваться от модели и верный CRC
  		  //ID станции возможно необходимо настраивать под конкретную?
   		    bool tempValid = true;
			byte tempI2C_temp1 = REG_Array1[4];         // Считываем старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1)
			byte tempI2C_temp2 = tempI2C_temp1 & 0x0F;  // удалить старшие биты
			tempI2C = tempI2C_temp2 << 8;               // сдвигаем полученный байт на 8 бит влево, т.к.он старший
			tempI2C += REG_Array1[5];

  			if (tempI2C!= 255)
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
   
			humI2C = REG_Array1[6];
  
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

		windI2C = REG_Array1[7] << 8;
		windI2C += REG_Array1[8];

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

		//rainI2C = REG_Array1[10] ;
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

			flag = false;                           // Данные получены и зарегистрированы
			I2C2.writeByte(0x01, 14, flag);         // Отправляем состояние флага ведомому (адрес ведомого 0x01, номер регистра 9, состояние флага)
		}
	}
}


int WeatherStationClass::calc_REG_Array()
{
	uint8_t tmp_byte = 0;
	uint16_t tmp_result = 0;
	if (stationID == 0xF5)
	{
		for (uint8_t j = 0; j < 12; j++)
		{
			tmp_byte = 0;
			tmp_byte = REG_Array1[j];
			tmp_result = tmp_result + tmp_byte;
		}
	}
	return (tmp_result & 0xFF);
}



//--------------------------------------------------------------------------------------------------------------------------------------

