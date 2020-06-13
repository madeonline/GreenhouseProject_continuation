#pragma once

#include <Arduino.h>

String formatFloat(float f, uint8_t decimalPlaces)
{
	int n = 1;
	for(uint8_t i=0;i<decimalPlaces;i++)
	{
		n *= 10;
	}

 String formatter;
 formatter = "%d.%0";
 formatter += decimalPlaces;
 formatter += "d";
 
 char buff[50] = {0};
 sprintf(buff,formatter.c_str(), (int32_t)f, abs(int32_t(f*n))%n);
 return buff;
}
