// UTFT font SensorFont2 size: 32x46

#if defined(__AVR__)
	#include <avr/pgmspace.h>
	#define fontdatatype const uint8_t
#elif defined(__PIC32MX__)
	#define PROGMEM
	#define fontdatatype const unsigned char
#elif defined(__arm__)
	#define PROGMEM
	#define fontdatatype const unsigned char
#endif


fontdatatype SensorFont2[1844] PROGMEM = {0x20,0x2E,0x30,0x0A,	// size 32x46, first 48, count 10
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xC0,
0x00,0x00,0x7F,0xC0,0x00,0x00,0x71,0xC0,0x00,0x00,0x71,0xC0,0x00,0x00,0x71,0xC0,
0x00,0x00,0x71,0xC0,0x00,0x00,0xF1,0xC0,0x00,0x00,0xE1,0xC0,0x00,0x00,0xE1,0xC0,
0x00,0x00,0xE1,0xC0,0x00,0x00,0xE1,0xC0,0x00,0x01,0xC1,0xC0,0x00,0x03,0xC1,0xC0,
0x00,0x0F,0x81,0xC0,0x00,0x0F,0x01,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '0' litres
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF0,0x00,0x00,0x00,0x38,
0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0xF0,
0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x38,
0x01,0xE0,0xF1,0xF0,0x01,0xF1,0xF0,0x00,0x01,0xDB,0xF0,0x00,0x01,0xDF,0x70,0x00,
0x01,0xDF,0x70,0x00,0x01,0xCE,0x70,0x00,0x01,0xCE,0x70,0x00,0x01,0xC4,0x70,0x00,
0x01,0xC0,0x70,0x00,0x01,0xC0,0x70,0x00,0x01,0xC0,0x70,0x00,0x01,0xC0,0x70,0x00,
0x01,0xC0,0x70,0x00,0x01,0xC0,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '1' m3
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0xF0,0x00,0x00,0x03,0xF0,
0x00,0x00,0x0F,0xF0,0x00,0x00,0x3F,0xFF,0x00,0x00,0xFF,0xFF,0x00,0x03,0xFF,0xFF,
0x00,0x0F,0xFF,0xFF,0x00,0x3F,0xC0,0x7F,0x00,0xFF,0xC0,0x3F,0x03,0xFF,0xCF,0x3F,
0x0F,0xFF,0xCF,0x3F,0x3F,0xFF,0xC0,0x7F,0x7F,0xFF,0xC0,0x3F,0x7F,0xFF,0xCF,0x9F,
0x3F,0xFF,0xCF,0x9F,0x0F,0xFF,0xCF,0x9F,0x03,0xFF,0xC0,0x3F,0x00,0xFF,0xC0,0x7F,
0x00,0x3F,0xFF,0xFF,0x00,0x0F,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x00,0x00,0xFF,0xFF,
0x00,0x00,0x3F,0xFF,0x00,0x00,0x0F,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,0xF0,
0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '2' east wind
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,
0x0F,0xF0,0x00,0x00,0x0F,0xFC,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xC0,0x00,
0xFF,0xFF,0xF0,0x00,0xFF,0x07,0xFC,0x00,0xFE,0x03,0xFF,0x00,0xFE,0x79,0xFF,0xC0,
0xFF,0xF9,0xFF,0xF0,0xFF,0xF3,0xFF,0xFC,0xFF,0xC7,0xFF,0xFE,0xFF,0xC3,0xFF,0xFE,
0xFF,0xF9,0xFF,0xFC,0xFF,0xF9,0xFF,0xF0,0xFE,0x71,0xFF,0xC0,0xFE,0x03,0xFF,0x00,
0xFF,0x07,0xFC,0x00,0xFF,0xFF,0xF0,0x00,0xFF,0xFF,0xC0,0x00,0xFF,0xFF,0x00,0x00,
0x0F,0xFC,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x0F,0x00,0x00,0x00,
0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '3' west wind
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,
0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x7F,0xFC,0x1F,0xFE,0x7F,0xF8,0x0F,0xFE,0x3F,0xF9,0xCF,0xFC,0x3F,0xF3,0xFF,0xFC,
0x1F,0xF3,0xFF,0xF8,0x1F,0xF3,0xFF,0xF8,0x0F,0xF3,0xFF,0xF0,0x0F,0xF3,0xFF,0xF0,
0x07,0xF3,0xFF,0xE0,0x07,0xF9,0xCF,0xE0,0x03,0xF8,0x0F,0xC0,0x03,0xFC,0x1F,0xC0,
0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,
0x00,0x7F,0xFE,0x00,0x00,0x7F,0xFE,0x00,0x00,0x3F,0xFC,0x00,0x00,0x3F,0xFC,0x00,
0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,
0x00,0x07,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,
0x00,0x01,0x80,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '4' north wind
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x80,0x00,0x00,0x01,0x80,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,
0x00,0x07,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,
0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x3F,0xFC,0x00,0x00,0x3F,0xFC,0x00,
0x00,0x7F,0xFE,0x00,0x00,0x7F,0xFE,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,
0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x03,0xFF,0xFF,0xC0,0x03,0xCF,0x0F,0xC0,
0x07,0xCE,0x07,0xE0,0x07,0xCE,0x63,0xE0,0x0F,0xCC,0xF3,0xF0,0x0F,0xCC,0xF3,0xF0,
0x1F,0xC0,0xF3,0xF8,0x1F,0xC0,0xF3,0xF8,0x3F,0xCC,0xF3,0xFC,0x3F,0xCC,0x63,0xFC,
0x7F,0xCE,0x07,0xFE,0x7F,0xCF,0x0F,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,
0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '5' south wind
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0F,0xC3,0xF1,0xEE,0x1C,0xC7,0x31,0xB6,0x18,0x66,0x19,0xB6,
0x18,0x66,0x19,0xB6,0x18,0x66,0x19,0xB6,0x18,0x66,0x19,0xB6,0x18,0xE6,0x39,0xB6,
0x1C,0xC7,0x31,0xB6,0x1F,0x87,0xE1,0xB6,0x18,0x06,0x00,0x00,0x18,0x06,0x00,0x00,
0x18,0x06,0x00,0x00,0x18,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '6' ppm
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '7'
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// digit '8'
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	// digit '9'
};