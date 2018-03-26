
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 176X220 pixels.
//
// This program requires the UTFT library.

//Demo for 
//by Open-Smart Team and Catalex Team
//catalex_inc@163.com
//Store: open-smart.aliexpress.com
//      http://dx.com
//Demo Function: 


#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
//***********************************************//
// If you use OPEN-SMART TFT breakout board                 //
// You need to add 5V-3.3V level converting circuit.
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

//
// Remember to change the model parameter to suit your display module!
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_RS A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
//#define LCD_RST A0 // 

UTFT tft(ILI9225,LCD_RS,LCD_WR,LCD_CS);

void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  tft.InitLCD();
  tft.setFont(SmallFont);
}

void loop()
{
  int buf[218];
  int x, x2;
  int y, y2;
  int r;

// Clear the screen and draw the frame
  tft.clrScr();

  tft.setColor(255, 0, 0);
  tft.fillRect(0, 0, 219, 13);
  tft.setColor(64, 64, 64);
  tft.fillRect(0, 162, 219, 175);
  tft.setColor(255, 255, 255);
  tft.setBackColor(255, 0, 0);
  tft.print("** Universal TFT Library **", CENTER, 1);
  tft.setBackColor(64, 64, 64);
  tft.setColor(255,255,0);
  tft.print("open-smart.aliexpress.com", CENTER, 163);

  tft.setColor(0, 0, 255);
  tft.drawRect(0, 14, 219, 161);

// Draw crosshairs
  tft.setColor(0, 0, 255);
  tft.setBackColor(0, 0, 0);
  tft.drawLine(109, 15, 109, 160);
  tft.drawLine(1, 88, 218, 88);

  for (int i=9; i<210; i+=10)
    tft.drawLine(i, 86, i, 90);
  for (int i=19; i<155; i+=10)
    tft.drawLine(107, i, 111, i);

// Draw sin-, cos- and tan-lines  
  tft.setColor(0,255,255);
  tft.print("Sin", 5, 15);
  for (int i=1; i<218; i++)
  {
    tft.drawPixel(i,88+(sin(((i*1.65)*3.14)/180)*70));
  }
  
  tft.setColor(255,0,0);
  tft.print("Cos", 5, 27);
  for (int i=1; i<218; i++)
  {
    tft.drawPixel(i,88+(cos(((i*1.65)*3.14)/180)*70));
  }

  tft.setColor(255,255,0);
  tft.print("Tan", 5, 39);
  for (int i=1; i<218; i++)
  {
    tft.drawPixel(i,88+(tan(((i*1.65)*3.14)/180)));
  }

  delay(2000);

  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);
  tft.setColor(0, 0, 255);
  tft.setBackColor(0, 0, 0);
  tft.drawLine(109, 15, 109, 160);
  tft.drawLine(1, 88, 218, 88);

// Draw a moving sinewave
  x=1;
  for (int i=1; i<(218*20); i++) 
  {
    x++;
    if (x==219)
      x=1;
    if (i>219)
    {
      if ((x==109)||(buf[x-1]==88))
        tft.setColor(0,0,255);
      else
        tft.setColor(0,0,0);
      tft.drawPixel(x,buf[x-1]);
    }
    tft.setColor(0,255,255);
    y=88+(sin(((i*1.6)*3.14)/180)*(65-(i / 100)));
    tft.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        tft.setColor(255,0,255);
        break;
      case 2:
        tft.setColor(255,0,0);
        break;
      case 3:
        tft.setColor(0,255,0);
        break;
      case 4:
        tft.setColor(0,0,255);
        break;
      case 5:
        tft.setColor(255,255,0);
        break;
    }
    tft.fillRect(44+(i*15), 23+(i*15), 88+(i*15), 63+(i*15));
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        tft.setColor(255,0,255);
        break;
      case 2:
        tft.setColor(255,0,0);
        break;
      case 3:
        tft.setColor(0,255,0);
        break;
      case 4:
        tft.setColor(0,0,255);
        break;
      case 5:
        tft.setColor(255,255,0);
        break;
    }
    tft.fillRoundRect(132-(i*15), 23+(i*15), 172-(i*15), 63+(i*15));
  }
  
  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        tft.setColor(255,0,255);
        break;
      case 2:
        tft.setColor(255,0,0);
        break;
      case 3:
        tft.setColor(0,255,0);
        break;
      case 4:
        tft.setColor(0,0,255);
        break;
      case 5:
        tft.setColor(255,255,0);
        break;
    }
    tft.fillCircle(64+(i*15),43+(i*15), 20);
  }
  
  delay(2000);
    
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

// Draw some lines in a pattern
  tft.setColor (255,0,0);
  for (int i=15; i<160; i+=5)
  {
    tft.drawLine(1, i, (i*1.44)-10, 160);
  }
  tft.setColor (255,0,0);
  for (int i=160; i>15; i-=5)
  {
    tft.drawLine(218, i, (i*1.44)-12, 15);
  }
  tft.setColor (0,255,255);
  for (int i=160; i>15; i-=5)
  {
    tft.drawLine(1, i, 232-(i*1.44), 15);
  }
  tft.setColor (0,255,255);
  for (int i=15; i<160; i+=5)
  {
    tft.drawLine(218, i, 231-(i*1.44), 160);
  }
  
  delay(2000);
  
    tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    tft.setColor(random(255), random(255), random(255));
    x=22+random(176);
    y=35+random(105);
    r=random(20);
    tft.drawCircle(x, y, r);
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    tft.setColor(random(255), random(255), random(255));
    x=2+random(216);
    y=16+random(143);
    x2=2+random(216);
    y2=16+random(143);
    tft.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

// Draw some random rounded rectangles
  for (int i=0; i<100; i++)
  {
    tft.setColor(random(255), random(255), random(255));
    x=2+random(216);
    y=16+random(143);
    x2=2+random(216);
    y2=16+random(143);
    tft.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

  for (int i=0; i<100; i++)
  {
    tft.setColor(random(255), random(255), random(255));
    x=2+random(216);
    y=16+random(143);
    x2=2+random(216);
    y2=16+random(143);
    tft.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,218,160);

  for (int i=0; i<10000; i++)
  {
    tft.setColor(random(255), random(255), random(255));
    tft.drawPixel(2+random(216), 16+random(143));
  }

  delay(2000);

  tft.fillScr(0, 0, 255);
  tft.setColor(255, 0, 0);
  tft.fillRoundRect(40, 57, 179, 119);
  
  tft.setColor(255, 255, 255);
  tft.setBackColor(255, 0, 0);
  tft.print("That's it!", CENTER, 62);
  tft.print("Restarting in a", CENTER, 88);
  tft.print("few seconds...", CENTER, 101);
  
  tft.setColor(0, 255, 0);
  tft.setBackColor(0, 0, 255);
  tft.print("Runtime: (msecs)", CENTER, 146);
  tft.printNumI(millis(), CENTER, 161);

  delay (10000);
}

