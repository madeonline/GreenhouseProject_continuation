
//----------------------------------------------------------------------------------------------------------------
#include "OneWireHub.h"
#include "DS18B20.h"  // Digital Thermometer, 12bit
#include <PWM.h>
#include <EEPROM.h>
#include "DS2433.h"

// start reading from the first byte (address 0) of the EEPROM

String inString = "";                       // string to hold input
int val1 = 0;
int val2 = 0;

bool calibration = false;                    // признак выполнения программы калибровки
byte calibration_OK = 0;                     // признак выполненной калибровки датчика
int address_calibr  = 0;                     // адрес признака калибровки датчика 
int address_SOIL_MOISTURE_0_PERCENT = 2;     // адрес значения величины АЦП для 0% влажности почвы (0-1023)
int address_SOIL_MOISTURE_100_PERCENT = 4;   // адрес значения величины АЦП для 100% влажности почвы (0-1023)

int SOIL_MOISTURE_0_PERCENT = 680;           // значение АЦП для 0% влажности почвы (0-1023)
int SOIL_MOISTURE_100_PERCENT = 650;         // значение АЦП для 100% влажности почвы (0-1023)




// ----------------------------------------------------------------------------------------------------------------
/*
 ПРОШИВКА ДЛЯ ОПРОСА ЕМКОСТНОГО ДАТЧИКА ВЛАЖНОСТИ ПОЧВЫ, ПРИНИМАЮЩЕГО НА ВХОД СИГНАЛ PWM, И
 ВЫДАЮЩИЙ ЗНАЧЕНИЕ ВЛАЖНОСТИ ПОЧВЫ.
 
 ПРОШИВКА ВЫДАЁТ НАРУЖУ ПОКАЗАНИЯ, ПРИКИДЫВАЯСЬ ДАТЧИКОМ DS18B20. ДЛЯ НАСТРОЙКИ ТАКОГО МОДУЛЯ
 НЕОБХОДИМО ПОДКЛЮЧИТЬ ЕГО К ЛИНИИ 1-WIRE КОНТРОЛЛЕРА, ЗАЙТИ В ОКНО "ПРИВЯЗКИ ЖЕЛЕЗА" КОНФИГУРАТОРА,
 И НА СТРАНИЦЕ "ЭМУЛЯТОРЫ DS18B20" УКАЗАТЬ НОМЕР ПИНА, К КОТОРОМУ ПОДКЛЮЧЕН МОДУЛЬ, УКАЗАТЬ, ЧТО ЭТО
 ДАТЧИК ВЛАЖНОСТИ ПОЧВЫ, И УКАЗАТЬ РЕЗУЛЬТИРУЮЩИЙ ИНДЕКС ДАТЧИКА (НАПРИМЕР, 0 - ДЛЯ ПЕРВОГО ТАКОГО ДАТЧИКА НА ЛИНИИ,
 1 - ДЛЯ ВТОРОГО И Т.Д.)
 
 Пины, которые использует плата модуля с датчиками:

  D2 - линия регистрации модуля в системе (1-Wire)
  D3 - линия генерации частоты емкостного сенсора
  A0 - чтение показаний с аналогового датчика влажности
 */

//----------------------------------------------------------------------------------------------------------------
/*
 Используемые типы датчиков, известные на линии 1-Wire. Если к типу нет комментария - это значение лучше не использовать в эмуляторе!!!
    0x01
    0x05
    0x09
    0x0F
    0x10  - DS18S20, температурный датчик, 9 байт скратчпад с данными
    0x1D
    0x20
    0x22 - DS1822, температурный датчик, 9 байт скратчпад с данными
    0x23
    0x26
    0x28 - DS18B20, температурный датчик, 9 байт скратчпад с данными
    0x29
    0x2C
    0x2D
    0x3A
    0xA0 - датчик влажности почвы, 9 байт скратчпад с данными точно такой же, как у DS18B20. Используется эмуляция DS18B20
    0xA1 - датчик влажности и температуры, 9 байт скратчпад с данными точно такой же, как у DS18B20. Используется эмуляция DS18B20
    0xA2 - датчик освещённости, скратчпад, 9 байт скратчпад с данными точно такой же, как у DS18B20. Используется эмуляция DS18B20
    0xFC
 */
//----------------------------------------------------------------------------------------------------------------
// НАСТРОЙКИ ПРОШИВКИ
//----------------------------------------------------------------------------------------------------------------
#define _DEBUG // раскомментировать для отладочного режима (плюётся в Serial, )
#define ONEWIRE_PIN                   2           // номер пина 1-Wire
#define PWM_PIN                       3           // номер пина, на котором будем управлять частотой
#define SENSOR_PIN                    A0          // номер пина, с которого читаются показания датчика
#define SENSOR_UPDATE_INTERVAL        2000        // интервал обновления данных с датчика, миллисекунд
#define FREQUENCY                     2000000ul   // частота  1 - 2000000 (Гц)
#define PWM_VALUE                     150         // частота ШИМ (0-255)

#define LED1_GREEN 10                             // Светодиод индикации высокой влажности
#define LED2_RED 9                                // Светодиод индикации низкой влажности
#define LED3_BLUE 8                               // Светодиод индикации средней влажности
#define REED_SWITCH 7                            // Геркон калибровки датчика влажности

#define COMMON_ANODE
#define COLOR_NONE LOW, LOW, LOW
#define COLOR_RED HIGH, LOW, LOW
#define COLOR_GREEN LOW, HIGH, LOW
#define COLOR_BLUE LOW, LOW, HIGH



// АДРЕС МОДУЛЯ НА ЛИНИИ 1-WIRE МЕНЯТЬ ЗДЕСЬ! ЗНАЧЕНИЕ ПОСЛЕДНЕГО БАЙТА ПЕРЕД ЗАКРЫВАЮЩЕЙ СКОБКОЙ (например, сменить 0x00 на 0x01 и т.п.)
// КАЖДЫЙ ДАТЧИК НА ОДНОЙ ЛИНИИ 1-WIRE ОБЯЗАН ИМЕТЬ УНИКАЛЬНЫЙ АДРЕС !!!

// указываем 0xA0 - как тип данных влажности почвы, в остальном - датчик ведёт себя ровно так же, как DS18B20
//auto ds18b20 = DS18B20(DS18B20::family_code, 0x00, 0x00, 0xB2, 0x18, 0xDA, 0x09);   // DS18B20: 9-12bit, -55 -  +85 degC
byte num_module = 0x00;  // Индивидуальный номер датчика. Каждому датчику необходимо присваивать индивидуальный номер в диапазоне 0х00  - 0хFF.
auto ds18b20 = DS18B20(/*DS18B20::family_code*/0xA0, 0x00, 0x00, 0xB2, 0x18, 0xDA, num_module);   // DS18B20: 9-12bit, -55 -  +85 degC


//----------------------------------------------------------------------------------------------------------------
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// \/

//----------------------------------------------------------------------------------------------------------------

void setColor(bool red, bool green, bool blue)       // Включение цвета свечения трехцветного светодиода.
{
#ifdef COMMON_ANODE
	red = !red;
	green = !green;
	blue = !blue;
#endif
	digitalWrite(LED2_RED, red);
	digitalWrite(LED1_GREEN, green);
	digitalWrite(LED3_BLUE, blue);
}

//----------------------------------------------------------------------------------------------------------------
#define NO_TEMPERATURE_DATA -128 // нет данных с датчика температуры или влажности
constexpr uint8_t pin_onewire{ ONEWIRE_PIN };
auto hub = OneWireHub(pin_onewire);
uint32_t sensorUpdateTimer = 0;
//----------------------------------------------------------------------------------------------------------------
float ReadSensor()
{
	int val = analogRead(SENSOR_PIN);

#ifdef _DEBUG
	Serial.print(F("Val: "));
	Serial.println(val);
#endif  

   val = constrain(val,min(SOIL_MOISTURE_0_PERCENT,SOIL_MOISTURE_100_PERCENT),max(SOIL_MOISTURE_0_PERCENT,SOIL_MOISTURE_100_PERCENT));

   int percentsInterval = map(val,min(SOIL_MOISTURE_0_PERCENT,SOIL_MOISTURE_100_PERCENT),max(SOIL_MOISTURE_0_PERCENT,SOIL_MOISTURE_100_PERCENT),0,10000);
   
  // теперь, если у нас значение 0% влажности больше, чем значение 100% влажности - надо от 10000 отнять полученное значение
  if(SOIL_MOISTURE_0_PERCENT > SOIL_MOISTURE_100_PERCENT)
    percentsInterval = 10000 - percentsInterval;

   int8_t sensorValue;
   byte sensorFract;

   sensorValue = percentsInterval/100;
   sensorFract = percentsInterval%100;

   if(sensorValue > 99)
   {
      sensorValue = 100;
      sensorFract = 0;
   }

   if(sensorValue < 0)
   {
      sensorValue = NO_TEMPERATURE_DATA;
      sensorFract = 0;
   }

   if (sensorValue <= 35)
   {
	   setColor(COLOR_RED);
   }
   else if (sensorValue < 70 & sensorValue > 35)
   {
	   setColor(COLOR_GREEN);
   }
   else if (sensorValue >= 70)
   {
	   setColor(COLOR_BLUE);
   }
   Serial.println(sensorValue);

   float result = sensorValue;
   result += float(sensorFract)/100;

  return result;

}

//----------------------------------------------------------------------------------------------------------------
void setup()
{
  
	Serial.begin(115200);
  

	pinMode(REED_SWITCH, INPUT_PULLUP);  

	pinMode(LED1_GREEN, OUTPUT);
	pinMode(LED2_RED, OUTPUT);
	pinMode(LED3_BLUE, OUTPUT);

	setColor(COLOR_NONE);


	InitTimersSafe();                                  //инициализируем все таймеры, кроме 0,
	hub.attach(ds18b20);
	SetPinFrequencySafe(PWM_PIN, FREQUENCY);           //устанавливает частоту для указанного pin
	SetPinFrequency(PWM_PIN, FREQUENCY);               //устанавливает частоту для указанного pin
	pwmWrite(PWM_PIN, PWM_VALUE);                      //0-255   используйте эту функцию вместо analogWrite 

#ifdef _DEBUG
	Serial.print(F("family_code: 0xA0, 0x00, 0x00, 0xB2, 0x18, 0xDA, 0x"));
	Serial.println(num_module,HEX);
#endif

	Serial.println("config done");
	calibration_OK = EEPROM.read(address_calibr);

	SOIL_MOISTURE_0_PERCENT = EEPROM_int_read(address_SOIL_MOISTURE_0_PERCENT);
	SOIL_MOISTURE_100_PERCENT = EEPROM_int_read(address_SOIL_MOISTURE_100_PERCENT);

#ifdef _DEBUG
	Serial.print("SOIL_MOISTURE_0_PERCENT = ");
	Serial.println(SOIL_MOISTURE_0_PERCENT);
	Serial.print("SOIL_MOISTURE_100_PERCENT = ");
	Serial.println(SOIL_MOISTURE_100_PERCENT);

#endif
	Serial.println("Setup Ok");
}

//----------------------------------------------------------------------------------------------------------------
void loop()
{

	if (digitalRead(REED_SWITCH) == LOW && calibration == false)
	{
		sensor_calibration();
	}
	else if(calibration_OK == 0x55)
	{
		hub.poll();

		if (millis() - sensorUpdateTimer >= SENSOR_UPDATE_INTERVAL)
		{
			// Set temp
			float humidity = ReadSensor();
			ds18b20.setTemperature(humidity);
			sensorUpdateTimer = millis();

		}
	}
	else
	{
		blink_LED(10, 1,50);  // 1 - RED, 2 - BLUE, 3 - GREEN
		delay(500);
	}
}

// чтение
int EEPROM_int_read(int addr)
{
	byte raw[2];
	for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(addr + i);
	int &num = (int&)raw;
	return num;
}

// запись
void EEPROM_int_write(int addr, int num)
{
	byte raw[2];
	(int&)raw = num;
	for (byte i = 0; i < 2; i++) EEPROM.write(addr + i, raw[i]);
}

void sensor_calibration()
{
	setColor(COLOR_GREEN);
	delay(1000);

	blink_LED(15, 1, 100);  // 1 - RED, 2 - BLUE, 3 - GREEN

	int val0 = analogRead(SENSOR_PIN);
	//EEPROM_int_write(address_SOIL_MOISTURE_0_PERCENT, val0);
	Serial.print("SOIL_MOISTURE_0_PERCENT = ");
	Serial.println(val0);
	blink_LED(5, 3, 300);  // 1 - RED, 2 - BLUE, 3 - GREEN
	delay(1000);
	blink_LED(15, 2, 100);  // 1 - RED, 2 - BLUE, 3 - GREEN
	int val100 = analogRead(SENSOR_PIN);
//	EEPROM_int_write(address_SOIL_MOISTURE_100_PERCENT, val100);
	Serial.print("SOIL_MOISTURE_100_PERCENT = ");
	Serial.println(val100);
	delay(1000);
	calibration_OK = 0x55;
	EEPROM.write(address_calibr, calibration_OK);
	Serial.println("Calibration complete");

	delay(1000);
	while (digitalRead(REED_SWITCH)==LOW) 
	{	// Ожидаем выключения геркона
		setColor(COLOR_RED);
		delay(300);
		setColor(COLOR_NONE);
		delay(300);
	}

	if (val0 <= val100 +5 && val0 >= val100 - 5)
	{
		Serial.println("Calibration failed");
		blink_LED(50, 1, 50);  // 1 - RED, 2 - BLUE, 3 - GREEN
	}
	else
	{
		Serial.println("Successful calibration");
		calibration = false;
		EEPROM_int_write(address_SOIL_MOISTURE_0_PERCENT, val0);
		EEPROM_int_write(address_SOIL_MOISTURE_100_PERCENT, val100);

		SOIL_MOISTURE_0_PERCENT = EEPROM_int_read(address_SOIL_MOISTURE_0_PERCENT);
		SOIL_MOISTURE_100_PERCENT = EEPROM_int_read(address_SOIL_MOISTURE_100_PERCENT);
		setColor(COLOR_GREEN);
		Serial.println("Parameters saved");
		Serial.print("SOIL_MOISTURE_0_PERCENT = ");
		Serial.println(SOIL_MOISTURE_0_PERCENT);
		Serial.print("SOIL_MOISTURE_100_PERCENT = ");
		Serial.println(SOIL_MOISTURE_100_PERCENT);
		Serial.println();
		delay(2000);

	}


}

void blink_LED(int count, int color, int time )
{

	for (int i = 0; i < count; i++)
	{
		switch (color) 
		{
		case 1:
			setColor(COLOR_RED);
			break;
		case 2:
			setColor(COLOR_BLUE);
			break;
		case 3:
			setColor(COLOR_GREEN);
			break;
		}

		delay(time);
		setColor(COLOR_NONE);
		delay(time);
	}

}





//----------------------------------------------------------------------------------------------------------------
