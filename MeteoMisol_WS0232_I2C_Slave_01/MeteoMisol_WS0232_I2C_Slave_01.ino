// Библиотека iarduino_I2C_connect разработана для удобства соединения нескольких arduino по шине I2C
// Данная Arduino является ведомым устройством на шине I2C с адресом 0x01


// Подключаем библиотеки:
#include <Wire.h>                             // подключаем библиотеку для работы с шиной I2C
#include <iarduino_I2C_connect.h>             // подключаем библиотеку для соединения arduino по шине I2C

// Объявляем переменные и константы:
iarduino_I2C_connect I2C2;                    // объявляем переменную для работы c библиотекой iarduino_I2C_connect

byte  REG_Array[15];                          // объявляем массив, данные которого будут доступны для чтения/записи по шине I2C
//=============================================================================================================================

/*
Пакет MISOL WS0232
REG_Array[0];    // ID станции   
REG_Array[1];    // 
REG_Array[2];    //
REG_Array[3];    //
REG_Array[4];    // Старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1) + направление ветра
REG_Array[5];    // Младший байт значения температуры
REG_Array[6];    // Влажность воздуха
REG_Array[7];    // Старший байт скорости ветра
REG_Array[8];    // Младший байт скорости ветра
REG_Array[9];    // 
REG_Array[10];   //
REG_Array[11];   // Передает состояние счетчика импульсов коромысла.
REG_Array[12];   // принятую контрольную сумму из пакета
REG_Array[13];   // контрольную сумму расчитанную пакета
REG_Array[14];   // Отправляем состояние флага ведомому (адрес ведомого 0x01, номер регистра 14, состояние флага)

*/




uint8_t count = 0;
volatile uint8_t res[225];
volatile uint8_t flag = 0;
uint8_t head_flag = 0;
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;

uint16_t tmp = 0;  // Получить ID станции

#define rxPin 2

#define MAX_DELTA 140

#define COMMON_ANODE

#define LED_RED         9   //  Светодиод индикации отсутствия приема пакета данных с метеостанции
#define LED_GREEN       8   //  Светодиод индикации нормальной работы системы
#define LED_BLUE        10  //  Светодиод индикации приема пакета данных с метеостанции

#define COLOR_NONE LOW, LOW, LOW
#define COLOR_RED HIGH, LOW, LOW
#define COLOR_GREEN LOW, HIGH, LOW
#define COLOR_BLUE LOW, LOW, HIGH

unsigned long packet_Millis = 0;             // Время индикации приема пакета
unsigned long false_Millis  = 0;             // Время индикации отсутсвия приема пакета
const long interval_packet  = 1000;          // Время индикации приема пакета
const long interval_false   = 100000;        // Время индикации отсутсвия приема пакета
bool packet_on              = false;

void setColor(bool red, bool green, bool blue)       // Включение цвета свечения трехцветного светодиода.
{
#ifdef COMMON_ANODE
	red = !red;
	green = !green;
	blue = !blue;
#endif
	digitalWrite(LED_RED, red);
	digitalWrite(LED_GREEN, green);
	digitalWrite(LED_BLUE, blue);
}



void setup()
{
  Serial.begin(115200);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  setColor(COLOR_GREEN);
  delay(500);
  setColor(COLOR_RED);
  delay(500);
  setColor(COLOR_BLUE);
  delay(500);
  setColor(COLOR_GREEN);

//Wire.setClock(400000);                              // устанавливаем скорость передачи данных по шине I2C = 400кБит/с
  Wire.setClock(100000);                              // устанавливаем скорость передачи данных по шине I2C = 100кБит/с
  Wire.begin(0x01);                                   // инициируем подключение к шине I2C в качестве ведомого (slave) устройства, с указанием своего адреса на шине.
  I2C2.begin(REG_Array);                              // инициируем возможность чтения/записи данных по шине I2C, из/в указываемый массив
  pinMode(rxPin, INPUT_PULLUP);
  attachInterrupt(0, read_input, CHANGE);             // 
  Serial.println("Setup END\n");
}
void loop()
{

	unsigned long currentMillis = millis();

	if (currentMillis - false_Millis >= interval_false)
	{
		// save the last time you blinked the LED
		false_Millis = currentMillis;
		setColor(COLOR_RED);

	}

	if (currentMillis - packet_Millis >= interval_packet && packet_on == true)
	{
		false_Millis = currentMillis;
		packet_on = false;
		setColor(COLOR_GREEN);
	}


	if (flag == 1) //если были данные
	{
		tmp = 0;  // Получить ID станции
		for (uint8_t i = 0; i < 8; i++)
		{
			tmp <<= 1;
			tmp |= res[i] & 1;
		};
		Serial.println();
		Serial.print("ID: ");
		Serial.println(tmp, HEX);

		//REG_Array[0] = tmp;  // Записать ID станции в массив

		// Получить контрольную сумму из принятого пакета
		uint8_t crc = 0;    
		for (uint8_t i = 96; i < 104; i++)
		{
			crc <<= 1;
			crc |= res[i] & 1;
		};

		Serial.print("received CRC: ");
		Serial.println(crc);

		// Посчитать контрольную сумму принятого пакета
		uint8_t Calc_CRC = calc_CRC();      // Расчет контрольной суммы первого пакета
		uint8_t Calc_CRC2 = calc_CRC2();    // Расчет контрольной суммы второго пакета
		Serial.print("calc_CRC: ");
		Serial.println(Calc_CRC);
		Serial.print("calc_CRC2: ");
		Serial.println(Calc_CRC2);

		REG_Array[12] = crc;             //   Записать принятую контрольную сумму в массив 
		REG_Array[13] = Calc_CRC;        //   Записать расчетную контрольную сумму в массив  

	

		if (tmp == 0xF5 && crc == Calc_CRC && crc == Calc_CRC2)
		{ //первый байт должен быть 0xF5 (ID станции, может варьироваться от модели и верный CRC
		  //ID станции возможно необходимо настраивать под конкретную?

		  //==============Температура========================
			float temp = 0;
			int16_t temp_tmp = 0;
			int16_t temp_tmp1 = 0;
			for (uint8_t i = 36; i < 48; i++)    //биты температуры
			{
				temp_tmp <<= 1;                  //смещаем влево
				temp_tmp |= res[i] & 1;          //прибавляем младший бит
				temp_tmp1 <<= 1;
				temp_tmp1 |= res[i + 112] & 1;   //прибавляем младший бит
			};
			//сверяем значение из двух посылок, если не ок, то выставляем в 255
			if (temp_tmp != temp_tmp1) 
			{
				temp_tmp = 255;
			}
			else
			{
				temp = (float)(temp_tmp) / 10.0;
			}

			//============Влажность=====================
			uint8_t hum = 0;
			uint8_t hum1 = 0;
			for (uint8_t i = 48; i < 56; i++) //биты влажности наружного воздуха
			{
				hum <<= 1;
				hum |= res[i] & 1;
				hum1 <<= 1;
				hum1 |= res[i + 112] & 1;
			};
			if (hum != hum1)
			{
				hum = 255;
			}

			//==============скорость ветра=================
			uint16_t ch_wind = 0;
			uint16_t ch_wind1 = 0;
			float wind = 0;
			for (uint8_t i = 56; i < 72; i++) //скорость ветра
			{
				ch_wind <<= 1;
				ch_wind |= res[i] & 1;
				ch_wind1 <<= 1;
				ch_wind1 |= res[i + 112] & 1;
			};
			//сверяем значение из двух посылок, если не ок, то выставляем в 255
			if (ch_wind != ch_wind1) 
			{
				ch_wind = 255;
			}
			else
			{
				wind = (float)ch_wind / 588;
			}

			//==================направление ветра====================
			uint8_t wind_dir = 0;
			uint8_t wind_dir1 = 0;
			for (uint8_t i = 32; i < 36; i++)
			{
				wind_dir <<= 1;
				wind_dir |= res[i] & 1;
				wind_dir1 <<= 1;
				wind_dir1 |= res[i + 112] & 1;
			};
			if (wind_dir != wind_dir1) 
			{
				wind_dir = 255;
			}

			//==================Дождь============================================= 
			// Передает состояние счетчика импульсов коромысла. 
			// Для определения наличия дождя нужно сравнивать предыдущие данные и текущие.

			uint8_t rain = 0;
			uint8_t rain1 = 0;
			for (uint8_t i = 88; i < 96; i++)         //биты счетчика импульсов дождя
			{
				rain <<= 1;
				rain |= res[i] & 1;
				rain1 <<= 1;
				rain1 |= res[i + 112] & 1;
			};
			if (rain != rain1)                     //сверяем значение из двух посылок, если не ок, то выставляем в 255
			{
				rain = 255;
			}

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

			//Serial.print("\n");          //
			////Батарея (при тестировании не подтвердилось, не применять!)
			//if (res[21] == res[21 + 112]&& res[21] == 0 )
			//{
			//	Serial.print(res[21 + 112]);
			//	Serial.print(" -  Low battery!!!");
			//}


			Serial.print("\n\n");
			//====================================== тестовый код отображения двух посылок ===================================================================
			Serial.print("    ID  \t\t            P  \t   Direct      Temp \t      Humidity     \t Wind        \t\t     Rain \tCRC");
			Serial.print("\n");
			for (uint8_t i = 0; i < 112; i++)
			{
				Serial.print(res[i]);
				//  if(i==7||i==20||i==21||i==31||i==35||i==47||i==55||i==71||i==79 || i == 87 ||i==95 || i == 103)
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63 || i == 71 || i == 79 || i == 87 || i == 95 || i == 103)
				{
					Serial.print(" ");
				}
			}
			Serial.print("\n");
			uint8_t i1 = 112;
			for (uint8_t i = 112; i < 112 + i1; i++)
			{
				Serial.print(res[i]);
				if (i == 7 + i1 || i == 15 + i1 || i == 23 + i1 || i == 31 + i1 || i == 39 + i1 || i == 47 + i1 || i == 55 + i1 || i == 63 + i1 || i == 71 + i1 || i == 79 + i1 || i == 87 + i1 || i == 95 + i1 || i == 103 + i1)
				{
					Serial.print(" ");
				}
			}
			//==================================================================================================================================================

			Serial.print("\n\n");
			REG_Array[14] = 1;             // Записать подтверждение готовности пакета к передаче 

				//byte calc_REG = calc_REG_Array();
			Serial.print("REG_Array - ");
			Serial.println(calc_REG_Array()); 
		}
		delay(10);
		res[0] = 0;
		flag = 0;
		count = 0;


		packet_Millis = currentMillis;
		packet_on = true;
		setColor(COLOR_BLUE);

		interrupts();
	}

	

}

boolean CheckValue(unsigned int base, unsigned int value) {
	return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA)) || ((value < base) && ((base - value) < MAX_DELTA)));
}

void read_input()
{
	state = digitalRead(rxPin);
	if (state == HIGH)
	{
		lolen = micros() - prevtime;
	}
	else
	{
		hilen = micros() - prevtime;
	}
	prevtime = micros();


	if (state == LOW)
	{
		// по спаду начинаем анализ
		if (CheckValue(300, hilen) && CheckValue(1770, lolen) && head_flag != 2)
		{ //нашли короткий импульс
			head_flag = 1;
		}
		else if (head_flag != 2) head_flag = 0; //если нет - все заново
		if (CheckValue(2034, hilen) && CheckValue(1520, lolen) && head_flag != 2)
		{ //нашли длинный импульс
			head_flag = 2;
		}
		else if (head_flag != 2) head_flag = 0; //если нет - все заново
		// голова посылки найдена начинаем прием полезных 224 бит данных
		if (CheckValue(960, hilen) && head_flag == 2) //бит 1 - 960 мс
		{
			res[count] = 1;
			count++;
		}
		else if (CheckValue(460, hilen) && head_flag == 2) // бит 0 - 480 мс
		{
			res[count] = 0;
			count++;
		}
		if (count == 223) { // отсчитали нужное число бит, все сбрасываем и запрещаем прерывания
			noInterrupts();
			head_flag = 0;
			flag = 1; //уходим в парсинг пакета
			count = 0;
			lolen = 0;
			hilen = 0;
		}
	}
}

int calc_CRC()
{
	uint8_t tmp_byte = 0;
	uint16_t tmp_result = 0;
	if (tmp == 0xF5)
	{
		for (uint8_t j = 0; j < 12; j++)
		{
			tmp_byte = 0;
			for (uint8_t i = 0; i < 8; i++)
			{
				tmp_byte <<= 1; //смещаем влево
				tmp_byte |= res[8 * j + i] & 1; //прибавляем младший бит
			}
			REG_Array[j] = tmp_byte;            // Записать принятый пакет в массив для передачи по I2C
			tmp_result = tmp_result + tmp_byte;
		}
	}
	return (tmp_result & 0xFF);
}

int calc_CRC2()  
{
	uint8_t tmp_byte = 0;
	uint16_t tmp_result = 0;
	if (tmp == 0xF5)
	{
		for (uint8_t j = 0; j < 12; j++)
		{
			tmp_byte = 0;
			for (uint8_t i = 112; i < 120; i++)
			{
				tmp_byte <<= 1; //смещаем влево
				tmp_byte |= res[8 * j + i] & 1; //прибавляем младший бит
			}
			tmp_result = tmp_result + tmp_byte;
		}
	}
	return (tmp_result & 0xFF);
}

int calc_REG_Array()
{
	uint8_t tmp_byte = 0;
	uint16_t tmp_result = 0;
	if (tmp == 0xF5)
	{
		for (uint8_t j = 0; j < 12; j++)
		{
			tmp_byte = 0;
			tmp_byte = REG_Array[j];
			tmp_result = tmp_result + tmp_byte;
		}
	}
	return (tmp_result & 0xFF);
}
