//----------------------------------------------------------------------------------------------------------------
/*
  ПАМЯТКА ПО КОМАНДАМ КОНТРОЛЛЕРА:

  0. КАЖДАЯ КОМАНДА ЗАКАНЧИВАЕТСЯ ПЕРЕВОДОМ СТРОКИ!

  1. КОМАНДЫ ЧЕРЕЗ UART РАБОТАЮТ, ТОЛЬКО ЕСЛИ USE_RS485_GATE ЗАКОММЕНТИРОВАНО;

  2. ДЛЯ ПРОСМОТРА ID КОНТРОЛЛЕРА, К КОТОРОМУ БУДЕТ ПРИВЯЗАН МОДУЛЬ, НАДО ПОСЛАТЬ КОМАНДУ

    GET=ID

  3. ДЛЯ УСТАНОВКИ ID КОНТРОЛЛЕРА - ПОСЫЛАЕТСЯ КОМАНДА

    SET=ID|ТУТ_ID

    например,

    SET=ID|24


  ПЕРЕД ОТПРАВКОЙ МОДУЛЯ КЛИЕНТУ ЛУЧШЕ ПРИНУДИТЕЛЬНО ВЫПОЛНИТЬ КОМАНДЫ, УСТАНАВЛИВАЮЩИЕ ID КОНТРОЛЛЕРА
  НА ПАРАМЕТРЫ ПО УМОЛЧАНИЮ, А ИМЕННО - ВЫПОЛНИТЬ ДВЕ КОМАНДЫ:

    SET=ID|0 // привязка к контроллеру с номером 0
    SET=ID|255 // пакеты будут принимать все контроллеры (броадкастовый пакет)

*/
//----------------------------------------------------------------------------------------------------------------
#include "UniGlobals.h"
#include "CommandBuffer.h"
#include "CommandParser.h"
#include <OneWire.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include "LoRa.h"

//----------------------------------------------------------------------------------------------------------------
// ПОЛЬЗОВАТЕЛЬСКИЕ НАСТРОЙКИ
//----------------------------------------------------------------------------------------------------------------
#define _DEBUG // раскомментировать для отладочного режима (плюётся в Serial)
//----------------------------------------------------------------------------------------------------------------
#define DEFAULT_CONTROLLER_ID 0       // ID контроллера по умолчанию
#define RADIO_SEND_INTERVAL 5000      // интервал между отсылкой данных по радиоканалу, миллисекунд
#define USE_WATCHDOG                  // использовать или нет внутренний ватчдог
#define WDT_UPDATE_INTERVAL 4000      // интервал сброса сторожевого таймера
//#define USE_RANDOM_SEED_PIN         // закомментировать, если не надо использовать пин для инициализации генератора псевдослучайных чисел
#define RANDOM_SEED_PIN A0            // какой пин (АНАЛОГОВЫЙ !!!) использовать для инициализации генератора псевдослучайных чисел (пин должен быть висящим в воздухе)



//----------------------------------------------------------------------------------------------------------------
// настройки хранения в EEPROM
//----------------------------------------------------------------------------------------------------------------
#define CONTROLLER_ID_ADDRESS   50    // по какому адресу в EEPROM храним ID контроллера (1 байт)
#define DISTANCE_EMPTY_ADDRESS  60    // по какому адресу будет хранится расстояние до воды пустого бака, см (2 байта)
#define DISTANCE_FULL_ADDRESS   70    // по какому адресу будет хранится расстояние до воды полного бака, см (2 байта)
#define MAX_WORKTIME_ADDRESS    80    // по какому адресу хранятся настройки максимального времени наполнения (4 байта)

//----------------------------------------------------------------------------------------------------------------
// настройки расстояния по умолчанию
//----------------------------------------------------------------------------------------------------------------
#define DISTANCE_EMPTY_DEFAULT 150    // расстояние в см до воды пустого бака (по умолчанию, меняется через конфигуратор)
#define DISTANCE_FULL_DEFAULT   50    // расстояние в см до воды полного бака (по умолчанию, меняется через конфигуратор)
#define CRITICAL_MIN_DISTANCE   40    // минимальное расстояние в см измеряемое ультразвуковым датчиком
#define CRITICAL_MAX_DISTANCE  350    // максимальное расстояние в см измеряемое ультразвуковым датчиком
//----------------------------------------------------------------------------------------------------------------
// настройки ультразвукового датчика
//----------------------------------------------------------------------------------------------------------------
#define SENSOR_TRIG_PIN  4           // номер пина TRIG, на котором сидит УЗ-датчик (пин - цифровой!)
#define SENSOR_ECHO_PIN  5           // номер пина ECHO, на котором сидит УЗ-датчик (пин - цифровой!)
#define SENSOR_UPDATE_INTERVAL 3000  // интервал обновления данных с датчика, миллисекунд
//----------------------------------------------------------------------------------------------------------------
// настройки управления клапаном
//----------------------------------------------------------------------------------------------------------------
#define VALVE_PIN         A4         // пин управления клапаном
#define VALVE_PIN_HIGH    3          // пин канцевика верхнего аварийного уровня
#define VALVE_ON_LEVEL    HIGH       // уровень включения клапана
#define MAX_WORK_TIME     600        // время для максимального наполнения, секунд (по умолчанию, будет меняться через конфигуратор) 

//----------------------------------------------------------------------------------------------------------------
// настройки LoRa
//----------------------------------------------------------------------------------------------------------------
#define USE_LORA                    

/*
  LoRa для своей работы занимает следующие пины: 9,10,11,12,13.
  Следите за тем, чтобы номера пинов не пересекались в слотах, или с RS-485, или ещё где.
*/
#define LORA_SS_PIN 8               // пин SS для LoRa
#define LORA_RESET_PIN 7            // пин Reset для LoRa
#define LORA_FREQUENCY 868E6        // частота работы (433E6, 868E6, 915E6)
#define LORA_TX_POWER 17            // мощность передатчика (1 - 17)


//----------------------------------------------------------------------------------------------------------------
// Дальше лазить - неосмотрительно :)
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
// ||
// ||
// \/
//----------------------------------------------------------------------------------------------------------------
// ДАЛЕЕ ИДУТ СЛУЖЕБНЫЕ НАСТРОЙКИ И КОД - МЕНЯТЬ С ПОЛНЫМ ПОНИМАНИЕМ ТОГО, ЧТО ХОДИМ СДЕЛАТЬ !!!
//----------------------------------------------------------------------------------------------------------------
int controllerID           = DEFAULT_CONTROLLER_ID;
uint32_t radioSendInterval = RADIO_SEND_INTERVAL;
uint32_t lastRadioSentAt   = 0;
#ifdef USE_WATCHDOG
uint32_t updateTimerWdt    = 0;  
#endif // USE_WATCHDOG

bool waterTankValveIsOn    = true;                    // флаг включения клапана заполнения бочки
uint32_t maxWorkTime       = 1000ul * MAX_WORK_TIME;  // максимальное время наполнения
uint16_t distanceEmpty     = DISTANCE_EMPTY_DEFAULT;  // расстояние до пустого бака, см
uint16_t distanceFull      = DISTANCE_FULL_DEFAULT;   // расстояние до полного бака, см
uint8_t errorType          = waterTankNoErrors;
uint8_t errorFlag          = 0;
MachineState machineState  = msNormal;
uint32_t fullFillTimer     = 0;
uint32_t sensorUpdateTimer = 0;
uint32_t catchedDistance   = 0;                      // уловленная дистанция до зеркала воды
uint32_t distance_temp     = 0;                      // промежуточные данные дистанции до зеркала воды
bool loRaInited            = false;

CommandBuffer commandBuffer(&Serial);

//----------------------------------------------------------------------------------------------------------------
void initLoRa()
{
#ifdef _DEBUG
  Serial.println(F(" Init LoRa..."));
#endif

  // инициализируем LoRa
  LoRa.setPins(LORA_SS_PIN, LORA_RESET_PIN, -1);
  loRaInited = LoRa.begin(LORA_FREQUENCY);

  if (loRaInited)
  {
    LoRa.setTxPower(LORA_TX_POWER);
    LoRa.sleep(); // засыпаем

#ifdef _DEBUG
    Serial.println(F("LoRa inited."));
#endif

  } // loRaInited
#ifdef _DEBUG
  else
  {
    Serial.println(F("LoRa init FAIL!"));
  }
#endif


}

//----------------------------------------------------------------------------------------------------------------
void turnValve(bool on)         // включение или выключение клапана
{
  if (waterTankValveIsOn != on)
  {
#ifdef _DEBUG
    Serial.print(F("Turn valve to: "));
    Serial.println(on ? F("ON") : F("OFF"));
#endif

    waterTankValveIsOn = on;
    digitalWrite(VALVE_PIN, on ? VALVE_ON_LEVEL : !(VALVE_ON_LEVEL));
  }
}
//----------------------------------------------------------------------------------------------------------------
bool canWork()                // проверяем, можем ли мы работать в штатном режиме?
{
  return (errorFlag == 0);    // можем работать только тогда, когда нет флага ошибки
}
//----------------------------------------------------------------------------------------------------------------
void updateSensor(bool immediate)
{
  //обновление данных с датчика.
  // если сигнал не получен - выставляем ошибку waterTankFullSensorError если клапан включён, и ошибку waterTankBottomSensorFailure - если клапан выключен (флаг waterTankValveIsOn)
  // если параметр immediate выставлен в true, то обновляем немедленно, иначе - по таймеру

  if (!immediate)
  {
    if (!(millis() - sensorUpdateTimer >= SENSOR_UPDATE_INTERVAL))
    {
      return; // не вышло время обновления
    }
  }

  digitalWrite(SENSOR_TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(SENSOR_TRIG_PIN, HIGH);
  delayMicroseconds(15);
  digitalWrite(SENSOR_TRIG_PIN, LOW);

  unsigned long duration = pulseIn(SENSOR_ECHO_PIN, HIGH);
  bool level_high = digitalRead(VALVE_PIN_HIGH);

  catchedDistance = 0;                          // считаем, что не получили расстояние

  if (duration < 50 || level_high == LOW )      // не уловили расстояние, ошибка!
  {
    errorFlag = 1;
    if (waterTankValveIsOn)
    {
      errorType = waterTankFullSensorError;
    }
    else
    {
      errorType = waterTankBottomSensorFailure;
    }
  }
  else // нет ошибок, сбрасываем флаг ошибки, высчитываем расстояние
  {
    errorFlag = 0;
    errorType = waterTankNoErrors;
    catchedDistance = duration/29/2;    // высчитываем расстояние в сантиметрах
#ifdef _DEBUG
	float cm = (duration / 2) / 29.1;
    Serial.print("Расстояние до объекта: ");
    Serial.print(cm,1);
    Serial.println(" см.");
#endif

  }


  sensorUpdateTimer = millis();
}

//----------------------------------------------------------------------------------------------------------------
void sendDataViaLoRa()
{
  if (!loRaInited) 
  {
#ifdef _DEBUG
    Serial.println(F("LoRa not inited!"));
#endif
    return;
  }

#ifdef _DEBUG
  Serial.println(F("Send sensors data via LoRa..."));
#endif

  bool sendDone = false;

  UniRawScratchpad scratchpadS;
  scratchpadS.head.packet_type   = uniWaterTankClient;
  scratchpadS.head.controller_id = controllerID;

  WaterTankDataPacket* dataPacket = (WaterTankDataPacket*) & (scratchpadS.data);

  // заполняем пакет данными
  fillDataPacket(dataPacket);

  // подсчитываем контрольную сумму
  scratchpadS.crc8 = OneWire::crc8((const byte*)&scratchpadS, sizeof(scratchpadS) - 1);

  for (int i = 0; i < 5; i++)                              // пытаемся послать 5 раз
  {
    LoRa.beginPacket();
    LoRa.write((byte*)&scratchpadS, sizeof(scratchpadS));  // пишем в эфир
    if (LoRa.endPacket())                                  // пишем в него
    {
      sendDone = true;
      break;
    }
    else
    {
      delay(random(10));
    }
  } // for

  if (!sendDone)
  {
#ifdef _DEBUG
    Serial.println(F("NO RECEIVING SIDE FOUND!"));
#endif
  }
  else
  {
#ifdef _DEBUG
    Serial.println(F("Sensors data sent."));
#endif
  }

  // рандомная задержка
  delay(random(50));
  LoRa.sleep(); // засыпаем
}

//----------------------------------------------------------------------------------------------------------------
void processSettingsPacket(uint16_t _distanceEmpty, uint16_t _distanceFull, uint32_t _maxWorkTime)
{
#ifdef _DEBUG
  Serial.println(F("SETTINGS RECEIVED!"));
#endif

  uint16_t oldDistanceEmpty = distanceEmpty;
  distanceEmpty = _distanceEmpty;

  if (distanceEmpty != oldDistanceEmpty)
  {
    EEPROM.put(DISTANCE_EMPTY_ADDRESS, distanceEmpty);
  }

  uint16_t oldDistanceFull = distanceFull;
  distanceFull = _distanceFull;

  if (distanceFull != oldDistanceFull)
  {
    EEPROM.put(DISTANCE_FULL_ADDRESS, distanceFull);
  }

  uint32_t newWorkTime = 1000ul * _maxWorkTime;

  if (newWorkTime != maxWorkTime)
  {
    maxWorkTime = newWorkTime;
    EEPROM.put(MAX_WORKTIME_ADDRESS, maxWorkTime);
  }

#ifdef _DEBUG
  Serial.println("*****SettingsPacket********");
  Serial.print("controllerID = ");
  Serial.println(controllerID);
  Serial.print("maxWorkTime = ");
  Serial.println(maxWorkTime);
  Serial.print("distanceEmpty = ");
  Serial.println(distanceEmpty);
  Serial.print("distanceFull = ");
  Serial.println(distanceFull);
#endif
  
  //TODO: ТУТ УСТАНОВКА НОВЫХ НАСТРОЕК!
}

//----------------------------------------------------------------------------------------------------------------
void processSettingsPacket(RS485WaterTankSettingsPacket* packet)
{
  processSettingsPacket(packet->distanceEmpty, packet->distanceFull, packet->maxWorkTime);
}
//----------------------------------------------------------------------------------------------------------------
void processSettingsPacket(NRFWaterTankSettingsPacket* packet)
{
  processSettingsPacket(packet->distanceEmpty, packet->distanceFull, packet->maxWorkTime);
}
//----------------------------------------------------------------------------------------------------------------
void processCommandPacket(uint8_t valveCommand)
{
#ifdef _DEBUG
  Serial.print(F("RECEIVED WATER TANK COMMAND! VALVE SHOULD BE: ["));
  if (valveCommand)
  {
    Serial.print(F("ON"));
  }
  else
  {
    Serial.print(F("OFF"));
  }
  Serial.println(F("]."));

#endif

  bool canProcess = canWork(); // проверяем, можем ли мы работать (не выставлен ли флаг ошибки?)

  if (!canProcess)
  {
    // почему-то работать не можем - попробуем обновить данные с датчика, вдруг заработает?
    updateSensor(true);
    canProcess = canWork(); // проверяем ещё раз - получилось прочитать дистанцию до воды?
  }

  if (canProcess)
  {
    // можем работать, флага ошибки - не выставлено.

    bool requestedWalveState = valveCommand > 0;

    if (waterTankValveIsOn != requestedWalveState)
    {
      // попросили изменить состояние клапана.
      // здесь мы должны учитывать, что при включении/выключении клапана - мы должны переместиться
      // на нужную ветку конечного автомата
      if (requestedWalveState)
      {
        // попросили включиться
        turnValve(true);
        fullFillTimer = millis();
        machineState = msWaitForTankFullfilled;
      }
      else
      {
        // попросили выключиться
        turnValve(false);
        machineState = msNormal;
      }
    }
  } // canWork
}
//----------------------------------------------------------------------------------------------------------------
void processCommandPacket(RS485WaterTankCommandPacket* packet)
{
  processCommandPacket(packet->valveCommand);
}
//----------------------------------------------------------------------------------------------------------------
void processCommandPacket(NRFWaterTankExecutionPacket* packet)
{
  processCommandPacket(packet->valveCommand);
}

//----------------------------------------------------------------------------------------------------------------
void processIncomingLoRaPackets()
{
  // обрабатываем входящие данные по LoRa
  if (!loRaInited)
  {
    return;
  }

  static NRFWaterTankExecutionPacket nrfPacket; // наш пакет, в который мы принимаем данные с контроллера
  int packetSize = LoRa.parsePacket();
  if (packetSize >= sizeof(NRFWaterTankExecutionPacket))
  {
    memset(&nrfPacket, 0, sizeof(NRFWaterTankExecutionPacket));
    LoRa.readBytes((byte*)&nrfPacket, sizeof(NRFWaterTankExecutionPacket));

    if (nrfPacket.controller_id == controllerID)
    {
      // это пакет с нашего контроллера пришёл, обновляем данные
      byte checksum = OneWire::crc8((const byte*) &nrfPacket, sizeof(NRFWaterTankExecutionPacket) - 1);

      if (checksum == nrfPacket.crc8) // чексумма сошлась
      {
        switch (nrfPacket.packetType)
        {
          case RS485WaterTankCommands: // пакет с командами для модуля контроля бака воды
            {
              processCommandPacket(&nrfPacket);
            }
            break; // RS485WaterTankCommands

          case RS485WaterTankSettings:
            {
              processSettingsPacket((NRFWaterTankSettingsPacket*) &nrfPacket);
            }
            break; // RS485WaterTankSettings
        } // switch
      } //  // good checksum
    }
  }
}

//----------------------------------------------------------------------------------------------------------------
void SetControllerID(uint8_t id)
{
  controllerID = id;
  EEPROM.write(CONTROLLER_ID_ADDRESS, controllerID);
}
//----------------------------------------------------------------------------------------------------------------
void readROM()
{
  controllerID = EEPROM.read(CONTROLLER_ID_ADDRESS);
  if (controllerID == 0xFF)
  {
    controllerID = DEFAULT_CONTROLLER_ID;
  }

  EEPROM.get(MAX_WORKTIME_ADDRESS, maxWorkTime);
  if (maxWorkTime == 0xFFFFFFFF)
  {
    maxWorkTime = 1000ul * MAX_WORK_TIME;          // максимальное время наполнения
  }

  EEPROM.get(DISTANCE_EMPTY_ADDRESS, distanceEmpty);
  if (distanceEmpty == 0xFFFF)
  {
    distanceEmpty = DISTANCE_EMPTY_DEFAULT;       // по умолчанию
  }

  EEPROM.get(DISTANCE_FULL_ADDRESS, distanceFull);
  if (distanceFull == 0xFFFF)
  {
    distanceFull = DISTANCE_FULL_DEFAULT;        // по умолчанию
  }

#ifdef _DEBUG
  Serial.println("readROM");
  Serial.print("controllerID = ");
  Serial.println(controllerID);
  Serial.print("maxWorkTime = ");
  Serial.println(maxWorkTime);
  Serial.print("distanceEmpty = ");
  Serial.println(distanceEmpty);
  Serial.print("distanceFull = ");
  Serial.println(distanceFull);
#endif

}
//----------------------------------------------------------------------------------------------------------------
void setupSensor()
{
#ifdef _DEBUG
  Serial.println(F("Setup level sensor..."));
#endif

  pinMode(SENSOR_TRIG_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_PIN, INPUT);

}
//----------------------------------------------------------------------------------------------------------------
uint8_t getFillStatus() // возвращает процент заполнения (0-100%)
{

	// catchedDistance -  расстояние в сантиметрах
	// пропорция: distanceEmpty = 0%, distanceFull = 100%, catchedDistance = х%
	//return map(catchedDistance, distanceFull, distanceEmpty, 0, 100);

	int val = 0;

	if (catchedDistance < CRITICAL_MIN_DISTANCE || catchedDistance > CRITICAL_MAX_DISTANCE) // нет данных по расстоянию до зеркала воды
	{
 
	#ifdef _DEBUG
		Serial.print("Данные вне диапазона = ");
		Serial.println(catchedDistance);
	#endif
	return 0;
	}

	//в distanceEmpty - расстояние до зеркала воды пустого бака
	//в distanceFull - расстояние до зеркала воды полного бака
	//   пропорция: distanceEmpty = 0%, distanceFull = 100%, catchedDistance = х%
	 

	if (catchedDistance <= distanceFull) // полный бак
	{
		val = 100;
	#ifdef _DEBUG
		Serial.print("distanceFull = ");
		Serial.println(distanceFull);
		Serial.print("distanceEmpty = ");
		Serial.println(distanceEmpty);
		Serial.print(" Дистанция см. - ");
		Serial.print(catchedDistance);
		Serial.print("   Заполнение %  - ");
		Serial.println(val);
	#endif
		return 100;
	}

	if (catchedDistance >= distanceEmpty) // пустой бак
	{
		val = 0;
	#ifdef _DEBUG
		Serial.print("distanceFull = ");
		Serial.println(distanceFull);
		Serial.print("distanceEmpty = ");
		Serial.println(distanceEmpty);
		Serial.print(" Дистанция см. - ");
		Serial.print(catchedDistance);
		Serial.print("   Заполнение %  - ");
		Serial.println(val);
	#endif
		return 0;
	}


	val = map(catchedDistance, distanceFull, distanceEmpty, 100, 0);

#ifdef _DEBUG
    Serial.print("distanceFull = ");
	Serial.println(distanceFull);
	Serial.print("distanceEmpty = ");
	Serial.println(distanceEmpty);
	Serial.print(" Дистанция см. - ");
	Serial.print(catchedDistance);
	Serial.print("   Заполнение %  - ");
	Serial.println(val);
#endif

	return val;
}
//----------------------------------------------------------------------------------------------------------------
void fillDataPacket(WaterTankDataPacket* packet)
{
  packet->valveState = waterTankValveIsOn;
  packet->errorFlag = errorFlag;
  packet->errorType = errorType;
  packet->fillStatus = getFillStatus();

#ifdef _DEBUG
  Serial.print(F("Fill status is: "));
  Serial.println(packet->fillStatus);
#endif
  // пакет с данными заполнен и готов к отправке по радиоканалу
}

//----------------------------------------------------------------------------------------------------------------
void setup()
{
  bool serialStarted = false;

#ifdef _DEBUG
  Serial.begin(57600);
  Serial.println(F("DEBUG MODE!"));
  serialStarted = true;
#endif


#ifdef USE_RANDOM_SEED_PIN
  randomSeed(analogRead(RANDOM_SEED_PIN));
#endif

  pinMode(VALVE_PIN, OUTPUT);
  pinMode(VALVE_PIN_HIGH, INPUT);
  

  radioSendInterval = RADIO_SEND_INTERVAL + random(100);

  if (!serialStarted)
  {
    Serial.begin(57600);
    serialStarted = true;
  }

  readROM();              // читаем настройки
  setupSensor();          // настраиваем датчик
#ifdef USE_LORA
  initLoRa();             // поднимаем радиоканал
#endif // USE_LORA

  turnValve(false);       // выключаем клапан при старте

  delay(1000);            // подождём секунду, мало ли.
  updateSensor(true);     // сразу получаем данные при старте
#ifdef USE_WATCHDOG
  delay(5000);            // Задержка, чтобы было время перепрошить устройство в случае bootloop
  wdt_enable(WDTO_8S);    // Для тестов не рекомендуется устанавливать значение менее 8 сек.
#endif // USE_WATCHDOG

}

//----------------------------------------------------------------------------------------------------------------
void ProcessIDCommand(Command& cmd)
{

  size_t argsCount = cmd.GetArgsCount();
  if (cmd.GetType() == ctGET)
  {
    // GET

    Serial.print("OK=");
    Serial.println(controllerID);
    return;

  }
  else
  {
    // SET
    if (argsCount > 0)
    {
      uint8_t id = atoi(cmd.GetArg(0));
      SetControllerID(id);
      Serial.print("OK=");
      Serial.println("ADDED");
      return;
    }
  }

  Serial.print("ER=");
  Serial.println("UNKNOWN_COMMAND");
}
//----------------------------------------------------------------------------------------------------------------
void ProcessIncomingCommand(String& cmd)
{

  CommandParser cParser;
  Command parsed;
  if (cParser.ParseCommand(cmd, parsed))
  {

    String module = parsed.GetTargetModuleID();
    if (module == F("ID"))
    {
      ProcessIDCommand(parsed);
    }
  }
}
//----------------------------------------------------------------------------------------------------------------
void loop()
{

  if (canWork()) // можем работать, флага ошибки - не выставлено.
  {
    updateSensor(false);    // обновляем данные с датчика
  } // canWork
  else
  {
    // работать не можем, выставлен флаг ошибки, выключаем клапан
    turnValve(false);
  } // else


#ifdef USE_WATCHDOG
  if (millis() - updateTimerWdt > WDT_UPDATE_INTERVAL)
  {
    updateTimerWdt = millis();
    wdt_reset();
  }
#endif // USE_WATCHDOG

#if defined(USE_LORA) && !defined(USE_RS485_GATE)
  if (commandBuffer.HasCommand())
  {
    String cmd = commandBuffer.GetCommand();
    commandBuffer.ClearCommand();
    ProcessIncomingCommand(cmd);
  }
#endif

  processIncomingLoRaPackets();

  if (millis() - lastRadioSentAt >= radioSendInterval)
  {
    sendDataViaLoRa();
    lastRadioSentAt = millis();
    radioSendInterval = RADIO_SEND_INTERVAL + random(100);
  }

  // тут можно проверять состояния конечного автомата
  if (canWork())
  {
    // можем работать, смотрим, в какой ветке конечного автомата мы находимся
    switch (machineState)
    {
      case msNormal:
        {
          // нормальное состояние, проверяем на критический уровень
          if (catchedDistance > 0) // есть показания с датчика
          {
            if (catchedDistance >= distanceEmpty-50) // очень далеко до зеркала воды
            {
				#ifdef _DEBUG
					Serial.println(F("Critical level detected, turn valve ON!"));
				#endif

              // расстояние до воды очень большое - надо открывать клапан
              turnValve(true);

              // включили таймер, перешли на другую ветку
              fullFillTimer = millis();
              machineState = msWaitForTankFullfilled;

            } // if(catchedDistance >= distanceEmpty)

          } // if(catchedDistance > 0)

        }
        break; // msNormal

      case msWaitForTankFullfilled:
        {
          // ждём наполнения бака в течение максимального времени
          if (catchedDistance > 0) // есть показания с датчика
          {
            if (catchedDistance <= distanceFull) // уровень выше, чем максимальный - надо выключать клапан
            {
				#ifdef _DEBUG
					Serial.println(F("Tank is full, turn valve OFF!"));
				#endif

              // датчик верхнего уровня показывает воду, выключаем клапан, переключаемся в нормальный режим
              turnValve(false);
              machineState = msNormal;
            }
            else
            {
              // датчик всё ещё не показывает минимальное расстояние до воды, смотрим - не истекло ли время ожидания наполнения

//#ifdef _DEBUG
//				Serial.print(F("Full timer = "));
//				Serial.println(millis() - fullFillTimer);
//#endif


              if (millis() - fullFillTimer >= maxWorkTime)
              {
				#ifdef _DEBUG
					Serial.println(F("Full sensor ERROR detected!"));
				#endif

                // истекло время максимальной работы, это ошибка датчика !!!

                // взводим флаг ошибки
                errorFlag = 1;
                errorType = waterTankFullSensorError;

                turnValve(false); // выключаем клапан
                machineState = msFillTankError;

              }
            }
          } // if(catchedDistance > 0) // есть показания с датчика
          else
          {
            // нет показаний с датчика, выключаем клапан!

#ifdef _DEBUG
            Serial.println(F("Sensor data ERROR detected!"));
#endif

            // взводим флаг ошибки
            errorFlag = 1;
            errorType = waterTankFullSensorError;

            turnValve(false);
			distance_temp = catchedDistance;
            machineState = msFillTankError;

          } // else
        }
        break; // msWaitForTankFullfilled

      case msFillTankError: // бак не заполнялся в течение длительного времени
        {

#ifdef _DEBUG
		  Serial.print(F("****** Sensor data temp - "));
		  Serial.println(distance_temp);
#endif


        }
        break; // msFillTankError

    } // switch

  } // canWork
  else
  {
    // работать не можем, был взведён флаг ошибки

    turnValve(false); // выключаем клапан

    // тут переведение конечного автомата в исходную ветку
    machineState = msNormal;

  } // else
}
//----------------------------------------------------------------------------------------------------------------
void yield()
{

}
//----------------------------------------------------------------------------------------------------------------
