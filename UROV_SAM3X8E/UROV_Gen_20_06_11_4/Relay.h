#pragma once
//--------------------------------------------------------------------------------------------------
#include <Arduino.h>
//--------------------------------------------------------------------------------------------------
class Relay
{
  public:
    Relay(uint8_t pin);
    void begin();

	bool isOn();
	void on();
	void off();
	void setState(bool state);
	void switchState();

  private:
  
	uint8_t pin;
	bool status;
};
//--------------------------------------------------------------------------------------------------
extern Relay Relay_LineALL;
extern Relay Relay_LineA;
extern Relay Relay_LineB;
extern Relay Relay_LineC;
extern Relay Relay_Shunt1;
extern Relay Relay_Shunt2;
//--------------------------------------------------------------------------------------------------

