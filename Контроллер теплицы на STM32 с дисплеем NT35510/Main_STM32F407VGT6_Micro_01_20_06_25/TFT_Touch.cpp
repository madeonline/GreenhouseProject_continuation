#include "TFT_Touch.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define Z_THRESHOLD     400
#define Z_THRESHOLD_INT  75
#define MSEC_THRESHOLD  3

#define READ_X 0x91
#define READ_Y 0xD1
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static MyTFTTouch   *isrPinptr;
void isrPin(void);
SPI_HandleTypeDef hspi2 = {};  // Указать номер SPI
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void isrPin( void )
{
  MyTFTTouch *o = isrPinptr;
  o->isrWake = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16_t besttwoavg( int16_t x , int16_t y , int16_t z ) 
{
  int16_t da, db, dc;
  int16_t reta = 0;
  if ( x > y ) da = x - y; else da = y - x;
  if ( x > z ) db = x - z; else db = z - x;
  if ( z > y ) dc = z - y; else dc = y - z;

  if ( da <= db && da <= dc ) reta = (x + y) >> 1;
  else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;   //    else if ( dc <= da && dc <= db ) reta = (x + y) >> 1;

  return (reta);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TS_Point::TS_Point(void) 
{
	x = y = z = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TS_Point::TS_Point(int16_t x0, int16_t y0, int16_t z0) 
{
	x = x0;
	y = y0;
	z = z0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TS_Point::operator==(TS_Point p1) 
{
	return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TS_Point::operator!=(TS_Point p1) 
{
	return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MyTFTTouch::MyTFTTouch(int8_t _cs_pin, int8_t _irq_pin,uint32_t _touchScaleX, uint32_t _touchScaleY, uint32_t _touchMinRawX, uint32_t _touchMaxRawX, uint32_t _touchMinRawY, uint32_t _touchMaxRawY)
{
	_pin_cs = _cs_pin;
	_pin_irq = _irq_pin;
  
  touchScaleX = _touchScaleX;
  touchScaleY = _touchScaleY;
  touchMinRawX = _touchMinRawX;
  touchMaxRawX = _touchMaxRawX;
  touchMinRawY = _touchMinRawY;
  touchMaxRawY = _touchMaxRawY;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyTFTTouch::begin()   // Настройка порта SPI2  
{

  // Настройка порта SPI2  с применением библиотеки настройки периферии HAL для STM32
  
  pinMode(_pin_cs, OUTPUT);
  digitalWrite(_pin_cs, HIGH);

  if(_pin_irq != -1)
  {
    pinMode(_pin_irq, INPUT);
    digitalWrite(_pin_irq, HIGH);

    attachInterrupt((_pin_irq), isrPin, FALLING);
    isrPinptr = this;
  }

  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  /* SPI2 clock enable */
  __HAL_RCC_SPI2_CLK_ENABLE();  // Включить тактирование SPI2
  __HAL_RCC_GPIOB_CLK_ENABLE(); // Включить тактирование pin

  /**SPI2 GPIO Configuration
  PB12     ------> SPI2_NSS
  PB13     ------> SPI2_SCK
  PB14     ------> SPI2_MISO
  PB15     ------> SPI2_MOSI
  */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; // Назначить pin для SPI2
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                                      // Настроить на вывод
  GPIO_InitStruct.Pull = GPIO_NOPULL;                                          // Поддержку резисторов отключить
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;                           // Скорость SPI 50 MHz
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;                                   // Альтернативная настройка pin для SPI2
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);                                      // включить порт B
  

  HAL_NVIC_SetPriority(SPI2_IRQn, 1, 0);                                       // Настройка приоритета прерывания для тачскрина (не применяем)
  HAL_NVIC_EnableIRQ(SPI2_IRQn);                                               // Настройка прерывания SPI для тачскрина (не применяем)

  // настройки режима SPI2
  hspi2.Instance = SPI2;                             
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = TOUCH_PRESCALER;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi2);

  // новые команды инициализации тача
  const uint8_t cmd_init80[] = { 0x80 };
  const uint8_t cmd_zero[] = { 0x00 };

  digitalWrite(_pin_cs, LOW);
  HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_init80, sizeof(cmd_init80), HAL_MAX_DELAY);
  HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_zero, sizeof(cmd_zero), HAL_MAX_DELAY);
  HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_zero, sizeof(cmd_zero), HAL_MAX_DELAY);
  digitalWrite(_pin_cs, HIGH);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TS_Point MyTFTTouch::getPoint()
{

  _point.x = _point.y = _point.z = -1;


  static const uint8_t cmd_read_x[] = { READ_X };
  static const uint8_t cmd_read_y[] = { READ_Y };
  static const uint8_t cmd_read_z1[] = { 0xB1 };
  static const uint8_t cmd_read_z2[] = { 0xC1 };
  static const uint8_t cmd_read_D0[] = { 0xD0 };
  static const uint8_t cmd_read_Zero[] = { 0x00 };
  static const uint8_t zeroes_tx[] = { 0x00, 0x00 };
  
  int16_t data[6];
  uint8_t raw[2];

  if (!isrWake) 
  {
    return _point;
  }
  uint32_t now = millis();
  if (now - msraw < MSEC_THRESHOLD) 
  {
    return _point;  
  }

   digitalWrite(_pin_cs, LOW);
  
  //spi2.transfer(0xB1 /* Z1 */);
   HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_z1, sizeof(cmd_read_z1), HAL_MAX_DELAY);
   
   HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_z2, sizeof(cmd_read_z2), HAL_MAX_DELAY);
   HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
  
  int16_t z1 = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(0xC1 /* Z2 */) >> 3;
  int z = z1 + 4095;
  
  HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
  HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
   
  int16_t z2 = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(READ_X /* X */) >> 3;
  z -= z2;
  if (z >= Z_THRESHOLD) 
  {
//    spi2.transfer16(READ_X /* X */);  // dummy X measure, 1st is always noisy
    HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
    
    HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
    data[0] = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(READ_Y /* Y */) >> 3;
    
    HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
    data[1] = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(READ_X /* X */) >> 3; // make 3 x-y measurements
    
    HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
    data[2] = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(READ_Y /* Y */) >> 3;
    
    HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
    data[3] = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(READ_X /* X */) >> 3;
  }
  else 
  {
    data[0] = data[1] = data[2] = data[3] = 0;  // Compiler warns these values may be used unset on early exit.
  }
  HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_D0, sizeof(cmd_read_D0), HAL_MAX_DELAY);
  HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
  data[4] = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(0xD0 /* Y */) >> 3; // Last Y touch power down
  
  HAL_SPI_Transmit(&hspi2, (uint8_t*)cmd_read_Zero, sizeof(cmd_read_Zero), HAL_MAX_DELAY);
  HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)zeroes_tx, raw, sizeof(raw), HAL_MAX_DELAY);
  data[5] = ((((uint16_t)raw[0]) << 8) | ((uint16_t)raw[1])) >> 3;//spi2.transfer16(0) >> 3;
  
  digitalWrite(_pin_cs, HIGH);
  //spi2.endTransaction();
  //Serial.printf("z=%d  ::  z1=%d,  z2=%d  ", z, z1, z2);
  if (z < 0) z = 0;
  if (z < Z_THRESHOLD) 
  { //  if ( !touched ) {
    // Serial.println();
    zraw = 0;
    if (z < Z_THRESHOLD_INT) 
    { //  if ( !touched ) {
      if (255 != _pin_irq) isrWake = false;
    }
    return _point;
  }
  zraw = z;
  
  // Average pair with least distance between each measured x then y
  //Serial.printf("    z1=%d,z2=%d  ", z1, z2);
  //Serial.printf("p=%d,  %d,%d  %d,%d  %d,%d", zraw,
    //data[0], data[1], data[2], data[3], data[4], data[5]);
  int16_t x = besttwoavg( data[0], data[2], data[4] );
  int16_t y = besttwoavg( data[1], data[3], data[5] );
  
  //Serial.printf("    %d,%d", x, y);
  //Serial.println();
  if (z >= Z_THRESHOLD) 
  {
    msraw = now;  // good read completed, set wait
        
    switch (rotation) 
    {
      case 0:
      xraw = 4095 - y;
      yraw = x;
      break;
      
      case 1:
      xraw = x;
      yraw = y;
      break;
      
      case 2:
      xraw = y;
      yraw = 4095 - x;
      break;
      
      case 3: 
      xraw = 4095 - x;
      yraw = 4095 - y;
      break;

      case 4:
      xraw = y;
      yraw = x;
      break;
    }

  //  Serial.print("xraw: "); Serial.println(xraw);
 //   Serial.print("yraw: "); Serial.println(yraw);

    // измерения есть, надо конвертировать их в реальные координаты экрана

    if (xraw < touchMinRawX) xraw = touchMinRawX;
    if (xraw > touchMaxRawX) xraw = touchMaxRawX;
  
    if (yraw < touchMinRawY) yraw = touchMinRawY;
    if (yraw > touchMaxRawY) yraw = touchMaxRawY;
  
    
    _point.x = (xraw - touchMinRawX) * touchScaleX / (touchMaxRawX - touchMinRawX);
    _point.y = (yraw - touchMinRawY) * touchScaleY / (touchMaxRawY - touchMinRawY);    

    _point.z = zraw;
  } // if(z)
  

	return _point;  // в _point результат тачскрина
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Определение нажатия на тачскрин.
bool MyTFTTouch:: TouchPressed()
{
	return HAL_GPIO_ReadPin(TOUCH_IRQ_GPIO_Port, TOUCH_IRQ_Pin) == GPIO_PIN_RESET;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
