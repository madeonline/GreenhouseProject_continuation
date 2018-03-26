case R61526_8:
		LCD_Write_COM(0xCB);  //9341
    LCD_Write_DATA(0x39); 
    LCD_Write_DATA(0x2C); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x34); 
    LCD_Write_DATA(0x02); 

    LCD_Write_COM(0xCF);  
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0XC1); 
    LCD_Write_DATA(0X30); 

    LCD_Write_COM(0xE8);  
    LCD_Write_DATA(0x85); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x78); 

    LCD_Write_COM(0xEA);  
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x00); 
 
    LCD_Write_COM(0xED);  
    LCD_Write_DATA(0x64); 
    LCD_Write_DATA(0x03); 
    LCD_Write_DATA(0X12); 
    LCD_Write_DATA(0X81); 

    LCD_Write_COM(0xF7);  
    LCD_Write_DATA(0x20); 
  
    LCD_Write_COM(0xC0);    //Power control 
    LCD_Write_DATA(0x23);   //VRH[5:0] 
 
    LCD_Write_COM(0xC1);    //Power control 
    LCD_Write_DATA(0x10);   //SAP[2:0];BT[3:0] 

    LCD_Write_COM(0xC5);    //VCM control 
    LCD_Write_DATA(0x3e);   //Contrast
    LCD_Write_DATA(0x28); 
 
    LCD_Write_COM(0xC7);    //VCM control2 
    LCD_Write_DATA(0x86);   //--
 
    LCD_Write_COM(0x36);    // Memory Access Control 
    LCD_Write_DATA(0x48);   

    LCD_Write_COM(0x3A);    
    LCD_Write_DATA(0x55); 

    LCD_Write_COM(0xB1);    
    LCD_Write_DATA(0x00);  
    LCD_Write_DATA(0x18); 
 
    LCD_Write_COM(0xB6);    // Display Function Control 
    LCD_Write_DATA(0x08); 
    LCD_Write_DATA(0x82);
    LCD_Write_DATA(0x27);  //9341
	
	LCD_Write_COM(0xB0);
	LCD_Write_DATA(0x3F);
	LCD_Write_DATA(0x3F);
	delay(15);

	LCD_Write_COM(0xB3);
	LCD_Write_DATA(0x02);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xB4);
	LCD_Write_DATA(0x00);

	LCD_Write_COM( 0xC0 );
	LCD_Write_DATA( 0x33);	//03
	LCD_Write_DATA( 0x4F);
	LCD_Write_DATA( 0x00);
	LCD_Write_DATA( 0x10);
	LCD_Write_DATA( 0xA2);
	LCD_Write_DATA( 0x00);
	LCD_Write_DATA( 0x01);
	LCD_Write_DATA( 0x00);

	LCD_Write_COM( 0xC1 );
	LCD_Write_DATA( 0x01);
	LCD_Write_DATA( 0x02);
	LCD_Write_DATA( 0x20);
	LCD_Write_DATA( 0x08);
	LCD_Write_DATA( 0x08);
	delay(15);
	LCD_Write_COM( 0xC3 );
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x28);
	LCD_Write_DATA(0x08);
	LCD_Write_DATA(0x08);
	delay(15);

	LCD_Write_COM(0xC4);
	LCD_Write_DATA(0x11 );
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0x23);
	LCD_Write_DATA(0x04);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xC8);//Gamma
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x0C);
	LCD_Write_DATA(0x0b);
	LCD_Write_DATA(0x15);
	LCD_Write_DATA(0x11);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x13);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x20);

	LCD_Write_DATA(0x13);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x11);
	LCD_Write_DATA(0x15);
	LCD_Write_DATA(0x0b);
	LCD_Write_DATA(0x0c);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x02);

	
	LCD_Write_COM(0xC9);//Gamma
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x0C);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x15);
	LCD_Write_DATA(0x11 );
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x13);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x20);
	LCD_Write_DATA(0x13);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x11);
	LCD_Write_DATA(0x15);
	LCD_Write_DATA(0x0b);
	LCD_Write_DATA(0x0c);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x02);

	LCD_Write_COM(0xCA);//Gamma
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x0C);
	LCD_Write_DATA(0x0b);
	LCD_Write_DATA(0x15);
	LCD_Write_DATA(0x11);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x13);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x20);
	LCD_Write_DATA(0x13);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x11);
	LCD_Write_DATA(0x15);
	LCD_Write_DATA(0x0b);
	LCD_Write_DATA(0x0c);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x02);

	LCD_Write_COM(0xD0);
	LCD_Write_DATA(0x33);
	LCD_Write_DATA(0x53);
	LCD_Write_DATA(0x87);
	LCD_Write_DATA(0x3b);
	LCD_Write_DATA(0x30);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xD1 );
	LCD_Write_DATA(0x2c);
	LCD_Write_DATA(0x61);
	LCD_Write_DATA(0x10);

	LCD_Write_COM(0xD2 );
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0x24);

	LCD_Write_COM(0xD4 );
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0x24);


	LCD_Write_COM(0xE2);
	LCD_Write_DATA(0x3f);
	delay(15);

	LCD_Write_COM(0x35 );
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0x36 );
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0x3A );
	LCD_Write_DATA(0x55);//55 16bit color

	LCD_Write_COM(0x2A );
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0xEF);

	LCD_Write_COM(0x2B );
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0x3F);

	LCD_Write_COM(0x11) ;
	delay(15);
	LCD_Write_COM(0x29);
	delay(15);
	LCD_Write_COM(0x2C) ;
	delay(15);
		break;
