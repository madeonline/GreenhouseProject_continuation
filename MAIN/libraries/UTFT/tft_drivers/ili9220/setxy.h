case ILI9220:
	LCD_Write_COM_DATA(0x21, (y1 << 8) | x1);
	LCD_Write_COM_DATA(0x44, (x2 << 8) | x1);
    LCD_Write_COM_DATA(0x45, (y2 << 8) | y1);
  	LCD_Write_COM(0x22); 							 
	break;
