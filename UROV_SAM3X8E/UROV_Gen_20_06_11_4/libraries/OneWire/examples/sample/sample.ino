#include <OneWire.h>

/* DS18S20 Temperature chip i/o */

OneWire  ds(35);  // on pin 10

void setup(void) {
  Serial2.begin(115200);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  if ( !ds.search(addr)) {
    Serial2.print("No more addresses.\n");
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial2.print("R=");
  for( i = 0; i < 8; i++) {
    Serial2.print(addr[i], HEX);
    Serial2.print(" ");
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial2.print("CRC is not valid!\n");
      return;
  }
  
  if ( addr[0] != 0x10) {
      Serial2.print("Device is not a DS18S20 family device.\n");
      return;
  }

  // The DallasTemperature library can do all this work for you!

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial2.print("P=");
  Serial2.print(present,HEX);
  Serial2.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial2.print(data[i], HEX);
    Serial2.print(" ");
  }
  Serial2.print(" CRC=");
  Serial2.print( OneWire::crc8( data, 8), HEX);
  Serial2.println();
}
