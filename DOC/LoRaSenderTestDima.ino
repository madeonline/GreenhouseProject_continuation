#include <SPI.h>
#include <LoRa.h>
 #define Serial SERIAL_PORT_USBVIRTUAL
int counter = 0;

void setup() {
  Serial.begin(9600);
 // while (!Serial);
LoRa.setPins(10,9,2);         // Только для М0
  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(1000);
}
