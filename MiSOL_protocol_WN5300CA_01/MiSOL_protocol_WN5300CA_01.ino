// Преамбула: 8 импульсов, 500 мкс. - HIGH, 1000 - LOW

#define RXB_DATA        2   //  PIN данных датчика RXB6
#define PREAMBLE_COUNT  8   //  Количество символов преамбулы
#define PREAMBLE_HIGH   500 //  Среднее HIGH в преамбуле, мкс
#define PRECISION       50  //  Точность при отклонении от длинысигнала в мкс., чем меньше число, тем строже фильтр, не реккомендуется выставлять менее 50 мкс.
#define BIT_COUNT       80  //  Количество ожидаемых бит данных

#include <Wire.h>             // Библиотека для работы с шиной I2C


class WH14 
{
public:
    byte TxType;            //  Тип передатчика
    byte SecurityCode;      //  Код безопасности
    byte TempBitError;      //  Бит(-ы, почему-то 2) ошибки показаний температуры
    word Temperatue;        //  Температура
    byte Humidity;          //  Влажность
    byte WindSpeed;         //  Скорость ветра
    byte Gust;              //  Порывы ветра
    word RainCounter;       //  Счетчик осадков
    byte WindDirBitError;   //  Бит ошибки показаний направления ветра
    byte LowBattBit;        //  Бит разряженной батареи передатчика
    byte WindDir;           //  Направление ветра
    byte Crc;               //  Контрольная сумма
    
    byte writeByte(int StartBit, int DescCount, bool reverse = true);  // Функция установки значения байту
    word writeWord(int StartBit, int DescCount);                       // Функция установки значений слову
    void parseData();                                                  // Функция разбора данных метеостанции
    void printSerial();                                                // Функция вывода сообщения в канал
    double getTemperature();                                           // Функция получения температуры в гр. С
    double getWindSpeed(byte wind);                                    // Функция получения скорости ветра в м/с
    String getWindDir();                                               // Функция получения направления ветра
    int getRain(byte rain);                                            // Функция получения кол-ва осадков
    word calcCRC();                                                    // Функция расчёта CRC8
};

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

WH14 wh14;

void setup() 
{
  Serial.begin(115200);
  pinMode(RXB_DATA, INPUT_PULLUP);                                             //  Маркируем порт, как порт для прерываний
  attachInterrupt(digitalPinToInterrupt(RXB_DATA), meteoInterrupt, CHANGE);    //  Подключаем обработку прерываний
  Serial.println("Init OK");
}
// Основной цикл
void loop() 
{
  if (meteoListening == false) 
  {
    // Включаем обработку показаний с метеостанции
    if (meteoCorrentData) 
	{
      // Готовим пакет для передачи I2C. Пока не сделан.

      meteoCorrentData = false; //  Обнуляем признак получения данных с метеостанции
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
byte WH14::writeByte(int StartBit, int DescCount, bool reverse) 
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
word WH14::writeWord(int StartBit, int DescCount) 
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
void WH14::printSerial() 
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
    Serial.print(getWindSpeed(wh14.WindSpeed),1);
    Serial.print(" m/s");
    // Порывы ветра
    Serial.print(", gust: ");
    Serial.print(getWindSpeed(wh14.Gust),1);
    Serial.print(" m/s");
    // Счётчик осадков
    Serial.print(" | Rain: ");
    Serial.print(getRain(wh14.RainCounter),1);
    // Crc
    Serial.print(" | CRC: ");
    Serial.print(Crc, BIN);
    Serial.print(" -> 0x");
    Serial.println(Crc, HEX);
}

// Функция получения температуры в гр. С
double WH14::getTemperature() 
{
  if (TempBitError == 0) 
  {
    return ((double)Temperatue-400)/10.0;
  }
}


// Функция получения направления ветра
String WH14::getWindDir() 
{
  if (wh14.WindDirBitError == 0)  
  {
      switch (wh14.WindDir) 
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
double WH14::getWindSpeed(byte wind) 
{
  return (wind*0.34);
}

// Функция получения кол-ва осадков
// Значение - счётчик, накапливается. 
// Для определения дифференциальной величины, например, за час, необходимо в начале каждого часа находить разницу между предшествующим и текущим значением
int WH14::getRain(byte rain) 
{
  return (rain*0.3);
}

// Функция разбора данных метеостанции
void WH14::parseData() 
{
    // Тип передатчика
    TxType = writeByte(3,4);
    // Код безопасности
    SecurityCode = writeByte(11,8);
    //  Бит(-ы, почему-то 2) ошибки показаний температуры
    TempBitError = writeByte(13,2);
    // Температура в форенгейтах
    Temperatue = writeWord(23,10);
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

word WH14::calcCRC() 
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
    wh14.parseData();
    if (wh14.TxType == 0xA && wh14.calcCRC() == wh14.Crc) 
	{
      wh14.printSerial();
      meteoCorrentData = true;
    }
    // Обнулим счётчики бит
    meteoBitCounter = 0;
    meteoPreambleCount = 0;
  }
}

