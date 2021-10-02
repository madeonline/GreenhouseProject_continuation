 #include "RTCSupport.h"
 #include <Arduino.h>
 #include "AbstractModule.h"
//--------------------------------------------------------------------------------------------------------------------------------------
// RTCTime
//--------------------------------------------------------------------------------------------------------------------------------------
#include <time.h>
//--------------------------------------------------------------------------------------------------------------------------------------
uint32_t RTCTime::unixtime() const
{
  tm t;
  t.tm_hour = hour;
  t.tm_isdst = 0;
  t.tm_mday = dayOfMonth;
  t.tm_min = minute;
  t.tm_mon = month - 1;
  t.tm_sec = second;
  t.tm_wday = dayOfWeek - 1;
  t.tm_year = year - 1900;

  return mktime(&t);
}
//--------------------------------------------------------------------------------------------------------------------------------------
RTCTime RTCTime::maketime(uint32_t time)
{
  RTCTime result;

  time_t tt = time;
  tm* t = localtime(&tt);

  result.hour = t->tm_hour;
  //t.tm_isdst = 0;
  result.dayOfMonth = t->tm_mday;
  result.minute = t->tm_min;
  result.month = t->tm_mon + 1;
  result.second = t->tm_sec;
  result.dayOfWeek = t->tm_wday;  

  // 0 - воскресенье, 1 - понедельник, ... 6 - суббота
  //Serial.println(result.dayOfWeek);
  
  if(!result.dayOfWeek) // 0 - воскресенье, остальные - соответствуют своим номерам
    result.dayOfWeek = 7;
  //else
 //   result.dayOfWeek++;
    
  if(result.dayOfWeek > 7)
    result.dayOfWeek = result.dayOfWeek - 7;
    
  result.year = t->tm_year + 1900; 

  return result;
}    
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_INTERNAL_CLOCK
//--------------------------------------------------------------------------------------------------------------------------------------
char RealtimeClock::workBuff[12] = {0};
//--------------------------------------------------------------------------------------------------------------------------------------
RealtimeClock::RealtimeClock()
{
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void RealtimeClock::begin()
{

  STM32RTC& rtc = STM32RTC::getInstance(); 
  // Select RTC clock source: LSI_CLOCK, LSE_CLOCK or HSE_CLOCK.
  // By default the LSI is selected as source.
  rtc.setClockSource(STM32RTC::LSE_CLOCK);     // 
  rtc.begin(); // initialize RTC 24H format
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
RTCTime RealtimeClock::getTime()
{
  STM32RTC& rtc = STM32RTC::getInstance(); 
  RTCTime result;

  uint32_t subSeconds;
   rtc.getTime((uint8_t*)&(result.hour), (uint8_t*)&(result.minute), (uint8_t*)&(result.second),&subSeconds);

   uint8_t y;
   rtc.getDate((uint8_t*)&(result.dayOfWeek), (uint8_t*)&(result.dayOfMonth), (uint8_t*)&(result.month),&y);
   result.year = 2000+y;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void RealtimeClock::setTime(const RTCTime& time)
{
  setTime(time.second, time.minute, time.hour, time.dayOfWeek, time.dayOfMonth, time.month,time.year);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void RealtimeClock::setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint16_t year)
{
  STM32RTC& rtc = STM32RTC::getInstance(); 

  while(year > 100) // приводим к диапазону 0-99
  {
    year -= 100;
  }
  
  rtc.setTime(hour, minute, second);  
  rtc.setDate(dayOfWeek,dayOfMonth,month, year);
}
//--------------------------------------------------------------------------------------------------------------------------------------
Temperature RealtimeClock::getTemperature()
{
 Temperature res;
 
 return res;
}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* RealtimeClock::getDayOfWeekStr(const RTCTime& t)
{
  static const char* dow[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
  return dow[t.dayOfWeek-1];
}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* RealtimeClock::getTimeStr(const RTCTime& t)
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
const char* RealtimeClock::getDateStr(const RTCTime& t)
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
#else
//--------------------------------------------------------------------------------------------------------------------------------------
char RealtimeClock::workBuff[12] = {0};
//--------------------------------------------------------------------------------------------------------------------------------------
RealtimeClock::RealtimeClock()
{
    wireInterface = &Wire;
}
//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t RealtimeClock::dec2bcd(uint8_t val)
{
  return( (val/10*16) + (val%10) );
}
//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t RealtimeClock::bcd2dec(uint8_t val)
{
  return( (val/16*10) + (val%16) );
}
//--------------------------------------------------------------------------------------------------------------------------------------
void RealtimeClock::setTime(const RTCTime& time)
{
  setTime(time.second, time.minute, time.hour, time.dayOfWeek, time.dayOfMonth, time.month,time.year);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void RealtimeClock::setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint16_t year)
{

  while(year > 100) // приводим к диапазону 0-99
  {
    year -= 100;
  }
 
  wireInterface->beginTransmission(DS3231Address);
  
  wireInterface->write(0); // указываем, что начинаем писать с регистра секунд
  wireInterface->write(dec2bcd(second)); // пишем секунды
  wireInterface->write(dec2bcd(minute)); // пишем минуты
  wireInterface->write(dec2bcd(hour)); // пишем часы
  wireInterface->write(dec2bcd(dayOfWeek)); // пишем день недели
  wireInterface->write(dec2bcd(dayOfMonth)); // пишем дату
  wireInterface->write(dec2bcd(month)); // пишем месяц
  wireInterface->write(dec2bcd(year)); // пишем год
  
  wireInterface->endTransmission(true);  

  delay(10); // немного подождём для надёжности

}
//--------------------------------------------------------------------------------------------------------------------------------------
Temperature RealtimeClock::getTemperature()
{

     Temperature res;
      
     union int16_byte {
           int i;
           byte b[2];
       } rtcTemp;
         
      wireInterface->beginTransmission(DS3231Address);
      wireInterface->write(0x11);
      if(wireInterface->endTransmission(true) != 0) // ошибка
        return res;
    
      if(wireInterface->requestFrom(DS3231Address, 2) == 2)
      {
        rtcTemp.b[1] = wireInterface->read();
        rtcTemp.b[0] = wireInterface->read();
    
        long tempC100 = (rtcTemp.i >> 6) * 25;
    
        res.Value = tempC100/100;
        res.Fract = abs(tempC100 % 100);
        
      }
      
      return res;

}
//--------------------------------------------------------------------------------------------------------------------------------------
RTCTime RealtimeClock::getTime()
{
  static RTCTime t;
  static uint32_t lastRequestTime = 0;
  
  if(!lastRequestTime)
    memset(&t,0,sizeof(t));


     if(!lastRequestTime || ( (millis() - lastRequestTime) > 999) )
     {
        wireInterface->beginTransmission(DS3231Address);
        wireInterface->write(0); // говорим, что мы собираемся читать с регистра 0
        
        if(wireInterface->endTransmission(true) != 0) // ошибка
          return t;
        
        if(wireInterface->requestFrom(DS3231Address, 7) == 7) // читаем 7 байт, начиная с регистра 0
        {
            t.second = bcd2dec(wireInterface->read() & 0x7F);
            if(t.second > 59)
              t.second = 59;
            
            t.minute = bcd2dec(wireInterface->read());
            if(t.minute > 59)
              t.minute = 59;
            
            t.hour = bcd2dec(wireInterface->read() & 0x3F);
            if(t.hour > 23)
              t.hour = 23;
            
            t.dayOfWeek = bcd2dec(wireInterface->read());
            t.dayOfWeek = constrain(t.dayOfWeek,1,7);
            
            t.dayOfMonth = bcd2dec(wireInterface->read());
            t.dayOfMonth = constrain(t.dayOfMonth,1,31);
            
            t.month = bcd2dec(wireInterface->read());
            t.month = constrain(t.month,1,12);
            
            t.year = bcd2dec(wireInterface->read());     
            t.year += 2000; // приводим время к нормальному формату
        } // if

        lastRequestTime = millis();
     }
      
      return t;

}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* RealtimeClock::getDayOfWeekStr(const RTCTime& t)
{
  uint8_t idx = t.dayOfWeek;
  if(idx > 0)
    --idx;

  if(idx > 6)
    idx = 6;
    
  static const char* dow[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
  return dow[idx];
}
//--------------------------------------------------------------------------------------------------------------------------------------
const char* RealtimeClock::getTimeStr(const RTCTime& t)
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
const char* RealtimeClock::getDateStr(const RTCTime& t)
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
void RealtimeClock::begin(uint8_t wireNumber)
{
#if TARGET_BOARD == STM32_BOARD
/*
  if(wireNumber == 1)
    wireInterface = &Wire1;
  else
*/  
    wireInterface = &Wire;
#else
  #error "Unknown target board!!!"      
#endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
#endif
