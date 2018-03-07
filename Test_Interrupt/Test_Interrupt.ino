int pin = 13;
volatile int state = LOW;
volatile uint32_t list_time[100];
volatile int count = 0;
volatile bool interrupt_on = false;
static uint32_t  time_x = 900000;

//static uint32_t last = 0;
//if(micros() - last >50000) {last = micros(); Serial.print("!!!!");}


void blink()
{
  list_time[count] = micros();              // Записать время прерывания в базу
  count++;                                  // Увеличить счетчик прерываний
  if (count > 99) count = 0;                // Ограничим количество импульсов
  interrupt_on = true;                      // Выставляем флаг - Прерывание зарегистрировано
  state = !state;                           // Проверим светодиодом
}

void setup()
{
  Serial.begin(115200);
  //  while (!Serial);
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(7), blink, RISING);
}

void loop()
{
  digitalWrite(pin, state);                                       // Отобразим факт прерывания светодиодом
  if (interrupt_on)                                               // Проверяем было ли прерывание
  {
  //  delay(5);
    delayMicroseconds(20000);

    uint32_t last = micros();                                     // Получить текущее время. !!! Что то не так фиксируется текущее время
    if ((micros() - list_time[count - 1]) > time_x )              // Проверим не превышен ли предел ожидания импульса. Импульсы закончились?
    {
      interrupt_on = false;                                       //  Данные получены, ожидаем новые
      Serial.println(last);
      Serial.println(list_time[count - 1]);
      Serial.println(last - list_time[count - 1]);
      Serial.println(count);
      for (int i = 0; i < count - 2; i++)
      {
        Serial.print(i + 1);
        Serial.print(" - ");
        Serial.println((list_time[i + 1] - list_time[i])/1000);
      }
      Serial.println("*************");
     // interrupt_on = false;                                         // Данные получены, ожидаем новые. Вывод текущих данных блокируем. Флаг прерывания в исходное положение.
      count = 0;                                                    // Счетчик импульсов в исходное состояние
     }
  }
}


