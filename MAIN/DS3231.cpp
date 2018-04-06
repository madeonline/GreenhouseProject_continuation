#include "DS3231.h"
#include "ConfigPin.h"
//--------------------------------------------------------------------------------------------------------------------------------------
char DS3231Clock::workBuff[12] = {0};
DS3231Clock RealtimeClock;
//--------------------------------------------------------------------------------------------------------------------------------------
const uint8_t daysArray [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };
//--------------------------------------------------------------------------------------------------------------------------------------
uint32_t DS3231Time::unixtime(void)
{
  uint32_t u;

  u = time2long(date2days(this->year, this->month, this->dayOfMonth), this->hour, this->minute, this->second);
  u += 946684800; // + 01.01.2000 00:00:00 unixtime

  return u;
}
//--------------------------------------------------------------------------------------------------------------------------------------
DS3231Time DS3231Time::addDays(long days)
{
  uint32_t ut = unixtime();
  long diff = days*86400;
  ut += diff;  
  return fromUnixtime(ut);
}
//--------------------------------------------------------------------------------------------------------------------------------------
DS3231Time DS3231Time::fromUnixtime(uint32_t time)
{
  DS3231Time result;

  uint8_t year;
  uint8_t month, monthLength;
  unsigned long days;

  result.second = time % 60;
  time /= 60; // now it is minutes
  result.minute = time % 60;
  time /= 60; // now it is hours
  result.hour = time % 24;
  time /= 24; // now it is days
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (isLeapYear(year) ? 366 : 365)) <= time) {
    year++;
  }
  result.year = year + 1970; // year is offset from 1970 
  
  days -= isLeapYear(year) ? 366 : 365;
  time  -= days; // now it is days in this year, starting at 0
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) 
  {
    if (month==1) 
    { // february
      if (isLeapYear(year)) 
      {
        monthLength=29;
      } 
      else 
      {
        monthLength=28;
      }
    } 
    else 
    {
      monthLength = pgm_read_byte(daysArray + month); //monthDays[month];
    }
    
    if (time >= monthLength) 
    {
      time -= monthLength;
    } 
    else 
    {
        break;
    }
  }
  result.month = month + 1;  // jan is month 1  
  result.dayOfMonth = time + 1;     // day of month  

    int dow;
    byte mArr[12] = {6,2,2,5,0,3,5,1,4,6,2,4};
    dow = (result.year % 100);
    dow = dow*1.25;
    dow += result.dayOfMonth;
    dow += mArr[result.month-1];
    
    if (isLeapYear(result.year) && (result.month<3))
     dow -= 1;
     
    while (dow>7)
     dow -= 7;

   result.dayOfWeek = dow;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool DS3231Time::isLeapYear(uint16_t year)
{
  return (year % 4 == 0);
}
//--------------------------------------------------------------------------------------------------------------------------------------
uint16_t DS3231Time::date2days(uint16_t _year, uint8_t _month, uint8_t _day)
{
    _year = _year - 2000;

    uint16_t days16 = _day;

    for (uint8_t i = 1; i < _month; ++i)
    {
        days16 += pgm_read_byte(daysArray + i - 1);
    }

    if ((_month == 2) && isLeapYear(_year))
    {
        ++days16;
    }

    return days16 + 365 * _year + (_year + 3) / 4 - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------
long DS3231Time::time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds)
{    
    return ((days * 24L + hours) * 60 + minutes) * 60 + seconds;
}
//--------------------------------------------------------------------------------------------------------------------------------------
DS3231Clock::DS3231Clock()
{
  wire = &Wire;
}
//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t DS3231Clock::dec2bcd(uint8_t val)
{
  return( (val/10*16) + (val%10) );
}
//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t DS3231Clock::bcd2dec(uint8_t val)
{
  return( (val/16*10) + (val%16) );
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS3231Clock::setTime(const DS3231Time& time)
{
  setTime(time.second, time.minute, time.hour, time.dayOfWeek, time.dayOfMonth, time.month,time.year);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS3231Clock::setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint16_t year)
{

  while(year > 100) // приводим к диапазону 0-99
    year -= 100;
 
  wire->beginTransmission(DS3231Address);
  
  wire->write(0); // указываем, что начинаем писать с регистра секунд
  wire->write(dec2bcd(second)); // пишем секунды
  wire->write(dec2bcd(minute)); // пишем минуты
  wire->write(dec2bcd(hour)); // пишем часы
  wire->write(dec2bcd(dayOfWeek)); // пишем день недели
  wire->write(dec2bcd(dayOfMonth)); // пишем дату
  wire->write(dec2bcd(month)); // пишем месяц
  wire->write(dec2bcd(year)); // пишем год
  
  wire->endTransmission();

  delay(10); // немного подождём для надёжности
}
//--------------------------------------------------------------------------------------------------------------------------------------
DS3231Temperature DS3231Clock::getTemperature()
{
 DS3231Temperature res = {0};
  
 union int16_byte {
       int i;
       byte b[2];
   } rtcTemp;
     
  wire->beginTransmission(DS3231Address);
  wire->write(0x11);
  if(wire->endTransmission() != 0) // ошибка
    return res;

  if(wire->requestFrom(DS3231Address, 2) == 2)
  {
    rtcTemp.b[1] = wire->read();
    rtcTemp.b[0] = wire->read();

    long tempC100 = (rtcTemp.i >> 6) * 25;

    res.Value = tempC100/100;
    res.Fract = abs(tempC100 % 100);
    
  }
  
  return res;
}
//--------------------------------------------------------------------------------------------------------------------------------------
DS3231Time DS3231Clock::getTime()
{
  static DS3231Time t = {0};
  static uint32_t timeMillis = 0;
  static bool first = true;

  // чтобы часто не дёргать I2C
  if(first || (millis() - timeMillis > 1000))
  {
    first = false;
    timeMillis = millis();
    
    wire->beginTransmission(DS3231Address);
    wire->write(0); // говорим, что мы собираемся читать с регистра 0
    
    if(wire->endTransmission() != 0) // ошибка
      return t;
    
    if(wire->requestFrom(DS3231Address, 7) == 7) // читаем 7 байт, начиная с регистра 0
    {
        t.second = bcd2dec(wire->read() & 0x7F);
        t.minute = bcd2dec(wire->read());
        t.hour = bcd2dec(wire->read() & 0x3F);
        t.dayOfWeek = bcd2dec(wire->read());
        t.dayOfMonth = bcd2dec(wire->read());
        t.month = bcd2dec(wire->read());
        t.year = bcd2dec(wire->read());     
        t.year += 2000; // приводим время к нормальному формату
    } // if
  }
  return t;
}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* DS3231Clock::getTimeStr(const DS3231Time& t)
{
  char* writePtr = workBuff;
  
  if(t.hour < 10)
    *writePtr++ = '0';
  else
    *writePtr++ = (t.hour/10) + '0';

  *writePtr++ = (t.hour % 10) + '0';

 *writePtr++ = ':';

 if(t.minute < 10)
  *writePtr++ = '0';
 else
  *writePtr++ = (t.minute/10) + '0';

 *writePtr++ = (t.minute % 10) + '0';

 *writePtr++ = ':';

 if(t.second < 10)
  *writePtr++ = '0';
 else
  *writePtr++ = (t.second/10) + '0';

 *writePtr++ = (t.second % 10) + '0';

 *writePtr = 0;

 return workBuff;
}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* DS3231Clock::getDateStr(const DS3231Time& t)
{
  char* writePtr = workBuff;
  if(t.dayOfMonth < 10)
    *writePtr++ = '0';
  else
    *writePtr++ = (t.dayOfMonth/10) + '0';
  *writePtr++ = (t.dayOfMonth % 10) + '0';

  *writePtr++ = '.';

  if(t.month < 10)
    *writePtr++ = '0';
  else
    *writePtr++ = (t.month/10) + '0';
  *writePtr++ = (t.month % 10) + '0';

  *writePtr++ = '.';

  *writePtr++ = (t.year/1000) + '0';
  *writePtr++ = (t.year % 1000)/100 + '0';
  *writePtr++ = (t.year % 100)/10 + '0';
  *writePtr++ = (t.year % 10) + '0';  

  *writePtr = 0;

  return workBuff;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS3231Clock::begin(uint8_t wireNumber)
{
#if defined (__arm__) && defined (__SAM3X8E__)  
  if(wireNumber == 1)
    wire = &Wire1;
 else
#endif 
  wire = &Wire;
     
  wire->begin();
  
  ConfigPin::setI2CPriority(5);
  
}
//--------------------------------------------------------------------------------------------------------------------------------------

