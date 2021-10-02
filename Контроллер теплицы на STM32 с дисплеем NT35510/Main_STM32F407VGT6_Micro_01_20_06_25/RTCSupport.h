#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------
#include "AbstractModule.h"
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_INTERNAL_CLOCK
  #include <STM32RTC.h>      // Часы
#else
    #include <Wire.h>
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
struct RTCTime // данные по текущему времени
{
  uint8_t second; // секунда (0-59)
  uint8_t minute; // минута (0-59)
  uint8_t hour; // час (0-23)
  uint8_t dayOfWeek; // день недели (1 - понедельник и т.д.)
  uint8_t dayOfMonth; // день месяца (1-31)
  uint8_t month; // месяц(1-12)
  uint16_t year; // формат - ХХХХ

  uint32_t unixtime() const;  
  RTCTime maketime(uint32_t unixtime);


}; 
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_INTERNAL_CLOCK
//--------------------------------------------------------------------------------------------------------------------------------------
class RealtimeClock
{
  public:
    RealtimeClock();
    void setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint16_t year);
    void setTime(const RTCTime& time);


    const char* getDayOfWeekStr(const RTCTime& t);
    const char* getTimeStr(const RTCTime& t);
    const char* getDateStr(const RTCTime& t);

    RTCTime getTime();

    Temperature getTemperature();
 
    void begin();
        
  private:
    static char workBuff[12]; // буфер под дату/время
    
  
};
//--------------------------------------------------------------------------------------------------------------------------------------
#else
//--------------------------------------------------------------------------------------------------------------------------------------
enum { DS3231Address = 0x68/*, DS3231Address_W = 0xD0, DS3231Address_R = 0xD1*/}; // адрес датчика
//--------------------------------------------------------------------------------------------------------------------------------------
class RealtimeClock
{


public:

    RealtimeClock();

    void setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint16_t year);
    void setTime(const RTCTime& time);

    const char* getDayOfWeekStr(const RTCTime& t);
    const char* getTimeStr(const RTCTime& t);
    const char* getDateStr(const RTCTime& t);

    RTCTime getTime();

    Temperature getTemperature();
 
    void begin(uint8_t wireNumber);

private:

    uint8_t dec2bcd(uint8_t val);
    uint8_t bcd2dec(uint8_t val);

    TwoWire* wireInterface;
    static char workBuff[12]; // буфер под дату/время

};
//--------------------------------------------------------------------------------------------------------------------------------------
#endif
