
// *** Hardwarespecific functions ***
void UTFT::_hw_special_init()
{

}

void UTFT::LCD_Writ_Bus8(char VH,char VL, byte mode)
{
	switch (mode)
	{
	case 1:
		if (display_serial_mode==SERIAL_4PIN)
		{
		if (VH==1)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		}
		else //SERIAL_5PIN
		{
		if (VH==1)
			sbi(P_RS, B_RS);
		else
			cbi(P_RS, B_RS);
		}

		if (VL & 0x80)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x40)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x20)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x10)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x08)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x04)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x02)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x01)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		break;
	case 8:
//		PORTD = VH;
//		pulse_low(P_WR, B_WR);
//		PORTD = VL;
//		pulse_low(P_WR, B_WR);
        #ifdef USE_OPENSMART_SHIELD_PINOUT
		PORTD = (PORTD & B00101111) | ((VL) & B11010000);
		PORTB = (PORTB & B11010000) | ((VL) & B00101111);
		pulse_low(P_WR, B_WR);
		#else
  		PORTB = VL & 0x3;  
  		PORTD = VL & 0xFD;  
  		pulse_low(P_WR, B_WR);
		#endif
		break;
	case 16:
		PORTD = VH;
		cport(PORTC, 0xFC);
		sport(PORTC, (VL>>6) & 0x03);
		PORTB =  VL & 0x3F;
		pulse_low(P_WR, B_WR);
		break;
	case LATCHED_16:
		PORTD = VH;
		cbi(P_ALE, B_ALE);
		pulse_high(P_ALE, B_ALE);
		cbi(P_CS, B_CS);
		PORTD =  VL;
		pulse_low(P_WR, B_WR);
		sbi(P_CS, B_CS);
		break;
	}
}

void UTFT::LCD_Writ_Bus16(char VH,char VL, byte mode)
{
  switch (mode)
  {
  case 1:
    if (display_serial_mode==SERIAL_4PIN)
    {
    if (VH==1)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    }
    else //SERIAL_5PIN
    {
    if (VH==1)
      sbi(P_RS, B_RS);
    else
      cbi(P_RS, B_RS);
    }

    if (VL & 0x80)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    if (VL & 0x40)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    if (VL & 0x20)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    if (VL & 0x10)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    if (VL & 0x08)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    if (VL & 0x04)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    if (VL & 0x02)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    if (VL & 0x01)
      sbi(P_SDA, B_SDA);
    else
      cbi(P_SDA, B_SDA);
    pulse_low(P_SCL, B_SCL);
    break;
  case 8:
//    PORTD = VH;
//    pulse_low(P_WR, B_WR);
//    PORTD = VL;
//    pulse_low(P_WR, B_WR);
     #ifdef USE_OPENSMART_SHIELD_PINOUT
		PORTD = (PORTD & B00101111) | ((VH) & B11010000);
		PORTB = (PORTB & B11010000) | ((VH) & B00101111);
		pulse_low(P_WR, B_WR);
		PORTD = (PORTD & B00101111) | ((VL) & B11010000);
		PORTB = (PORTB & B11010000) | ((VL) & B00101111);
		pulse_low(P_WR, B_WR);
	
		#else
  		PORTB = VH & 0x3;  
  		PORTD = VH & 0xFD;  
  		pulse_low(P_WR, B_WR);
  		
  		PORTB = VL & 0x3;  
  		PORTD = VL & 0xFD;  
  		pulse_low(P_WR, B_WR);
		#endif
    break;
  case 16:
    PORTD = VH;
    cport(PORTC, 0xFC);
    sport(PORTC, (VL>>6) & 0x03);
    PORTB =  VL & 0x3F;
    pulse_low(P_WR, B_WR);
    break;
  case LATCHED_16:
    PORTD = VH;
    cbi(P_ALE, B_ALE);
    pulse_high(P_ALE, B_ALE);
    cbi(P_CS, B_CS);
    PORTD =  VL;
    pulse_low(P_WR, B_WR);
    sbi(P_CS, B_CS);
    break;
  }
}

void UTFT::_set_direction_registers(byte mode)
{
//	DDRD = 0xFF;
//	if (mode==16)
//	{
//		DDRB |= 0x3F;
//		DDRC |= 0x03;
//	}
    #ifdef USE_OPENSMART_SHIELD_PINOUT
	DDRD |=  B11010000; 
	DDRB |=  B00101111;
	#else
	for(int p=2;p<10;p++)
 	 {
	    pinMode(p,OUTPUT);
	  }
   #endif

}

void UTFT::_fast_fill_16(int ch, int cl, long pix)
{
	long blocks;

	PORTD = ch;
	cport(PORTC, 0xFC);
	sport(PORTC, (cl>>6) & 0x03);
	PORTB =  cl & 0x3F;

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(P_WR, B_WR);
		}
}

void UTFT::_fast_fill_8(int ch, long pix)
{
	long blocks;
    #ifdef USE_OPENSMART_SHIELD_PINOUT
	PORTD = (PORTD & B00101111) | ((ch) & B11010000);
	PORTB = (PORTB & B11010000) | ((ch) & B00101111);
	#else
//	PORTD = ch;
  		PORTB = ch & 0x3;  
  		PORTD = ch & 0xFD;  
    #endif
	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		}
}
