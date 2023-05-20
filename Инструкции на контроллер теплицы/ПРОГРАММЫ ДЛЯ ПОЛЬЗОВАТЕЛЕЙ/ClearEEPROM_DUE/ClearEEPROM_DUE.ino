//--------------------------------------------------------------------------------------------------------------------------------
// какую AT24CX используем ?
#define AT24CX_TYPE 3 // 1 - AT24C32, 2 - AT24C64, 3 - AT24C128, 4 - AT24C256, 5 - AT24C512
//--------------------------------------------------------------------------------------------------------------------------------
#include "AT24CX.h"
#include <Wire.h>
//--------------------------------------------------------------------------------------------------------------------------------
#if AT24CX_TYPE == 3
AT24C32 eeprom;
const unsigned int eepromSize = 4096;
#elif AT24CX_TYPE == 2
AT24C64 eeprom;
const unsigned int eepromSize = 4096 * 2;
#elif AT24CX_TYPE == 3
AT24C128 eeprom;
const unsigned int eepromSize = 4096 * 4;
#elif AT24CX_TYPE == 4
AT24C256 eeprom;
const unsigned int eepromSize = 4096 * 8;
#elif AT24CX_TYPE == 5
AT24C512 eeprom;
const unsigned int eepromSize = 4096 * 16;
#endif

uint32_t adr_Serial = 0;


//--------------------------------------------------------------------------------------------------------------------------------
// НАСТРОЙКИ ВНЕШНЕГО ВАТЧДОГА
//--------------------------------------------------------------------------------------------------------------------------------

  #define WATCHDOG_REBOOT_PIN 66          // номер пина, на котором будут меняться уровни
  #define WATCHDOG_NORMAL_LEVEL HIGH      // уровень на ноге, который держится WATCHDOG_WORK_INTERVAL миллисекунд
  #define WATCHDOG_TRIGGERED_LEVEL LOW    // уровень импульса на ноге, который держится WATCHDOG_PULSE_DURATION миллисекунд
  #define WATCHDOG_PULSE_DURATION 600     // сколько миллисекунд держать уровень WATCHDOG_TRIGGERED_LEVEL на ноге


//--------------------------------------------------------------------------------------------------------------------------------
void setup()
{
    Serial.begin(57600);
    Serial.println("Start AT24CX EEPROM cleaning!!");

    Wire.begin();
 
    pinMode(WATCHDOG_REBOOT_PIN, OUTPUT);
    digitalWrite(WATCHDOG_REBOOT_PIN, WATCHDOG_TRIGGERED_LEVEL);
    delay(WATCHDOG_PULSE_DURATION);
    digitalWrite(WATCHDOG_REBOOT_PIN, WATCHDOG_NORMAL_LEVEL);
 
    byte info = 0;
    uint32_t len = 0;
    eeprom.write(254, (byte)0xF1);
    info = eeprom.read(254);
    if (info != 0xF1)
    {
        Serial.println("AT24CX EEPROM filed!");
    }
    else
    {
        Serial.println("Test AT24CX EEPROM Ok!");
        Serial.println("Start AT24CX EEPROM cleaning...");

        len = 16 * 1024;
        for (uint32_t address = 0; address < len; address++)
        {
            eeprom.write(address, 0xFF);

            adr_Serial++;
            if (adr_Serial == 256)
            {
            Serial.println(address + 1);
            adr_Serial = 0;
        
            digitalWrite(WATCHDOG_REBOOT_PIN, WATCHDOG_TRIGGERED_LEVEL);
            delay(WATCHDOG_PULSE_DURATION);
            digitalWrite(WATCHDOG_REBOOT_PIN, WATCHDOG_NORMAL_LEVEL);
            }
        } // for
        Serial.println("AT24CX EEPROM cleared!");
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:

}
//--------------------------------------------------------------------------------------------------------------------------------
