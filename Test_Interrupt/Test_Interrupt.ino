int pin = 13;
volatile int state = LOW;
volatile uint32_t list_time1[100];
volatile int count1 = 0;
volatile uint32_t list_time2[100];
volatile int count2 = 0;
volatile uint32_t list_time3[100];
volatile int count3 = 0;
volatile bool interrupt_on1 = false;
volatile bool interrupt_on2 = false;
volatile bool interrupt_on3 = false;

static uint32_t  time_x = 900000;
int serial_count = 0;

//static uint32_t last = 0;
//if(micros() - last >50000) {last = micros(); Serial.print("!!!!");}

#define INTERRUPT1_PIN 7
#define INTERRUPT2_PIN 8
#define INTERRUPT3_PIN 35

void blink1()
{
  list_time1[count1] = micros();                // Записать время прерывания в базу
  count1++;                                     // Увеличить счетчик прерываний
//  if (count1 > 99) count1 = 0;                  // Ограничим количество импульсов
  interrupt_on1 = true;                         // Выставляем флаг - Прерывание зарегистрировано
  //  state = !state;                           // Проверим светодиодом
}

void blink2()
{
  list_time2[count2] = micros();               // Записать время прерывания в базу
  count2++;                                    // Увеличить счетчик прерываний
//  if (count2 > 99) count2 = 0;                 // Ограничим количество импульсов
  interrupt_on2 = true;                        // Выставляем флаг - Прерывание зарегистрировано
  //  state = !state;                          // Проверим светодиодом
}

void blink3()
{
  list_time3[count3] = micros();              // Записать время прерывания в базу
  count3++;                                   // Увеличить счетчик прерываний
//  if (count3 > 99) count3 = 0;                // Ограничим количество импульсов
  interrupt_on3 = true;                       // Выставляем флаг - Прерывание зарегистрировано
  //  state = !state;                         // Проверим светодиодом
}

void setup()
{
  Serial.begin(115200);
  //  while (!Serial);
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT1_PIN), blink1, RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT2_PIN), blink2, RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT3_PIN), blink3, RISING);
}

void loop()
{
  //  digitalWrite(pin, state);                                       // Отобразим факт прерывания светодиодом
  if (interrupt_on1)                                               // Проверяем было ли прерывание
  {
  //  noInterrupts();
    int thisCount1 = count1;

    uint32_t last1 = micros();                                     // Получить текущее время. !!! Что то не так фиксируется текущее время
    if ((micros() - list_time1[thisCount1 - 1]) > time_x )         // Проверим не превышен ли предел ожидания импульса. Импульсы закончились?
    {
      interrupt_on1 = false;                                       //  Данные получены, ожидаем новые
      serial_count++;
      Serial.print("Send - ");
      Serial.println(serial_count);
      Serial.println("Channel 1");
      Serial.println(last1);
      Serial.println(list_time1[thisCount1 - 1]);
      Serial.println(last1 - list_time1[thisCount1 - 1]);
      Serial.println(thisCount1);
      for (int i = 0; i < thisCount1 - 2; i++)
      {
        Serial.print(i + 1);
        Serial.print(" - ");
        Serial.println((list_time1[i + 1] - list_time1[i]) / 1000);
      }
      Serial.println("*************");
      count1 = 0;                                                    // Счетчик импульсов в исходное состояние
    }
   // interrupts();
  }

  if (interrupt_on2)                                                 // Проверяем было ли прерывание
  {
 //   noInterrupts();
    int thisCount2 = count2;
    uint32_t last2 = micros();                                     // Получить текущее время. !!! Что то не так фиксируется текущее время
    if ((micros() - list_time2[thisCount2 - 1]) > time_x )              // Проверим не превышен ли предел ожидания импульса. Импульсы закончились?
    {
      interrupt_on2 = false;                                       //  Данные получены, ожидаем новые
      serial_count++;
      Serial.print("Send - ");
      Serial.println(serial_count);
      Serial.println("Channel 2");
      Serial.println(last2);
      Serial.println(list_time2[thisCount2 - 1]);
      Serial.println(last2 - list_time2[thisCount2 - 1]);
      Serial.println(thisCount2);
      for (int i = 0; i < thisCount2 - 2; i++)
      {
        Serial.print(i + 1);
        Serial.print(" - ");
        Serial.println((list_time2[i + 1] - list_time2[i]) / 1000);
      }
      Serial.println("*************");
      // interrupt_on = false;                                         // Данные получены, ожидаем новые. Вывод текущих данных блокируем. Флаг прерывания в исходное положение.
      count2 = 0;                                                    // Счетчик импульсов в исходное состояние
    }
  //  interrupts();
  }

  if (interrupt_on3)                                               // Проверяем было ли прерывание
  {

    int thisCount3 = count3;

    uint32_t last = micros();                                     // Получить текущее время. !!! Что то не так фиксируется текущее время
    if ((micros() - list_time3[thisCount3 - 1]) > time_x )              // Проверим не превышен ли предел ожидания импульса. Импульсы закончились?
    {
      interrupt_on3 = false;                                       //  Данные получены, ожидаем новые
      serial_count++;
      Serial.print("Send - ");
      Serial.println(serial_count);
      Serial.println("Channel 3");
      Serial.println(last);
      Serial.println(list_time3[thisCount3 - 1]);
      Serial.println(last - list_time3[thisCount3 - 1]);
      Serial.println(thisCount3);
      for (int i = 0; i < thisCount3 - 2; i++)
      {
        Serial.print(i + 1);
        Serial.print(" - ");
        Serial.println((list_time3[i + 1] - list_time3[i]) / 1000);
      }
      Serial.println("*************");
      // interrupt_on = false;                                         // Данные получены, ожидаем новые. Вывод текущих данных блокируем. Флаг прерывания в исходное положение.
      count3 = 0;                                                    // Счетчик импульсов в исходное состояние
    }
   // interrupts();
  }
}


