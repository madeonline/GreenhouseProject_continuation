case R61580_8:
	LCD_Write_COM(0x20); 
	LCD_Write_DATA(x1>>8);
	LCD_Write_DATA(x1);
	LCD_Write_DATA(x2>>8);
	LCD_Write_DATA(x2);
	LCD_Write_COM(0x21); 
	LCD_Write_DATA(y1>>8);
	LCD_Write_DATA(y1);
	LCD_Write_DATA(y2>>8);
	LCD_Write_DATA(y2);
	LCD_Write_COM(0x22); 
	break;
