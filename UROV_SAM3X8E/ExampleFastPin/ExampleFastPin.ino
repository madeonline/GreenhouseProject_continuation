
#include <digitalWriteFast.h>

#define pinNum 53


void setup()
{
  volatile byte pin;
  pinModeFast(pinNum, OUTPUT);
  digitalWriteFast(pinNum, LOW);

  for(int i = 1; i<10; i++ ) 
  {
    digitalWriteFast(pinNum, HIGH);
    delayMicroseconds(i);
    digitalWriteFast(pinNum, LOW);
    delayMicroseconds(1);
  }
}
void loop()
{
/*
  for(int i = 1; i<500; i++ ) 
  {
    digitalWriteFast(pinNum, HIGH);
    delayMicroseconds(1+i);
    digitalWriteFast(pinNum, LOW);
      delayMicroseconds(1+i);
  }
   for(int i = 500; i>0; i-- ) 
  {
    digitalWriteFast(pinNum, HIGH);
    delayMicroseconds(1+i);
    digitalWriteFast(pinNum, LOW);
    delayMicroseconds(1+i);
  }
*/
  /*
  //the pin is toggled multiple time before looping is because it took too long that the pin stayed low for 600ns, while clearing or setting the pin only took 125ns. For 16MHz Arduino Uno.
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  digitalWriteFast(pinNum, HIGH);
  digitalWriteFast(pinNum, LOW);
  */
}
