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
#define SIGNAL_PIN 3 // номер пина, на который будет подаваться высокий уровень нужное кол-во микросекунд (имитация срабатывания реле)
#define SIGNAL_PIN_DURATION 5000 // длительность импульса, микросекунд
uint32_t SIGNAL_PIN_WAIT_ARTER_RAISE = 5000; // сколько микросекунд ждать после импульса реле до начала выдачи пачек импульсов прерываний

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePinScenario pulseScene1;
CorePinScenario pulseScene2;
CorePinScenario pulseScene3;
CorePinScenario pulseSceneLed;
//bool onIdleTimer = false;
uint32_t timer = 0;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const int ledPin = LED_BUILTIN;    // the number of the LED pin

#define Serial SERIAL_PORT_USBVIRTUAL

void setup()
{
  Serial.begin(115200);
  pinMode(LINE1, OUTPUT);
  pinMode(LINE2, OUTPUT);
  pinMode(LINE3, OUTPUT);

  pinMode(PULSE_PIN1, OUTPUT);
  pinMode(PULSE_PIN2, OUTPUT);
  pinMode(PULSE_PIN3, OUTPUT);

  pinMode(ledPin, OUTPUT);

  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);


  scene1.add({LINE1, HIGH, DURATION});
  scene1.add({LINE1, LOW, DURATION});


  scene2.add({LINE2, HIGH, DURATION});
  scene2.add({LINE2, LOW, DURATION});

  scene3.add({LINE3, HIGH, DURATION});
  scene3.add({LINE3, LOW, DURATION});

  // добавляем тестовые импульсы
  pulseScene1.setLoop(false);
  pulseScene2.setLoop(false);
  pulseScene3.setLoop(false);


  unsigned long duration = 40000;
  uint8_t level = HIGH;

  // Формируем 30 импульса первого графика прерывания. Желтый цвет графика.
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

  for (int i = 0; i < 10; i++, duration -= 100)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }

  // затем добавляем 10 импульсов по возрастанию
  for (int i = 0; i < 10; i++, duration += 1000)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }
  // затем добавляем 15 импульсов по возрастанию
  for (int i = 0; i < 15; i++, duration += 1600)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }
  // затем добавляем 5 импульсов по возрастанию
  for (int i = 0; i < 5; i++, duration += 600)
  {
    pulseScene1.add({PULSE_PIN1, level, duration});
    level = !level;
  }

  duration = 40000;
  level = HIGH;

  // Формируем 30 импульса второго графика прерывания. Синий график.
  // сперва добавляем 20 импульсов по убыванию, от 20 мс до 2 мс, с шагом 1,5 мс

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

  // Формируем 30 импульса второго графика прерывания. Красный график.
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
  // затем добавляем 10 импульсов по возрастанию
  for (int i = 0; i < 10; i++, duration += 1600)
  {
    pulseScene3.add({PULSE_PIN3, level, duration});
    level = !level;
  }

  duration = 500000;
  level = HIGH;
  pulseSceneLed.add({ledPin, level, duration });

  // добавляем установку низкого уровня
  pulseScene1.add({PULSE_PIN1, LOW, 10000});
  pulseScene2.add({PULSE_PIN2, LOW, 10000});
  pulseScene3.add({PULSE_PIN3, LOW, 10000});

  // добавляем паузу в 20 секунд
  pulseSceneLed.add({ledPin, LOW, 20000000 });

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  msNormal,
  msOnIdleTimer,
  msOnRelayImpulse,
  msWaitRelayDone,

} MachineState;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MachineState state = msNormal;
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

    pulseSceneLed.update();


    static bool bFirst = true;

    if (!bFirst)
    {
      pulseScene1.update();
      pulseScene2.update();
      pulseScene3.update();
    }

    if (bFirst || (pulseScene1.isDone() && pulseScene2.isDone() && pulseScene3.isDone()) )
    {
      // сцены закончились
      switch (state)
      {
        case msNormal:
          {
            state = msOnIdleTimer;
            timer = micros();
          }
          break;

        case msOnIdleTimer:
          {
            if (micros() - timer > 20000000)
            {
              state = msOnRelayImpulse;

              digitalWrite(SIGNAL_PIN, HIGH);

              timer = micros();
              SIGNAL_PIN_WAIT_ARTER_RAISE = random(5000, 30000);
              Serial.println(SIGNAL_PIN_WAIT_ARTER_RAISE);
            }
          }
          break;

        case msOnRelayImpulse:
          {
            if (micros() - timer > SIGNAL_PIN_DURATION)
            {
              digitalWrite(SIGNAL_PIN, LOW);

              timer = micros();

              state = msWaitRelayDone;

            }
          }
          break;

        case msWaitRelayDone:
          {
            if (micros() - timer > SIGNAL_PIN_WAIT_ARTER_RAISE)
            {
              state = msNormal;
              pulseScene1.reset();
              pulseScene2.reset();
              pulseScene3.reset();

              bFirst = false;
            }
          }
          break;

      } // switch

      /*
        if(!onIdleTimer)
        {
            onIdleTimer = true;
            timer = micros();
        }
        else
        {
            if(micros() - timer > 20000000)
            {
              onIdleTimer = false;
              pulseScene1.reset();
              pulseScene2.reset();
              pulseScene3.reset();
            }
        }
      */
    }

  } // while(1)
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
