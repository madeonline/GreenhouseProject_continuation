//--------------------------------------------------------------------------------------------------
#include "Relay.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------
Relay Relay_LineALL(rele_lineAll);
Relay Relay_LineA(rele_lineA);
Relay Relay_LineB(rele_lineB);
Relay Relay_LineC(rele_lineC);
Relay Relay_Shunt1(rele_shunt1);
Relay Relay_Shunt2(rele_shunt2);
//--------------------------------------------------------------------------------------------------
Relay::Relay(uint8_t p)
{
  pin = p;
  status = false;
}
//--------------------------------------------------------------------------------------------------
void Relay::begin()
{
	 pinMode(pin,OUTPUT);
	 status = true;
	 off();
}
//--------------------------------------------------------------------------------------------------
bool Relay::isOn()
{
	return status;
}
//--------------------------------------------------------------------------------------------------
void Relay::on()
{
	if(status)
	{
		return;
	}
	
	status = true;
	digitalWrite(pin, RELAY_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void Relay::off()
{
	if(!status)
	{
		return;
	}
	
	status = false;
	digitalWrite(pin, !RELAY_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void Relay::setState(bool state)
{
	status = state;
	digitalWrite(pin, status ? RELAY_ON_LEVEL : !RELAY_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void Relay::switchState()
{
	status = !status;
	setState(status);
}
//--------------------------------------------------------------------------------------------------

