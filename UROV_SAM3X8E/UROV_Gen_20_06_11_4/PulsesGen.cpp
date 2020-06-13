//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "FileUtils.h"
#include "CONFIG.h"
#include "PulsesGen.h"
#include "Settings.h"
#include "DueTimer.h"
#include "digitalWriteFast.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8_t IMPULSE_PIN_A = 53;    // НОМЕР ПИНА A, НА КОТОРОМ БУДУТ ГЕНЕРИРОВАТЬСЯ ИМПУЛЬСЫ
const uint8_t IMPULSE_PIN_B = 49;    // НОМЕР ПИНА B, НА КОТОРОМ БУДУТ ГЕНЕРИРОВАТЬСЯ ИМПУЛЬСЫ
const uint8_t PULSE_ON_LEVEL = HIGH; // УРОВЕНЬ ВКЛЮЧЕННОГО ИМПУЛЬСА
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ImpulseGeneratorClass ImpulseGeneratorA(IMPULSE_PIN_A);
ImpulseGeneratorClass ImpulseGeneratorB(IMPULSE_PIN_B);
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
volatile bool timerAttached = false;
volatile bool timerStarted = false;
volatile uint8_t timerUsed = 0;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void genUpdate()
{  
  ImpulseGeneratorA.update();
  ImpulseGeneratorB.update();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ImpulseGeneratorClass::ImpulseGeneratorClass(uint8_t p)
{
  pin = p;
  
  workMode = igNothing;
  pList = NULL;
  memAddress = 0;
  memCount = 0;
  lastMicros = 0;
  listIterator = 0;
  pauseTime = 0;
  currentPinLevel = !PULSE_ON_LEVEL;
  done = false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::pinConfig()
{
  pinModeFast(pin,OUTPUT);
  currentPinLevel = !PULSE_ON_LEVEL;
  digitalWriteFast(pin,currentPinLevel);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::timerStart()
{
    timerUsed++;
    if(!timerStarted)
    {
      timerStarted = true;
      GEN_TIMER.start();
    }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::timerStop()
{
  if(timerUsed)
  {
    timerUsed--;
  }
  
  if(!timerUsed)
  {
     GEN_TIMER.stop();
     timerStarted = false;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::timerConfig()
{
    if(!timerAttached)
    {
      timerAttached = true;
      GEN_TIMER.attachInterrupt(genUpdate);
      GEN_TIMER.setPeriod(GEN_TIMER_PERIOD);
    }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t ImpulseGeneratorClass::getNextPauseTime(bool& done)
{
  done = false;
  uint32_t result = 0;
  uint8_t* ptr = (uint8_t*)&result;

 switch(workMode)
  {
    case igNothing: 
    {
      done = true;
    }
    break;
    
    case igFile:
    {
        if(file.isOpen())
        {
            for(size_t i=0;i<sizeof(result);i++)
            {
              int iCh = file.read();
              if(iCh == -1) // конец файла или ошибка чтения
              {
                done = true;
                break;
              }
              else
              {
                *ptr++ = (uint8_t) iCh;
              }
            } // for
            
        }
        else
        {
          done = true;
        }
    }
    break;

    case igEEPROM:
    {
      if(!memCount) // все записи из EEPROM вычитаны
      {
        done = true;
      }
      else
      {
          for(size_t i=0;i<sizeof(result);i++)
          {
              *ptr++ = Settings.read(memAddress);
              memAddress++;
              
          } // for
          
          if(memCount)
          {
            memCount--;
          }
      } // else
    }
    break;

    case igList:
    {
      if(!pList || listIterator >= pList->size()) // весь список пробежали
      {
        done = true;        
      }
      else
      {
        result = (*pList)[listIterator];
        listIterator++;
      }
    }
    break;
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::wipe()
{

  timerStop();
  
  switch(workMode)
  {
    case igNothing: 
    break;
    
    case igFile:
    {
        if(file.isOpen())
        {
          file.close();
        }
    }
    break;

    case igEEPROM:
    {
      memAddress = 0;
      memCount = 0;
    }
    break;

    case igList:
    {
      pList = NULL;
      listIterator = 0;
    }
    break;
  }

  pauseTime = 0;
  lastMicros = 0;
  
  workMode = igNothing;
  
  currentPinLevel = !PULSE_ON_LEVEL;  
  digitalWriteFast(pin,currentPinLevel);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::start(const String& fileName)
{
  wipe();
  pinConfig();
  timerConfig();

#ifndef _SD_OFF
  if(!file.open(fileName.c_str(),O_READ))
    return;  

  file.rewind();
  
  workMode = igFile;
  done = false;

  timerStart();

#endif // _SD_OFF

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::start(int memAddr)
{
    pinConfig();
    timerConfig();
  
    memAddress = memAddr;
    memCount = 0;

    uint8_t* ptr = (uint8_t*)&memCount;
    for(size_t i=0;i<sizeof(memCount);i++)
    {
      *ptr++ = Settings.read(memAddress);
      memAddress++;
    }

  workMode = igEEPROM;
  done = false;

  timerStart();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::start(const Vector<uint32_t>& list)
{
  pinConfig();
  timerConfig();
  
  pList = &list;
  listIterator = 0;
  workMode = igList;
  done = false;

  timerStart();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::stop()
{
  wipe();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImpulseGeneratorClass::update()
{
  if(workMode == igNothing || done) // не работаем никак, или уже закончили
  {
    return;
  }
  
  if(micros() - lastMicros >= pauseTime) // время паузы между сменой уровня вышло
  {
    pauseTime = getNextPauseTime(done);
    
    if(!done) // ещё не закончили работу, время паузы вышло, меняем уровень на пине
    {
      currentPinLevel = !currentPinLevel;
      
      digitalWriteFast(pin,currentPinLevel);

      lastMicros = micros(); // не забываем, что надо засечь текущее время
    }
    else // работа закончена, список импульсов кончился
    {
      wipe();
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

