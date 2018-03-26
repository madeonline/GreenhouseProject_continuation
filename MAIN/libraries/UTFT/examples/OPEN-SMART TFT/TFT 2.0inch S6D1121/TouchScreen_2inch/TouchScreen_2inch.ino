//Demo for 2.0" TFT LCD
//by Open-Smart Team and Catalex Team
//catalex_inc@163.com
//          OPEN-SMART UNO Black:  https://www.aliexpress.com/store/product/One-Set-UNO-R3-CH340G-ATMEGA328P-Development-Board-with-USB-Cable-for-Arduino-UNO-R3-Compatible/1199788_32653902890.html
//Reference: https://www.aliexpress.com/store/product/OPEN-SMART-3-2-inch-TFT-LCD-Display-Shield-with-temperature-sensor-onboard-for-Arduino-Mega2560/1199788_32749958914.html?spm=2114.8147860.0.0.qPVmYz
//Store: http://www.aliexpress.com/store/1199788
//      http://dx.com
//Demo Function: put the paper we ship to you at the bottom of the touch screen,
//               and touch the screen with the pen or your finger, the monitor
//              will print the number you touch.
// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// This demo code returns raw readings, public domain



#include <stdint.h>
#include <UTFT.h>
#include <TouchScreen.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
//***********************************************//
// If you use OPEN-SMART TFT breakout board                 //
// Reconmmend you to add 5V-3.3V level converting circuit.
// Of course you can use OPEN-SMART UNO Black version with 5V/3.3V power switch,
// you just need switch to 3.3V.
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
//----------------------------------------|
// TFT Breakout  -- Arduino UNO / Mega2560 / OPEN-SMART UNO Black
// GND              -- GND
// 3V3               -- 3.3V
// CS                 -- A3
// RS                 -- A2
// WR                -- A1
// RD                 -- 3.3V
// RST                -- RESET
// LED                -- GND
// DB0                -- 8
// DB1                -- 9
// DB2                -- 10
// DB3                -- 11
// DB4                -- 4
// DB5                -- 13
// DB6                -- 6
// DB7                -- 7

// These are the pins for the touch pannel!
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
#define TS_MINX  150
#define TS_MINY  120
#define TS_MAXX  900
#define TS_MAXY  950//you can debug to get the value
#define DEBUG_TS_VALUE 1

#define MINPRESSURE 10
#define MAXPRESSURE 1024

#define FRAME_WIDTH 10
#define RECT_WIDTH  100
#define RECT_HEIGHT 80

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
// Remember to change the model parameter to suit your display module!

#define BOXSIZE   40
#define PENRADIUS  3
int oldcolor, currentcolor;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 250 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 250);

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_RS A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
//#define LCD_RD A0 // LCD Read goes to Analog 0 the library not use read function

UTFT tft(ITDB24E_8,LCD_RS,LCD_WR,LCD_CS);
void setup(void) {
  Serial.begin(9600);
  tft.InitLCD();
  tft.setFont(SmallFont);
  tft.fillScr(BLACK);
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
  tft.fillRect(BOXSIZE*3,0, BOXSIZE, BOXSIZE, CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);

  //erase button
  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);
  tft.setColor(BLACK);
  tft.drawLine(BOXSIZE*6, 0,BOXSIZE*6+39,39);
  tft.drawLine(BOXSIZE*6, 39,BOXSIZE*6+39,0);
  
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
  currentcolor = RED;

}
int erase = 0;
void loop(void) {
  // a point object holds x y and z coordinates

  TSPoint p = ts.getPoint();
   pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);

  //    tft.fillRect(0, BOXSIZE, 319, 239-BOXSIZE);
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  	
  	#ifdef DEBUG_TS_VALUE
  	 Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
	#endif
	if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
	  tft.setColor(BLACK);
  	  for(uint8_t i = 0;i < 200;i ++){
        tft.drawHLine(0,40+i,320);
		
  	  }
	  for(int16_t i = 0;i < 320;i ++){
        tft.drawVLine(i,40,200);
		
  	  }
	  return;
    }
	//tft.fillScr(BLACK);
	 int16_t temp = p.y;
  	 p.y = map(p.x, TS_MINX, TS_MAXX, 240, 0);
     p.x = map(temp, TS_MINY, TS_MAXY, 0, 320);

	 if (p.y < BOXSIZE) {
       oldcolor = currentcolor;

       if (p.x < BOXSIZE) { 
         currentcolor = RED; 

         tft.drawRect(0, 0, BOXSIZE, BOXSIZE,WHITE);
		
       } else if (p.x < BOXSIZE*2) {
         currentcolor = YELLOW;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = GREEN;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = CYAN;
         tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = BLUE;
         tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = MAGENTA;
         tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);
       }
	   else if (p.x < BOXSIZE*7) {
      Serial.println("erase");
	  tft.setColor(BLACK);
  	  for(uint8_t i = 0;i < 200;i ++){
        tft.drawHLine(0,40+i,320);
		
  	  }
	  for(int16_t i = 0;i < 320;i ++){
        tft.drawVLine(i,40,200);
		
  	  }
	  return;
    }

		
       if (oldcolor != currentcolor) {
          if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == GREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
          if (oldcolor == CYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
       }
    
	 	}
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < 320)) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }

	
	
	
  	}
}
