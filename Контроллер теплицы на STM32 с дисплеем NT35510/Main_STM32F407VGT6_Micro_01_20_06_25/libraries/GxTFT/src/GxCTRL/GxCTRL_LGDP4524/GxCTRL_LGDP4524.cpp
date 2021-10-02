// created by Porokhnya Dmitry to be the GxCTRL_LGDP4524 class for the GxTFT library
// code extracts taken from http://www.rinkydinkelectronics.com/download.php?f=UTFT.zip
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#include "GxCTRL_LGDP4524.h"

uint32_t GxCTRL_LGDP4524::readID()
{
	return readRegister(0x0, 0, 2);
}

uint32_t GxCTRL_LGDP4524::readRegister(uint8_t nr, uint8_t index, uint8_t bytes)
{
/*	
 uint32_t rv = 0;
  bytes = min(bytes, 4);
  IO.startTransaction();
  IO.writeCommand(nr);
  for (uint8_t i = 0; i < index; i++)
  {
    IO.readData(); // skip
  }
  for (; bytes > 0; bytes--)
  {
    rv <<= 8;
    rv |= IO.readData();
  }
  IO.endTransaction();
  return rv;
  */
  IO.writeCommand(nr);
  return IO.readData16();
}

uint16_t GxCTRL_LGDP4524::readPixel(uint16_t x, uint16_t y)
{
  uint16_t rv;
  readRect(x, y, 1, 1, &rv);
  return rv;
}


void GxCTRL_LGDP4524::readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  uint32_t num = uint32_t(w) * uint32_t(h);
  for (uint16_t yy = y; yy <= ye; yy++)
  {
    for (uint16_t xx = x; xx <= xe; xx++)
    {
      IO.startTransaction();
      setWindowAddress(xx, yy, xe, ye);
      IO.writeCommand(0x22);
      IO.readData16(); // dummy
      *data++ = IO.readData16();
      IO.endTransaction();
    }
  }
}

void GxCTRL_LGDP4524::init()
{
//Serial.println("INIT GxCTRL_LGDP4524...");	
	
	rotation = 1; // landscape is default
  
	// reset first
	IO.reset();
	
	
	HAL_Delay(350);

	IO.writeCommand(0x07);
	IO.writeData16(0x0000);
	
	IO.writeCommand(0x12);
	IO.writeData16(0x0000);   
	
	IO.writeCommand(0x13);
	IO.writeData16(0x0000);   
	
	IO.writeCommand(0x14);
	IO.writeData16(0x0331);   
	
	HAL_Delay(50);   
	  
	IO.writeCommand(0x11);
	IO.writeData16(0x0101);
	
	IO.writeCommand(0x12);
	IO.writeData16(0x000A);
	
	IO.writeCommand(0x13);
	IO.writeData16(0x0D45);
	
	IO.writeCommand(0x10);
	IO.writeData16(0x0164); 
	
	IO.writeCommand(0x12);
	IO.writeData16(0x001A); 
	
	HAL_Delay(50);   
	
	IO.writeCommand(0x13);
	IO.writeData16(0x2D45);  
	
	IO.writeCommand(0x10);
	IO.writeData16(0x0160);  
	
	HAL_Delay(50);   
	
	IO.writeCommand(0x60);
	IO.writeData16(0x2000);   
	
	IO.writeCommand(0x60);
	IO.writeData16(0x0000); 
	
	IO.writeCommand(0x61);
	IO.writeData16(0x0002); 
	
	HAL_Delay(50);   
	
	IO.writeCommand(0x01);
	IO.writeData16(0x011B); 
	
	IO.writeCommand(0x02);
	IO.writeData16(0x0700); 
	
	IO.writeCommand(0x03);
	IO.writeData16(0x1030); 
	
	IO.writeCommand(0x08);
	IO.writeData16(0x0504);
	
	IO.writeCommand(0x09);
	IO.writeData16(0x0000); 
	
	IO.writeCommand(0x0B);
	IO.writeData16(0x0C30);   
	
	IO.writeCommand(0x21);
	IO.writeData16(0x0000);   
	
	IO.writeCommand(0x30);
	IO.writeData16(0x0606);  
	
	IO.writeCommand(0x31);
	IO.writeData16(0x0606);   
	
	IO.writeCommand(0x32);
	IO.writeData16(0x0606);   
	
	IO.writeCommand(0x33);
	IO.writeData16(0x0803);   
	
	IO.writeCommand(0x34);
	IO.writeData16(0x0202);  
	
	IO.writeCommand(0x35);
	IO.writeData16(0x0404);   
	
	IO.writeCommand(0x36);
	IO.writeData16(0x0404); 
	
	IO.writeCommand(0x37);
	IO.writeData16(0x0404);   
	
	IO.writeCommand(0x38);
	IO.writeData16(0x0402); 
	
	IO.writeCommand(0x39);
	IO.writeData16(0x100C);   
	
	IO.writeCommand(0x3A);
	IO.writeData16(0x0101);  
	
	IO.writeCommand(0x3B);
	IO.writeData16(0x0101);  
	
	IO.writeCommand(0x3C);
	IO.writeData16(0x0101); 
	
	IO.writeCommand(0x3D);
	IO.writeData16(0x0101); 
	
	IO.writeCommand(0x3E);
	IO.writeData16(0x0001);  
	
	IO.writeCommand(0x3F);
	IO.writeData16(0x0001);  
	
	IO.writeCommand(0x42);
	IO.writeData16(0xDB00); 
	
	IO.writeCommand(0x44);
	IO.writeData16(0xAF00); 
	
	IO.writeCommand(0x45);
	IO.writeData16(0xDB00);  
	
	HAL_Delay(50);   
	
	IO.writeCommand(0x71);
	IO.writeData16(0x0040);   
	
	IO.writeCommand(0x72);
	IO.writeData16(0x0002);  
	
	HAL_Delay(50);   
	
	IO.writeCommand(0x10);
	IO.writeData16(0x6060);    
	
	IO.writeCommand(0x07);
	IO.writeData16(0x0005);   
	
	HAL_Delay(50);   
	
	IO.writeCommand(0x07);
	IO.writeData16(0x0025);   
	
	IO.writeCommand(0x07);
	IO.writeData16(0x0027);  
	
	HAL_Delay(50);   
	
	IO.writeCommand(0x07);
	IO.writeData16(0x0037);   
	
	IO.writeCommand(0x21);
	IO.writeData16(0x0000);

	IO.writeCommand(0x22);


	HAL_Delay(250);	
  
//Serial.println("INIT GxCTRL_LGDP4524 DONE."); Serial.flush();

//while(1);

	
}

#define swap(type, i, j) {type t = i; i = j; j = t;}

void GxCTRL_LGDP4524::setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	//Serial.println("setWindowAddress");
	
	
	//	код из UTFT
	
	if(rotation & 1) // landscape
	{		
		swap(uint16_t, x0, y0);
		swap(uint16_t, x1, y1);
		
		y0=physical_height-y0;
		y1=physical_height-y1;
		
		swap(uint16_t, y0, y1);
	}
	
	// LCD_Write_COM_DATA(0x44, (x2<<8)+x1); 
	IO.writeCommand(0x44);
	IO.writeData16((x1<<8)+x0);

	// 	LCD_Write_COM_DATA(0x45, (y2<<8)+y1); 
	IO.writeCommand(0x45);
	IO.writeData16((y1<<8)+y0);

// 	LCD_Write_COM_DATA(0x21, (y1<<8)+x1); 
	IO.writeCommand(0x21);
	IO.writeData16((y0<<8)+x0);

	
//	LCD_Write_COM(0x22); 	
    IO.writeCommand(0x22);
	

}

void GxCTRL_LGDP4524::setRotation(uint8_t r)
{
  rotation = r & 3; 
}

