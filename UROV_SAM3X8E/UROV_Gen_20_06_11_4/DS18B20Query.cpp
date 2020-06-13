#include "DS18B20Query.h"
#include "CONFIG.h"
#include "Settings.h"
#include <OneWire.h>
//--------------------------------------------------------------------------------------------------------------------------------------
_ds_temp::operator String() const
{
  if(!hasData())
  {
    return "-";
  }
  
  static char VAL_BUFFER[10] = {0};  
  sprintf_P(VAL_BUFFER,(const char*) F("%s%d,%02u"), Negative ? "-" : "", Whole,Fract);
  return VAL_BUFFER;  
}
//--------------------------------------------------------------------------------------------------------------------------------------
DS18B20Dispatcher DS18B20LineManager;
//--------------------------------------------------------------------------------------------------------------------------------------
DS18B20Dispatcher::DS18B20Dispatcher()
{
	
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::begin()
{
	savedBindings.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::addBinding(uint8_t pin, uint8_t sensorIndex)
{
	DS18B20Binding bnd;
	memset(&bnd,0xFF,sizeof(DS18B20Binding));
	
	bnd.pin = pin;
	bnd.index = sensorIndex;
	
	// теперь вычитываем с EEPROM сохранённый адрес для этой записи
	int readAddr = DS18B20_BINDING_ADDRESS + savedBindings.size()*sizeof(DS18B20Binding);
	DS18B20Binding saved;
	uint8_t* writePtr = (uint8_t*)&saved;
	for(size_t i=0;i<sizeof(DS18B20Binding);i++)
	{
		*writePtr++ = Settings.read(readAddr++);
	}
	
	// копируем сохранённый адрес
	memcpy(bnd.address,saved.address,sizeof(saved.address));
	
	savedBindings.push_back(bnd);
	
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool DS18B20Dispatcher::alreadyConverted(uint8_t pin)
{
	for(size_t i=0;i<conversionList.size();i++)
	{
		if(conversionList[i] == pin)
    {
			return true;
    }
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::beginConversion()
{
 #ifdef DS18B20_DEBUG
	DEBUG_LOGLN(F("DS18B20: begin conversion..."));
 #endif
	
	conversionList.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::startConversion(uint8_t pin)
{
	if(!pin)
  {
		return;
  }
	
	if(alreadyConverted(pin))
	{	
		return;
	}
#ifdef DS18B20_DEBUG	
	DEBUG_LOG(F("DS18B20: start conversion on pin "));
	DEBUG_LOGLN(String(pin));
#endif
	
	conversionList.push_back(pin);

	OneWire ow(pin);

	if(!ow.reset()) // нет датчика
	{
#ifdef DS18B20_DEBUG  
		DEBUG_LOG(F("DS18B20: no sensors found on pin "));
		DEBUG_LOGLN(String(pin));
#endif
		return;
	}

#ifdef DS18B20_DEBUG
	DEBUG_LOG(F("DS18B20: start conversion on pin "));
	DEBUG_LOGLN(String(pin));
#endif

	ow.write(0xCC); // пофиг на адреса (SKIP ROM)
	ow.write(0x44); // запускаем преобразование

	ow.reset();		
	
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool DS18B20Dispatcher::alreadySetResolution(uint8_t pin)
{
	for(size_t i=0;i<resolutionList.size();i++)
	{
		if(resolutionList[i] == pin)
    {
			return true;
    }
	}
	return false;	
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::beginSetResolution()
{
#ifdef DS18B20_DEBUG  
	DEBUG_LOGLN(F("DS18B20: begin set resolution..."));
#endif
	
	resolutionList.empty();	
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::setResolution(uint8_t pin,DS18B20Resolution res)
{
	if(!pin)
  {
		return;
  }

if(alreadySetResolution(pin))
 {
		return;
 }
#ifdef DS18B20_DEBUG	
	DEBUG_LOG(F("DS18B20: set resolution on pin "));
	DEBUG_LOGLN(String(pin));
#endif
  
	resolutionList.push_back(pin);	

	OneWire ow(pin);


	if(!ow.reset()) // нет датчика
	{
#ifdef DS18B20_DEBUG  
		DEBUG_LOG(F("DS18B20: no sensors found on pin "));
  	DEBUG_LOGLN(String(pin));		
#endif		
		return;  
	}

	ow.write(0xCC); // пофиг на адреса (SKIP ROM)
	ow.write(0x4E); // запускаем запись в scratchpad

	ow.write(0); // верхний температурный порог 
	ow.write(0); // нижний температурный порог
	ow.write(res); // разрешение датчика

	ow.reset();
	ow.write(0xCC); // пофиг на адреса (SKIP ROM)
	ow.write(0x48); // COPY SCRATCHPAD
	delay(10);
	ow.reset();	
	
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool DS18B20Dispatcher::alreadyScanned(uint8_t pin)
{
	for(size_t i=0;i<scanList.size();i++)
	{
		if(scanList[i] == pin)
    {
			return true;
    }
	}
	return false;		
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::beginScan()
{
#ifdef DS18B20_DEBUG  
	DEBUG_LOGLN(F("DS18B20: begin scan lines..."));
#endif
	
	scanList.empty();
	scanResults.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::scan(uint8_t pin)
{
	if(!pin)
  {
		return;
  }

	if(alreadyScanned(pin))
  {
		return;
  }
#ifdef DS18B20_DEBUG	
	DEBUG_LOG(F("DS18B20: scan on pin "));
	DEBUG_LOGLN(String(pin));
#endif
	
	scanList.push_back(pin);	

	OneWire ow(pin);

	if(!ow.reset()) // нет датчика
	{
#ifdef DS18B20_DEBUG  
		DEBUG_LOG(F("DS18B20: no sensors found on pin "));
		DEBUG_LOGLN(String(pin));		
#endif		
		return;  
	}

	// тут сканируем линию
  byte addr[8];
  ow.reset_search();
  
  while(ow.search(addr))
  {
	  // получили следующий адрес на линии
	  if(OneWire::crc8(addr, 7) == addr[7])
	  {
		  // хороший адрес, надо сохранить этот адрес у себя, чтобы потом его использовать
		  DS18B20Binding bnd;
		  memset(&bnd,0xFF,sizeof(DS18B20Binding));
		  bnd.pin = pin;
		  memcpy(bnd.address,addr,sizeof(addr));
      
#ifdef DS18B20_DEBUG		  
		  DEBUG_LOG(F("DS18B20: found sensor on pin "));
		  DEBUG_LOG(String(pin));
		  DEBUG_LOG(F(" with address "));
		  for(size_t i=0;i<sizeof(addr);i++)
		  {
			  DEBUG_LOG(String(addr[i],HEX));
			  DEBUG_LOG(" ");
		  }
		  
		  DEBUG_LOGLN("");
#endif // DS18B20_DEBUG
		  
		  scanResults.push_back(bnd);
	  }
	  
  } // while

#ifdef DS18B20_DEBUG  
  DEBUG_LOG(F("DS18B20: scan done, total sensors found: "));
  DEBUG_LOGLN(String(scanResults.size()));
#endif
  
  // теперь у нас есть набор адресов датчиков, которые отзываются, на каждом пине из настроек

 	
}
//--------------------------------------------------------------------------------------------------------------------------------------
int DS18B20Dispatcher::findBinding(uint8_t sensorIndex, uint8_t pin)
{
	uint8_t address[8];
	memset(address,0xFF,sizeof(address));
	
	for(size_t i=0;i<savedBindings.size();i++)
	{
		if(savedBindings[i].pin == pin && savedBindings[i].index == sensorIndex)
		{
			if(memcmp(address,savedBindings[i].address,sizeof(address))) // есть сохранённый адрес
      {
				return i;
      }
			
			break;
		}
	}
	
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool DS18B20Dispatcher::inScanResults(uint8_t pin, const DS18B20Binding& binding)
{
	for(size_t i=0;i<scanResults.size();i++)
	{
		if(scanResults[i].pin == pin && !memcmp(binding.address,scanResults[i].address,sizeof(binding.address))) // датчик - в откликнувшихся на линии
    {
			return true;
    }
	}
	
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::resetBinding(int recordIndex)
{
	if(recordIndex < 0 || recordIndex >= savedBindings.size())
  {
		return;
  }
	
	// сбрасываем адрес в ноль
	memset(savedBindings[recordIndex].address,0xFF,sizeof(savedBindings[recordIndex].address));	
	saveBinding(recordIndex);
	
}
//--------------------------------------------------------------------------------------------------------------------------------------
int DS18B20Dispatcher::findFree(uint8_t pin, int& scanResultsIndex) // ищем первый датчик на линии, для которого отсутствуют привязки к индексу
{
	scanResultsIndex = -1;
	
	uint8_t address[8];
	memset(address,0xFF,sizeof(address));
	
	for(size_t i=0;i<scanResults.size();i++)
	{
		if(scanResults[i].pin == pin)
		{
			bool found = false;
			for(size_t k=0;k<savedBindings.size();k++)
			{
				if(savedBindings[k].pin == pin && !memcmp(scanResults[i].address,savedBindings[k].address,sizeof(scanResults[i].address)))
				{
					found = true;
					break;
				}
			} // for
			
			if(!found)
			{
				// нет датчика на линии - с таким адресом в привязках. Ищем первый свободный слот в привязках.
				for(size_t k=0;k<savedBindings.size();k++)
				{
					if(savedBindings[k].pin == pin && !memcmp(address,savedBindings[k].address,sizeof(address)))
					{
						scanResultsIndex = i;
						return k;
					}
				}
			}
		}
	} // for
	
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void DS18B20Dispatcher::saveBinding(int recordIndex)
{
	//СОХРАНЕНИЕ В EEPROM
	int writeAddr = DS18B20_BINDING_ADDRESS + recordIndex*sizeof(DS18B20Binding);
	uint8_t* readAddr = (uint8_t*)&(savedBindings[recordIndex]);
	
	for(size_t i=0;i<sizeof(DS18B20Binding);i++)
	{
		Settings.write(writeAddr++,*readAddr++);
	}	
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool DS18B20Dispatcher::getTemperature(uint8_t sensorIndex, uint8_t pin, DS18B20Temperature& result)
{
	
	result.Whole = NO_TEMPERATURE_DATA; // нет данных с датчика
	result.Fract = 0;
	result.Negative = false;

#ifdef DS18B20_DEBUG  
	DEBUG_LOG(F("DS18B20: request temperature on pin "));
	DEBUG_LOG(String(pin));
	DEBUG_LOG(F(" and sensor index "));	
	DEBUG_LOGLN(String(sensorIndex));
#endif
	
	bool address_found = false;
	byte addr[8]; // адрес, который ищем	

	// у нас запросили температуру с датчика по индексу, и всё, что мы знаем - это пин, на котором он висит.
	// нам надо для начала посмотреть - есть ли у нас в сохранённых привязках адрес для датчика с таким пином
	// и таким индексом.
	int recordIndex = findBinding(sensorIndex,pin);
	// Если есть - то проверяем - есть ли такой датчик в откликнувшихся. Если датчик не откликнулся - то сбрасываем ему привязку к адресу.
	
	if(recordIndex != -1) // нашли в сохранённых ранее привязках адреса к индексу
	{
#ifdef DS18B20_DEBUG  
		DEBUG_LOG(F("DS18B20: found saved binding at index "));
		DEBUG_LOG(String(recordIndex));
		DEBUG_LOG(F(" address "));
		  for(size_t i=0;i<sizeof(savedBindings[recordIndex].address);i++)
		  {
			  DEBUG_LOG(String(savedBindings[recordIndex].address[i],HEX));
			  DEBUG_LOG(" ");
		  }
        DEBUG_LOGLN("");
#endif // DS18B20_DEBUG

		
		if(inScanResults(pin,savedBindings[recordIndex]))
		{
			// датчик есть в откликнувшихся на линии
#ifdef DS18B20_DEBUG      
			DEBUG_LOG(F("DS18B20: sensor exists on line "));
			DEBUG_LOGLN(String(pin));
#endif
			
			// говорим, что нашли датчик, копируем его адрес для дальнейшего опроса
			address_found = true;
			memcpy(addr,savedBindings[recordIndex].address,sizeof(addr));
		}
		else
		{
			// датчика нет в откликнувшихся на линии
#ifdef DS18B20_DEBUG			
			DEBUG_LOG(F("DS18B20: sensor NOT exists on line "));
			DEBUG_LOGLN(String(pin));
#endif
			
			// сбрасываем его привязку к адресу
			resetBinding(recordIndex);
		}
	}
	else // не нашли привязку датчика к адресу
	{
#ifdef DS18B20_DEBUG  
		DEBUG_LOG(F("DS18B20: NO BINDING FOUND FOR SENSOR INDEX "));
		DEBUG_LOGLN(String(sensorIndex));	
#endif
		// Если датчика нет в сохранённых привязках - то мы туда помещаем первый откликнувшийся на линии, но НЕ ИМЕЮЩИЙ привязок до этого.
		// по итогу - мы получаем адрес для сканирования.
		int scanResultsIndex = -1;
		int freeBindingIndex = findFree(pin, scanResultsIndex);
		
		if(scanResultsIndex > -1 && freeBindingIndex > -1)
		{
			// нашли свободный слот, сохраняем туда адрес датчика на линии
#ifdef DS18B20_DEBUG      
			DEBUG_LOG(F("DS18B20: found free slot in binding: "));
			DEBUG_LOG(String(freeBindingIndex));
			DEBUG_LOG(F("; scan index = "));
			DEBUG_LOGLN(String(scanResultsIndex));
#endif
			
			memcpy(savedBindings[freeBindingIndex].address,scanResults[scanResultsIndex].address,sizeof(savedBindings[freeBindingIndex].address));
			saveBinding(freeBindingIndex);
			
			// говорим, что нашли адрес
			address_found = true;
			memcpy(addr,scanResults[scanResultsIndex].address,sizeof(addr));
		}
	}
	
	
	if(address_found) // нашли адрес
	{
		  OneWire ow(pin);

		  if(!ow.reset()) // нет датчика
		  {
#ifdef DS18B20_DEBUG      
			DEBUG_LOG(F("DS18B20: no sensors found on pin "));
			DEBUG_LOGLN(String(pin));
#endif 
			return false;
		  }
		  
		  uint8_t type_s;

		  switch (addr[0]) 
		  {
			case 0x10:
#ifdef DS18B20_DEBUG      
			  DEBUG_LOGLN(F("DS18B20: Chip = DS18S20"));  // or old DS1820
#endif  
			  type_s = 1;
			  break;
			case 0x28:
#ifdef DS18B20_DEBUG      
				DEBUG_LOGLN(F("DS18B20: Chip = DS18B20"));
#endif  
			  type_s = 0;
			  break;
			case 0x22:
#ifdef DS18B20_DEBUG      
				DEBUG_LOGLN(F("DS18B20: Chip = DS1822"));
#endif
			  type_s = 0;
			  break;
			default:
#ifdef DS18B20_DEBUG      
				DEBUG_LOGLN(F("DS18B20: Device is not a DS18x20 family device."));
#endif
			  return false;
		  }   

		  ow.select(addr);
		  
		  ow.write(0xBE); // читаем scratchpad датчика на пине
		  byte data[9];

		  for(uint8_t i=0;i<9;i++)
			data[i] = ow.read();


		 if (OneWire::crc8( data, 8) != data[8]) // проверяем контрольную сумму
		 {
#ifdef DS18B20_DEBUG      
			 DEBUG_LOGLN(F("DS18B20: CRC NOT VALID!"));
#endif 
			  return false;
		 }

		 // Convert the data to actual temperature
		  // because the result is a 16 bit signed integer, it should
		  // be stored to an "int16_t" type, which is always 16 bits
		  // even when compiled on a 32 bit processor.
		  int16_t raw = (data[1] << 8) | data[0];
		  if (type_s) 
		  {
			raw = raw << 3; // 9 bit resolution default
			if (data[7] == 0x10) 
			{
			  // "count remain" gives full 12 bit resolution
			  raw = (raw & 0xFFF0) + 12 - data[6];
			}
		  } 
		  else 
		  {
			byte cfg = (data[4] & 0x60);
			// at lower res, the low bits are undefined, so let's zero them
			if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
			else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
			else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
			//// default is 12 bit resolution, 750 ms conversion time
		  }
		  
		  float celsius = ((float) raw / 16.0)*100.0;
		  int32_t tc_100 = celsius;
		  
		  result.Negative = tc_100 < 0;
		  result.Whole = abs(tc_100/100);
		  result.Fract = abs(tc_100 % 100);


		  if(result.Whole < -55 || result.Whole > 125)
		  {
			result.Negative = false;
			result.Whole = NO_TEMPERATURE_DATA;
			result.Fract = 0;
		  }		  
				
	}
	
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
// TempSensorsClass
//--------------------------------------------------------------------------------------------------------------------------------------
const uint8_t DS_PINS[] = { DS18B20_PINS };
//--------------------------------------------------------------------------------------------------------------------------------------
TempSensorsClass TempSensors;
//--------------------------------------------------------------------------------------------------------------------------------------
TempSensorsClass::TempSensorsClass()
{
  lastMillis = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
size_t TempSensorsClass::getSensorsCount()
{
  return sizeof(DS_PINS)/sizeof(DS_PINS[0]);
}
//--------------------------------------------------------------------------------------------------------------------------------------
DS18B20Temperature TempSensorsClass::getTemperature(size_t sensorIndex)
{
  DS18B20Temperature result;

  if(temperatures.size() && sensorIndex < temperatures.size())
  {
    result = temperatures[sensorIndex];
  }


  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TempSensorsClass::begin()
{
   DS18B20LineManager.begin();
   DS18B20LineManager.beginConversion();
   DS18B20LineManager.beginSetResolution();

   size_t cnt = getSensorsCount();

   DS18B20Temperature emptyT;

   for(size_t i=0;i<cnt;i++)
   {
      uint8_t pin = DS_PINS[i];

       // добавляем привязку
      DS18B20LineManager.addBinding(pin,i);
      
      DS18B20LineManager.setResolution(pin,temp12bit);
      temperatures.push_back(emptyT);

      // запускаем конвертацию с датчиков при старте, через N миллисекунд нам вернётся измеренная температура
      DS18B20LineManager.startConversion(pin);
      
   }
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TempSensorsClass::update()
{
  if(millis() - lastMillis >= TEMPERATURE_UPDATE_INTERVAL) // пришло время опрашивать датчики
  {

    // запускаем конвертацию датчиков, игнорируя повторные вызовы для одной линии
     DS18B20LineManager.beginConversion();


     size_t cnt = getSensorsCount();
     for(size_t i=0;i<cnt;i++)
     {

         uint8_t pin = DS_PINS[i];

        DS18B20LineManager.startConversion(pin);
     } // for

     // теперь сканируем линии
     DS18B20LineManager.beginScan();

     for(size_t i=0;i<cnt;i++)
     {
        uint8_t pin = DS_PINS[i];
         DS18B20LineManager.scan(pin);
     } // for


     // теперь - читаем датчики
     for(size_t i=0;i<cnt;i++)
     {
        uint8_t pin = DS_PINS[i];

        DS18B20Temperature tempData;
        if(DS18B20LineManager.getTemperature(i,pin, tempData))
        {
          
        }
        // теперь запоминаем значение датчика
        temperatures[i] = tempData;
     } // for
     

    lastMillis = millis();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------

