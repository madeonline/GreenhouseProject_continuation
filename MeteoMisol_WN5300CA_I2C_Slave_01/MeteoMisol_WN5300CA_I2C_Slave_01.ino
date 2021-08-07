
// Библиотека iarduino_I2C_connect разработана для удобства соединения нескольких arduino по шине I2C
// Данная Arduino является ведомым устройством на шине I2C с адресом 0x01


// Подключаем библиотеки:
#include <Wire.h>                                     // подключаем библиотеку для работы с шиной I2C
#include <iarduino_I2C_connect.h>                     // подключаем библиотеку для соединения arduino по шине I2C
// Объявляем переменные и константы:
iarduino_I2C_connect I2C2;                            // объявляем переменную для работы c библиотекой iarduino_I2C_connect

byte           REG_Array[15];                        // объявляем массив, данные которого будут доступны для чтения/записи по шине I2C

/*
Пакет MISOL WN5300CA

REG_Array[0] = SecurityCode;      // ID метеостанции. Код безопасности
REG_Array[1] = TempBitError;      // ошибки показаний температуры
REG_Array[2] = Temperatue >> 8;   // Старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1) + направление ветра
REG_Array[3] = Temperatue;        // Младший байт значения температуры
REG_Array[4] = Humidity;          // Влажность воздуха
REG_Array[5] = WindSpeed;         // Скорость ветра
REG_Array[6] = Gust;              // Порывы ветра
REG_Array[7] = RainCounter >> 8;  // Старший байт счётчик осадков
REG_Array[8] = RainCounter;       // Младший байт счётчик осадков
REG_Array[9] = WindDirBitError;   // Бит ошибки показаний направления ветра
REG_Array[10] = LowBattBit;       // Бит разряженной батареи передатчика
REG_Array[11] = WindDir;          // Направление ветра
REG_Array[12] = calc_REG_Array(); // Записать расчетную контрольную сумму в массив
REG_Array[13] = 0;                //
REG_Array[14] = true;             // Записать подтверждение готовности пакета к передаче

*/




//=============================================================================================================================

uint16_t tmp = 0;  // Получить ID станции



//=============================================================================================================================

// Преамбула: 8 импульсов, 500 мкс. - HIGH, 1000 - LOW

#define RXB_DATA        2   //  PIN данных датчика RXB6
#define PREAMBLE_COUNT  8   //  Количество символов преамбулы
#define PREAMBLE_HIGH   500 //  Среднее HIGH в преамбуле, мкс
#define PRECISION       80  //  Точность при отклонении от длинысигнала в мкс., чем меньше число, тем строже фильтр, не реккомендуется выставлять менее 50 мкс.
#define BIT_COUNT       80  //  Количество ожидаемых бит данных


#define COMMON_ANODE

#define LED_RED         9   //  Светодиод индикации отсутствия приема пакета данных с метеостанции
#define LED_GREEN       8   //  Светодиод индикации нормальной работы системы
#define LED_BLUE        10  //  Светодиод индикации приема пакета данных с метеостанции

#define COLOR_NONE LOW, LOW, LOW
#define COLOR_RED HIGH, LOW, LOW
#define COLOR_GREEN LOW, HIGH, LOW
#define COLOR_BLUE LOW, LOW, HIGH

unsigned long packet_Millis = 0;             // Время индикации приема пакета
unsigned long false_Millis = 0;              // Время индикации отсутсвия приема пакета
const long interval_packet = 1000;           // Время индикации приема пакета
const long interval_false  = 100000;         // Время индикации отсутсвия приема пакета
bool packet_on = false; 


byte TxType;            //  Тип передатчика
byte SecurityCode;      //  Код безопасности
byte TempBitError;      //  Бит(-ы, почему-то 2) ошибки показаний температуры
word Temperatue;        //  Температура
int Temperatue_send; 
byte Humidity;          //  Влажность
byte WindSpeed;         //  Скорость ветра
byte Gust;              //  Порывы ветра
word RainCounter;       //  Счетчик осадков
byte WindDirBitError;   //  Бит ошибки показаний направления ветра
byte LowBattBit;        //  Бит разряженной батареи передатчика
byte WindDir;           //  Направление ветра
byte Crc;               //  Контрольная сумма

byte led_send = false;
    
byte writeByte(int StartBit, int DescCount, bool reverse = true);  // Функция установки значения байту
word writeWord(int StartBit, int DescCount);                       // Функция установки значений слову
void parseData();                                                  // Функция разбора данных метеостанции
void printSerial();                                                // Функция вывода сообщения в канал
double getTemperature();                                           // Функция получения температуры в гр. С
double getWindSpeed(byte wind);                                    // Функция получения скорости ветра в м/с
String getWindDir();                                               // Функция получения направления ветра
int getRain(byte rain);                                            // Функция получения кол-ва осадков
word calcCRC();                                                    // Функция расчёта CRC8

// через volatile, так как изменяется через обработчик прерываний и должна быть в ОЗУ
volatile boolean  meteoListening = true;      //  Переменная - флаг получения пакета
volatile uint32_t meteoStartPulse = 0;        //  Метка времени, на которой был пойман HIGH
volatile uint8_t  meteoPreambleCount = 0;     //  Количество считанных бит преамбулы
volatile uint8_t  meteoBitCounter;            //  Счетчик считанных бит данных
volatile boolean  meteoBitArray[BIT_COUNT];   //  Массив считанных бит данных
volatile uint32_t meteoStartPreamble = 0;     //  Метка времени, старт преамбулы
volatile uint32_t Te;
volatile uint32_t Te2_3;

bool meteoCorrentData;                        //  Признак получения корректных данных с метеостанции


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

  Wire.begin(0x01);                                                            // инициируем подключение к шине I2C в качестве ведомого (slave) устройства, с указанием своего адреса на шине.
  I2C2.begin(REG_Array);                                                       // инициируем возможность чтения/записи данных по шине I2C, из/в указываемый массив
  pinMode(RXB_DATA, INPUT_PULLUP);                                             //  Маркируем порт, как порт для прерываний
  attachInterrupt(digitalPinToInterrupt(RXB_DATA), meteoInterrupt, CHANGE);    //  Подключаем обработку прерываний
  Serial.println("Init OK");
}
// Основной цикл
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


  if (meteoListening == false) 
  {
    // Включаем обработку показаний с метеостанции
    if (meteoCorrentData) 
	{
      // Готовим пакет для передачи I2C.

     
		REG_Array[0] = SecurityCode;      // ID метеостанции. Код безопасности 
		REG_Array[1] = TempBitError;      // ошибки показаний температуры
		REG_Array[2] = Temperatue_send >> 8;   // Старший байт значения температуры ведомого (адрес ведомого 0x01, номер регистра 1) + направление ветра
		REG_Array[3] = Temperatue_send;        // Младший байт значения температуры
		REG_Array[4] = Humidity;          // Влажность воздуха
		REG_Array[5] = WindSpeed;         // Скорость ветра
		REG_Array[6] = Gust;              // Порывы ветра
		REG_Array[7] = RainCounter >> 8;  // Старший байт счётчик осадков
		REG_Array[8] = RainCounter;       // Младший байт счётчик осадков
		REG_Array[9] = WindDirBitError;   // Бит ошибки показаний направления ветра
		REG_Array[10] = LowBattBit;       // Бит разряженной батареи передатчика
		REG_Array[11] = WindDir;          // Направление ветра
		REG_Array[12] = calc_REG_Array(); // Записать расчетную контрольную сумму в массив  
	    REG_Array[13] = 0;                // 

        Serial.print("calc_REG_Array - ");
        Serial.print(REG_Array[12]);

		REG_Array[14] = 1;                 // Записать подтверждение готовности пакета к передаче

		meteoCorrentData = false;          //  Обнуляем признак получения данных с метеостанции

		packet_Millis = currentMillis;
		packet_on = true;
		setColor(COLOR_BLUE);
    }
    meteoListening = true;
  }
}

//  Проверка корректности длины битов преамбулы
boolean checkLenPreamblePulse(uint32_t p_duration) 
{
  if (p_duration >= (PREAMBLE_HIGH-PRECISION) && (p_duration <= (PREAMBLE_HIGH+PRECISION))) 
  {
    return true;
  }
  return false;
}

// Функция записи бита в протокол
byte writeByte(int StartBit, int DescCount, bool reverse) 
{
    int   Count = 0;
    int   BitPos;
    byte  ResultByte = 0;
    if (reverse) 
	{
      while (Count < DescCount) 
	  {
        BitPos = StartBit-Count;
        bitWrite(ResultByte, Count, meteoBitArray[BitPos]);
        Count++;
      }
    }
	else 
	{
      while (Count < DescCount) 
	  {
        BitPos = StartBit+Count;
        bitWrite(ResultByte, Count, meteoBitArray[BitPos]);
        Count++;
      }
    }
    return ResultByte;
}

// Функция записи пары байт в протокол
word writeWord(int StartBit, int DescCount)
{
    int   Count = 0;
    int   BitPos;
    word  ResultWord = 0;
    while (Count < DescCount) 
	{
      BitPos = StartBit-Count;
      bitWrite(ResultWord, Count, meteoBitArray[BitPos]);
      Count++;
    }
    return ResultWord;
}

// Функция вывода показаний в канал
void printSerial()
{
    // Покажем весь принятый пакет
    int i = 0;
    Serial.print("Data:  ");
    while(i < BIT_COUNT)
	{
      Serial.print(String(meteoBitArray[i]));
      i++;
    }
    Serial.println("");
    // Тип передатчика
    Serial.print("TX Type: ");
    Serial.print(TxType, HEX);
    // Код безопасности
    Serial.print(" | Security Code: ");
    Serial.print(SecurityCode, HEX);
    //  Бит разряженной батареи передатчика
    Serial.print(" | Low Battary: ");
    Serial.print(LowBattBit, DEC);
    // Температура
    Serial.print(" | Temperature: ");
    Serial.print(getTemperature(),1);
    Serial.print("*C ");
    // Влажность
    Serial.print( "| Humidity: ");
    Serial.print(Humidity, DEC);
    Serial.print("%");
    // Скорость ветра
    Serial.print(" | Wind ");
    Serial.print(getWindDir());
    Serial.print(": speed: ");
    Serial.print(getWindSpeed(WindSpeed),1);
    Serial.print(" m/s");
    // Порывы ветра
    Serial.print(", gust: ");
    Serial.print(getWindSpeed(Gust),1);
    Serial.print(" m/s");
    // Счётчик осадков
    Serial.print(" | Rain: ");
    Serial.print(getRain(RainCounter),1);
    // Crc
    Serial.print(" | CRC: ");
    Serial.print(Crc, BIN);
    Serial.print(" -> 0x");
    Serial.println(Crc, HEX);
}

// Функция получения температуры в гр. С
double getTemperature()
{
  if (TempBitError == 0) 
  {
    return ((double)Temperatue-400)/10.0;
  }
}


// Функция получения направления ветра
String getWindDir()
{
  if (WindDirBitError == 0)  
  {
      switch (WindDir) 
	  {
        case 0x0: return "N";    break;
        case 0x1: return "NNE";  break;
        case 0x2: return "NE";   break;
        case 0x3: return "NEE";  break;
        case 0x4: return "E";    break;
        case 0x5: return "EES";  break;
        case 0x6: return "ES";   break;
        case 0x7: return "ESS";  break;
        case 0x8: return "S";    break;
        case 0x9: return "SSW";  break;
        case 0xA: return "SW";   break;
        case 0xB: return "SWW";  break;
        case 0xC: return "W";    break;
        case 0xD: return "WWN";  break;
        case 0xE: return "WN";   break;
        case 0xF: return "WNN";  break;
      }
  }
  return "NULL";
}

// Функция получения силы ветра
double  getWindSpeed(byte wind)
{
  return (wind*0.34);
}

// Функция получения кол-ва осадков
// Значение - счётчик, накапливается. 
// Для определения дифференциальной величины, например, за час, необходимо в начале каждого часа находить разницу между предшествующим и текущим значением
int getRain(byte rain)
{
  return (rain*0.3);
}

// Функция разбора данных метеостанции
void parseData()
{
    // Тип передатчика
    TxType = writeByte(3,4);
    // Код безопасности
    SecurityCode = writeByte(11,8);
    //  Бит(-ы, почему-то 2) ошибки показаний температуры
    TempBitError = writeByte(13,2);
    // Температура в форенгейтах
    Temperatue = writeWord(23,10);
    Temperatue_send  = writeWord(23,10);
    // Влажность
    Humidity = writeByte(31,8);
    // Скорость ветра
    WindSpeed = writeByte(39,8);
    // Порывы ветра
    Gust = writeByte(47,8);
    // Счётчик осадков
    RainCounter = writeWord(63,16);
    // Бит ошибки показаний направления ветра
    WindDirBitError = writeByte(64,1);
    //  Бит разряженной батареи передатчика
    LowBattBit = writeByte(67,1);
    // Направление ветра
    WindDir = writeByte(71,4);
    // Crc
    Crc = writeByte(79,8);
}

word calcCRC()
{
  word polynom = 0x131; //  Полином расчитан алгоритмом https://sourceforge.net/projects/reveng/ для массива данных с обнулёнными последними 8-и битами сообщения: "CRC-8/MAXIM-DWO"
  word crcValue;
  boolean  calcArray[BIT_COUNT];   //  Свой массив
  // Копируем массив
  for (int i = 0; i < BIT_COUNT; i++) 
  {
    calcArray[i] = meteoBitArray[i];
  }
  // Обнуляем хвост
  for (int i = 0; i < 9; i++) 
  {
    calcArray[(BIT_COUNT-i)] = 0;
  }
  // Установки
  crcValue = 0x00;
  // Читаем биты до конца
  for (int i = 0; i < BIT_COUNT; i++) 
  {
    bitWrite(crcValue, 0, calcArray[i]);
    if bitRead(crcValue,8) {
      crcValue = crcValue ^ polynom;
    }
    crcValue <<= 1;
  }
  crcValue >>= 1;
  return crcValue;
}

// Функция обработки прерываний
void meteoInterrupt() 
{
  // Если пока не читаем, значит не читаем
  if(meteoListening == false)
  {
    return;
  }
  // Фиксирум/обновляем время изменения импульса
  uint32_t cur_timestamp  = micros();                         //  Метка времени
  uint8_t  cur_status     = digitalRead(RXB_DATA);            //  Тип импульса
  uint32_t pulse_duration = cur_timestamp - meteoStartPulse;  //  Длина последнего импульса

  // Начало по восхождению
  if (cur_status == HIGH) 
  {
    meteoStartPulse = cur_timestamp;  
  }
  
  // Анализ по спаду
  if (cur_status == LOW) 
  {
    // Ловим преамбулу
    if (meteoPreambleCount < PREAMBLE_COUNT) 
	{
      // Отметим время старта преамбулы
      if (meteoPreambleCount == 0) 
	  {
        meteoStartPreamble = cur_timestamp;
      }
      // Проверяем длину HIGH импульса
      if (checkLenPreamblePulse(pulse_duration)) 
	  {
        meteoPreambleCount ++;
        if(meteoPreambleCount == PREAMBLE_COUNT)
		{
          // Словили преамбулу
          meteoBitCounter = 0;
          // Расчёт длительности Te
          Te = (cur_timestamp - meteoStartPreamble) / 23; // вычисляем длительность базового импульса Te
          Te2_3 = Te * 3 / 2;
        }
      }
	  else 
	  {
        // Не то пальто, обнуляем
        meteoPreambleCount = 0;
      }
    }
    // Ловим битЫ
    if (meteoPreambleCount == PREAMBLE_COUNT && meteoBitCounter < BIT_COUNT) 
	{
      meteoBitArray[meteoBitCounter] = (pulse_duration > Te2_3) ? 0 : 1;
      meteoBitCounter++;
    }
  }
  // Кажется всё поймали
  if (meteoBitCounter == BIT_COUNT) 
  {
    // Перестаём читать, парсим
    meteoListening = false;
    parseData();
    if (TxType == 0xA && calcCRC() == Crc) 
	{
      printSerial();
      meteoCorrentData = true;
    }
    // Обнулим счётчики бит
    meteoBitCounter = 0;
    meteoPreambleCount = 0;
  }
}

//=============================================================================================================================


int calc_REG_Array()
{
	uint8_t tmp_byte = 0;
	uint16_t tmp_result = 0;
		for (uint8_t j = 0; j < 12; j++)
		{
			tmp_byte = 0;
			tmp_byte = REG_Array[j];
			tmp_result = tmp_result + tmp_byte;
		}
	return (tmp_result & 0xFF);
}
