#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <SdFat.h>
#include <Arduino.h>
#include "TinyVector.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
	igNothing,
	igFile,
	igEEPROM,
	igList
	
} ImpulseGeneratorWorkMode;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImpulseGeneratorClass
{
  public:

  ImpulseGeneratorClass(uint8_t pin);
	
	void start(const String& fileName);
	void start(int memAddr);
	void start(const Vector<uint32_t>& list);
	
	void stop();
	
	void update();
  
  bool isDone() { return done; }
  bool isRunning()
  {
    return (workMode != igNothing) && !done;
  }
	
	
  private:

  uint8_t pin;
  
  void pinConfig();
  void timerConfig();

  void timerStart();
  void timerStop();

  uint32_t getNextPauseTime(bool& done);

  void wipe();
  ImpulseGeneratorWorkMode workMode;
  bool done;
  
	SdFile file;
  
	int memAddress;
  uint32_t memCount;
 
	const Vector<uint32_t>* pList;
  size_t listIterator;
	
	uint32_t lastMicros;
  uint32_t pauseTime;
	uint8_t currentPinLevel;
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern ImpulseGeneratorClass ImpulseGeneratorA;
extern ImpulseGeneratorClass ImpulseGeneratorB;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
