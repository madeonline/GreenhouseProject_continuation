
#include <SPI.h>
#include <LoRa.h>

#define Serial SERIAL_PORT_USBVIRTUAL

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// генератор сигнала 50 Гц на трёх каналах со сдвигом фаз в 120 градусов
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "CorePinScenario.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DURATION 9875 // мкс между состояниями дадут нам меандр с частотой, близкой к 50 Гц
#define PHASE_SHIFT (DURATION/3) // сдвиг угла одной фазы относительно другой
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define LINE1 A4 // номер пина для линии 1
#define LINE2 A3 // номер пина для линии 2
#define LINE3 A2 // номер пина для линии 3
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePinScenario scene1;
CorePinScenario scene2;
CorePinScenario scene3;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PULSE_PIN1 A1 // номер пина для генерации тестовых импульсов индуктивного датчика №1
#define PULSE_PIN2 A0 // номер пина для генерации тестовых импульсов индуктивного датчика №2
#define PULSE_PIN3 A5 // номер пина для генерации тестовых импульсов индуктивного датчика №3

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePinScenario pulseScene1;
CorePinScenario pulseScene2;
CorePinScenario pulseScene3;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  // while (!Serial);
  LoRa.setPins(10, 9, 2);       // Только для  ATMEL_SAMD21
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  pinMode(LINE1, OUTPUT);
  pinMode(LINE2, OUTPUT);
  pinMode(LINE3, OUTPUT);

  pinMode(PULSE_PIN1, OUTPUT);
  pinMode(PULSE_PIN2, OUTPUT);
  pinMode(PULSE_PIN3, OUTPUT);


  scene1.add({LINE1, HIGH, DURATION});
  scene1.add({LINE1, LOW, DURATION});


  scene2.add({LINE2, HIGH, DURATION});
  scene2.add({LINE2, LOW, DURATION});

  scene3.add({LINE3, HIGH, DURATION});
  scene3.add({LINE3, LOW, DURATION});

  // добавляем тестовые импульсы


  unsigned long duration = 40000;
  uint8_t level = HIGH;
  // сперва добавляем 10 импульсов по убыванию, от 20 мс до 2 мс, с шагом 2 мс

  for (int i = 0; i < 10; i++, duration -= 2000)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }
  for (int i = 0; i < 10; i++, duration -= 1000)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }

  for (int i = 0; i < 15; i++)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }

  // затем добавляем 15 импульсов по возрастанию
  for (int i = 0; i < 14; i++, duration += 1000)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }
  // затем добавляем 16 импульсов по возрастанию
  for (int i = 0; i < 15; i++, duration += 1600)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }


  duration = 40000;
  level = HIGH;
  // сперва добавляем 10 импульсов по убыванию, от 20 мс до 2 мс, с шагом 2 мс

  for (int i = 0; i < 20; i++, duration -= 1500)
  {
    pulseScene2.add({PULSE_PIN2, level, duration});
    level = !level;
  }

  for (int i = 0; i < 16; i++, duration -= 625)
  {
    pulseScene2.add({PULSE_PIN2, level, duration});
    level = !level;
  }

  // затем добавляем 15 импульсов по возрастанию
  for (int i = 0; i < 10; i++, duration += 400)
  {
    pulseScene2.add({PULSE_PIN2, level, duration});
    level = !level;
  }
  // затем добавляем 5 импульсов по возрастанию
  for (int i = 0; i < 14; i++, duration += 2800)
  {
    pulseScene2.add({PULSE_PIN2, level, duration});
    level = !level;
  }



  duration = 40000;
  level = HIGH;
  // сперва добавляем 10 импульсов по убыванию, от 20 мс до 2 мс, с шагом 2 мс

  for (int i = 0; i < 10; i++, duration -= 1500)
  {
    pulseScene3.add({PULSE_PIN3, level, duration});
    level = !level;
  }
  for (int i = 0; i < 15; i++, duration -= 1200)
  {
    pulseScene3.add({PULSE_PIN3, level, duration});
    level = !level;
  }

  for (int i = 0; i < 10; i++)
  {
    pulseScene3.add({PULSE_PIN3, level, duration});
    level = !level;
  }

  // затем добавляем 15 импульсов по возрастанию
  for (int i = 0; i < 15; i++, duration += 1000)
  {
    pulseScene3.add({PULSE_PIN3, level, duration});
    level = !level;
  }
  // затем добавляем 5 импульсов по возрастанию
  for (int i = 0; i < 15; i++, duration += 1600)
  {
    pulseScene3.add({PULSE_PIN3, level, duration});
    level = !level;
  }


  // добавляем паузу в 5 секунд
  pulseScene1.add({PULSE_PIN1, LOW, 5000000});
  pulseScene2.add({PULSE_PIN2, LOW, 5000000});
  pulseScene3.add({PULSE_PIN3, LOW, 5000000});
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{

  // сдвигаем фазы при старте

  delay(1);
  scene1.update();
  delayMicroseconds(PHASE_SHIFT);
  scene2.update();
  delayMicroseconds(PHASE_SHIFT);
  scene3.update();

  while (1)
  {
    scene1.update();
    scene2.update();
    scene3.update();

    pulseScene1.update();
    pulseScene2.update();
    pulseScene3.update();

    // try to parse packet
    int packetSize = LoRa.parsePacket();
    //  Serial.println(packetSize);
    if (packetSize) {
      // received a packet
      Serial.print("Received packet '");

      // read packet
      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
      }

      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
