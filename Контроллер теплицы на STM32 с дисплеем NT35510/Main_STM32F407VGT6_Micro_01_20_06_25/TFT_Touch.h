#pragma once
#include "Arduino.h"
#include "Globals.h"
#include <SPI.h>

extern SPI_HandleTypeDef  hspi2;


class TS_Point {
public:
	TS_Point(void);
	TS_Point(int16_t x, int16_t y, int16_t z);

	bool operator==(TS_Point);
	bool operator!=(TS_Point);

	int16_t x, y, z;
};

class MyTFTTouch
{
public:
	MyTFTTouch(int8_t _cs_pin, int8_t _irq_pin, uint32_t _touchScaleX, uint32_t _touchScaleY, uint32_t _touchMinRawX, uint32_t _touchMaxRawX, uint32_t _touchMinRawY, uint32_t _touchMaxRawY);
	void begin();

	TS_Point getPoint();
	boolean TouchPressed();

 void setRotation(uint8_t n) { rotation = n % 5; }

  volatile bool isrWake=true;
   
private:
	int8_t _pin_cs, _pin_irq;
 
  uint32_t touchScaleX;
  uint32_t touchScaleY;

  uint32_t touchMinRawX, touchMaxRawX;
  uint32_t touchMinRawY, touchMaxRawY;

  int16_t xraw=0, yraw=0, zraw=0;
  uint8_t rotation = 1;
  uint32_t msraw=0x80000000;  

	TS_Point _point = { 0, 0, 0 };
};
