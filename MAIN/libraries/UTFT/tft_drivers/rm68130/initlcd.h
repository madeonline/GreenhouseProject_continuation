case RM68130:
		delay(50);
		// VCI=2.8V, IOVCC=2.8V --- Gamma 2.4
		//************* Start Initial Sequence **********//
		LCD_Write_COM_DATA(0x28, 0x00CE); // Software Reset
		LCD_Write_COM_DATA(0x01, 0x011C); // driver output control
		LCD_Write_COM_DATA(0x03, 0x1030); // Entry mode
		delay(50);
						// n=1,2,3 бн 262K бн80-system 16-bit
						// n=0 бн 65K бн80-system 16-bit
						// n=1,2 бн 262K бн80-system 8-bit
						// n=0,3 бн 65K бн80-system 8-bit
		LCD_Write_COM_DATA(0x07, 0x0017); // Display control
		LCD_Write_COM_DATA(0x11, 0x1000); // Power control
		LCD_Write_COM_DATA(0x20, 0x0000); // RAM address set(Horizontal Address)
		LCD_Write_COM_DATA(0x21, 0x0000); // RAM address set(Vertical Address)
		LCD_Write_COM_DATA(0x36, 0x00AF); // Windows address position
		LCD_Write_COM_DATA(0x37, 0x0000); // Windows address position
		LCD_Write_COM_DATA(0x38, 0x00DB); // Windows address position
		LCD_Write_COM_DATA(0x39, 0x0000); // Windows address position
		LCD_Write_COM_DATA(0x02, 0x0000); // LCD driving wave control 0 : Column Inversion
		LCD_Write_COM_DATA(0xB0, 0x1412); // Power control(0c12)
		LCD_Write_COM_DATA(0x0B, 0x0000); // Frame Rate Control 4-bit
		//************* Start Gamma Setting **********//
		LCD_Write_COM_DATA(0xE8, 0x0100); // Gamma Command 1 : Gamma Enable
		LCD_Write_COM_DATA(0xB1, 0x0F0F); // +- Gamma Voltage Setting
		LCD_Write_COM_DATA(0x50, 0x0003); // Below : Gamma Setting
		LCD_Write_COM_DATA(0x51, 0x0807);
		LCD_Write_COM_DATA(0x52, 0x0C08);
		LCD_Write_COM_DATA(0x53, 0x0503);
		LCD_Write_COM_DATA(0x54, 0x0003);
		LCD_Write_COM_DATA(0x55, 0x0807);
		LCD_Write_COM_DATA(0x56, 0x0003);
		LCD_Write_COM_DATA(0x57, 0x0503);
		LCD_Write_COM_DATA(0x58, 0x0000);
		LCD_Write_COM_DATA(0x59, 0x0000);
		LCD_Write_COM_DATA(0xE8, 0x0102);
		LCD_Write_COM_DATA(0xFB, 0x002A);
		LCD_Write_COM_DATA(0xE8, 0x0101);
		LCD_Write_COM_DATA(0xF1, 0x0040);
		LCD_Write_COM(0x22); 
		break;
