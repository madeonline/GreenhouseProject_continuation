case HX8352B_8:
	delay(200);
	// Register setting for EQ setting
	LCD_Write_COM_DATA(0xe5, 0x10);
    LCD_Write_COM_DATA(0xe7, 0x10);
	LCD_Write_COM_DATA(0xe8, 0x10);
	LCD_Write_COM_DATA(0xec, 0x48);
	LCD_Write_COM_DATA(0xed, 0x6c);

	// Power on Setting  
	LCD_Write_COM_DATA(0x23,0x6F);    
	LCD_Write_COM_DATA(0x24,0x57);    
	LCD_Write_COM_DATA(0x25,0x71);    
	LCD_Write_COM_DATA(0xE2,0x18);    
	LCD_Write_COM_DATA(0x1B,0x15);    
	LCD_Write_COM_DATA(0x01,0x00);    
	LCD_Write_COM_DATA(0x1C,0x03);    

	// Power on sequence   
	LCD_Write_COM_DATA(0x19,0x01);    
	delay(5);   
	LCD_Write_COM_DATA(0x1F,0x8C);   
	LCD_Write_COM_DATA(0x1F,0x84);   
	delay(10);  
	LCD_Write_COM_DATA(0x1F,0x94);   
	delay(10);  
	LCD_Write_COM_DATA(0x1F,0xD4);  
	delay(5);

	// Gamma Setting   
	LCD_Write_COM_DATA(0x40,0x00);  
	LCD_Write_COM_DATA(0x41,0x2B);  
	LCD_Write_COM_DATA(0x42,0x29);  
	LCD_Write_COM_DATA(0x43,0x3E);  
	LCD_Write_COM_DATA(0x44,0x3D);  
	LCD_Write_COM_DATA(0x45,0x3F); 
	LCD_Write_COM_DATA(0x46,0x24);  
	LCD_Write_COM_DATA(0x47,0x74);  
	LCD_Write_COM_DATA(0x48,0x08);  
	LCD_Write_COM_DATA(0x49,0x06);  
	LCD_Write_COM_DATA(0x4A,0x07);  
	LCD_Write_COM_DATA(0x4B,0x0D);  
	LCD_Write_COM_DATA(0x4C,0x17);  
    LCD_Write_COM_DATA(0x50,0x00);  
	LCD_Write_COM_DATA(0x51,0x02);  
	LCD_Write_COM_DATA(0x52,0x01);  
	LCD_Write_COM_DATA(0x53,0x16);  
	LCD_Write_COM_DATA(0x54,0x14);  
	LCD_Write_COM_DATA(0x55,0x3F);  
	LCD_Write_COM_DATA(0x56,0x0B);  
	LCD_Write_COM_DATA(0x57,0x5B);  
	LCD_Write_COM_DATA(0x58,0x08);  
	LCD_Write_COM_DATA(0x59,0x12);   
	LCD_Write_COM_DATA(0x5A,0x18);  
	LCD_Write_COM_DATA(0x5B,0x19);  
	LCD_Write_COM_DATA(0x5C,0x17);  
	LCD_Write_COM_DATA(0x5D,0xFF);  


//	LCD_Write_COM(0xFF); LCD_Write_DATA(0x00);
	//--------------------------------------------
    LCD_Write_COM_DATA(0x16,0x08);     
	LCD_Write_COM_DATA(0x28,0x20);    
	delay(40);  
	LCD_Write_COM_DATA(0x28,0x38);     
	delay(40);                 // Waiting 2 frames al least 
	LCD_Write_COM_DATA(0x28,0x3C);  
	 
	LCD_Write_COM(0x02);           
	LCD_Write_DATA(0x00);
	LCD_Write_COM(0x03);           
	LCD_Write_DATA(0x00);
	LCD_Write_COM(0x04);           
	LCD_Write_DATA(0x00);
	LCD_Write_COM(0x05);           
	LCD_Write_DATA(0xef);
        
	LCD_Write_COM(0x06);           
	LCD_Write_DATA(0x00);
	LCD_Write_COM(0x07);           
	LCD_Write_DATA(0x00);
	LCD_Write_COM(0x08);           
	LCD_Write_DATA(0x01);
	LCD_Write_COM(0x09);           
	LCD_Write_DATA(0x8f);

	LCD_Write_COM(0x17);           
	LCD_Write_DATA(0x05);

	
	LCD_Write_COM(0x22);
	break;
