case R61580_8:
		delay(2);
		LCD_Write_COM_DATA(0x00, 0x0000);
		LCD_Write_COM_DATA(0x00, 0x0000);
		LCD_Write_COM_DATA(0x00, 0x0000);
		LCD_Write_COM_DATA(0x00, 0x0000);

		      // Setup display
		LCD_Write_COM_DATA(0xA4, 0x0001);        // CALB=1
		delay(2);
		LCD_Write_COM_DATA(0x60, 0xA700);        // Driver Output Control
		LCD_Write_COM_DATA(0x08, 0x0808);        // Display Control BP=8, FP=8
		LCD_Write_COM_DATA(0x30, 0x0111);        // y control
		LCD_Write_COM_DATA(0x31, 0x2410);        // y control
		LCD_Write_COM_DATA(0x32, 0x0501);        // y control
		LCD_Write_COM_DATA(0x33, 0x050C);        // y control
		LCD_Write_COM_DATA(0x34, 0x2211);        // y control
		LCD_Write_COM_DATA(0x35, 0x0C05);        // y control
		LCD_Write_COM_DATA(0x36, 0x2105);        // y control
		LCD_Write_COM_DATA(0x37, 0x1004);        // y control
		LCD_Write_COM_DATA(0x38, 0x1101);        // y control
		LCD_Write_COM_DATA(0x39, 0x1122);        // y control
		LCD_Write_COM_DATA(0x90, 0x0019);        // 80Hz
		LCD_Write_COM_DATA(0x10, 0x0530);        // Power Control
		LCD_Write_COM_DATA(0x11, 0x0237);
		LCD_Write_COM_DATA(0x12, 0x01BF);
		LCD_Write_COM_DATA(0x13, 0x1300);
		delay(100);

		LCD_Write_COM_DATA(0x01, 0x0100);
		LCD_Write_COM_DATA(0x02, 0x0200);

		LCD_Write_COM_DATA(0x03,0x1030);
		
		LCD_Write_COM_DATA(0x09, 0x0001);
		LCD_Write_COM_DATA(0x0A, 0x0008);
		LCD_Write_COM_DATA(0x0C, 0x0001);
		LCD_Write_COM_DATA(0x0D, 0xD000);
		LCD_Write_COM_DATA(0x0E, 0x0030);
		LCD_Write_COM_DATA(0x0F, 0x0000);
		LCD_Write_COM_DATA(0x20, 0x0000);
		LCD_Write_COM_DATA(0x21, 0x0000);
		LCD_Write_COM_DATA(0x29, 0x0077);
		LCD_Write_COM_DATA(0x50, 0x0000);
		LCD_Write_COM_DATA(0x51, 0xD0EF);
		LCD_Write_COM_DATA(0x52, 0x0000);
		LCD_Write_COM_DATA(0x53, 0x013F);
		LCD_Write_COM_DATA(0x61, 0x0001);
		LCD_Write_COM_DATA(0x6A, 0x0000);
		LCD_Write_COM_DATA(0x80, 0x0000);
		LCD_Write_COM_DATA(0x81, 0x0000);
		LCD_Write_COM_DATA(0x82, 0x005F);
		LCD_Write_COM_DATA(0x93, 0x0701);
		LCD_Write_COM_DATA(0x07, 0x0100);
		LCD_Write_COM(0x22); 
		break;
