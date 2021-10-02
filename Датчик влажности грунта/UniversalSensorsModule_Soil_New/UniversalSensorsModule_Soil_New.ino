//----------------------------------------------------------------------------------------------------------------
/*
�������� ��� �������������� ������, ������������� ��� �����������
������ ���� �������������� �������� � �������� � ��� ��������� �� ���� 1-Wire.


����� �������������� ������ �� �����������, ��������� ������ nRF24L01+,
��� ���� ����������� ���������������� USE_NRF.

����� �������������� ������ �� �����������, ��������� ������ LoRa,
��� ���� ����������� ���������������� USE_LORA.

*/
//----------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <OneWire.h>
#include "BH1750.h"
#include "Max44009.h"
#include "UniGlobals.h"
#include "Si7021Support.h"
#include "DHTSupport.h"
#include "LowLevel.h"
#include "OneWireSlave.h"
#include "SHT1x.h"
#include "LowPower.h"
#include <PWM.h>

//----------------------------------------------------------------------------------------------------------------
/*
 ����, ������� ���������� ����� ������ � ���������:
 
  D2 - ����� ����������� ������ � ������� (1-Wire)
  D3 - ����� ��������� ������� ���������� �������
  A0, A1 - ������ ��������� � DS18B20, ��������� 4K7
  D4  - ������������ ������ ����/�������� ��� RS485
  D0, D1 = RX/TX RS485
  D5, D6 - � ���������� � ������� �� 10�
  A4, A5 - I2C
 */
//----------------------------------------------------------------------------------------------------------------
// ��������! ��� ����� 2 - �� ��������, ����� ���� �������� 1-Wire!
//----------------------------------------------------------------------------------------------------------------
// ���������������� ���������
//----------------------------------------------------------------------------------------------------------------
#define SOIL_MOISTURE_0_PERCENT 530// ������� ��� 0% ��������� �����, ��������� ������ ��������� (0-1023)
#define SOIL_MOISTURE_100_PERCENT 200 // ������� ��� 100% ��������� �����, ��������� ������ ��������� (0-1023)
//----------------------------------------------------------------------------------------------------------------
// ��������� ����������� �����
//----------------------------------------------------------------------------------------------------------------
//#define USE_POWER_SAVING // ����������������, ���� �� ���� ������������ ����� ����������������
#define LINES_POWER_DOWN_PIN 8 // ����� ����, �� ������� ����� ���������� �������� ����� I2C � ����� ��������
#define LINES_POWER_DOWN_LEVEL LOW // ������� �� ���� ��� ���������� �����
#define LINES_POWER_UP_LEVEL HIGH // ������� �� ���� ��� ��������� �����
#define LINES_POWER_UP_DELAY 20 // ������� ����������� ����� ������������ �������
//----------------------------------------------------------------------------------------------------------------
#define _DEBUG // ����������������� ��� ����������� ������ (������� � Serial, �� ������������ � ������������ RS-485 !!!)
//----------------------------------------------------------------------------------------------------------------
// ��������� ���������������� (�� �����������, ����� ������������ USE_RS485_GATE)
//----------------------------------------------------------------------------------------------------------------
//#define USE_LOW_POWER // ����������������, ���� �� ���� �������� ����� ������� ��������� �� �����������.
// ��������� ��� �� �������, �.�. ������������ ������ �����������.
//----------------------------------------------------------------------------------------------------------------
// ��������� nRF
//----------------------------------------------------------------------------------------------------------------
//#define USE_NRF // ����������������, ���� �� ���� �������� ����� nRF.
//----------------------------------------------------------------------------------------------------------------
/*
 nRF ��� ����� ������ �������� ��������� ����: 9,10,11,12,13. 
 ������� �� ���, ����� ������ ����� �� ������������ c �������� ����� ��������, ��� � RS-485.
 */
#define NRF_CE_PIN 9 // ����� ���� CE ��� ������ nRF
#define NRF_CSN_PIN 10 // ����� ���� CSN ��� ������ nRF
#define DEFAULT_RF_CHANNEL 19 // ����� ������ ��� nRF �� ���������
#define NRF_AUTOACK_INVERTED // ����������������� ��� ������� ����� � � ������� ��������, ���� � ��� ��� �� �����������. 
// ������ auto a�k � ��������� ������� ����� ��������������� ��������.

//----------------------------------------------------------------------------------------------------------------
// ��������� LoRa
//----------------------------------------------------------------------------------------------------------------
#define USE_LORA // ����������������, ���� �� ���� �������� ����� LoRa.
/*
 LoRa ��� ����� ������ �������� ��������� ����: 9,10,11,12,13. 
 ������� �� ���, ����� ������ ����� �� ������������ � ������, ��� � RS-485, ��� ��� ���.
 */
#define LORA_SS_PIN 10 // ��� SS ��� LoRa
#define LORA_RESET_PIN 9 // ��� Reset ��� LoRa
#define LORA_FREQUENCY 868E6 // ������� ������ (433E6, 868E6, 915E6)
#define LORA_TX_POWER 17 // �������� ����������� (1 - 17)

//----------------------------------------------------------------------------------------------------------------
// ��������� RS-485
//----------------------------------------------------------------------------------------------------------------
//#define USE_RS485_GATE // ����������������, ���� �� ����� ������ ����� RS-485
//----------------------------------------------------------------------------------------------------------------
#define RS485_SPEED 57600 // �������� ������ �� RS-485
#define RS485_DE_PIN 4 //v ����� ����, �� ������� ����� ��������� ������������ ����/�������� �� RS-485


//----------------------------------------------------------------------------------------------------------------

#define PWM_PIN 3 // ����� ����, �� ������� ����� ���������
unsigned long frequency = 2000000; // �������  1 - 2000000 (��)
int brightness = 150;         // ������� ��� (0-255)  


//----------------------------------------------------------------------------------------------------------------
// ��������� �������� ��� ������, ������ �����!
//----------------------------------------------------------------------------------------------------------------
const SensorSettings Sensors[3] = {
       
 {mstChinaSoilMoistureMeter,A0,0},  //  ������ ��������� ����� �� ���� A0
 {mstNone,0, 0},
 {mstNone,0, 0}  

/* 
 
 �������������� ���� ��������: 
 
  {mstSi7021,0,0} - ������ ����������� � ��������� Si7021 �� ���� I2C  
  {mstBH1750,BH1750Address1,0} - ������ ������������ BH1750 �� ���� I2C, ��� ������ ����� I2C
  {mstBH1750,BH1750Address2,0} - ������ ������������ BH1750 �� ���� I2C, ��� ������ ����� I2C
  {mstDS18B20,A0,0} - ������ DS18B20 �� ���� A0
  {mstChinaSoilMoistureMeter,A7,0} - ��������� ������ ��������� ����� �� ���� A7
  {mstDHT22, 6, 0} - ������ DHT2x �� ���� 6
  {mstDHT11, 5, 0} - ������ DHT11 �� ���� 5
  {mstPHMeter,A0, 0} // ������ pH �� ���� A0
  {mstSHT10,10,11} // ������ SHT10 �� ���� 10 (����� ������) � ���� 11 (�����)
  {mstMAX44009,MAX44009_ADDRESS1,0} - ������ ������������ MAX44009 �� ���� I2C, ��� ������ ����� I2C
  {mstMAX44009,MAX44009_ADDRESS2,0} - ������ ������������ MAX44009 �� ���� I2C, ��� ������ ����� I2C

  // ��������� ������� ��������� ����� ������ �� ������ �������� ���, �� ���������� �������� �������������� ��������� ����� !!! ������������ ����������� ���������� - 254, ����������� - 1.
  {mstFrequencySoilMoistureMeter,A5, 0} - ��������� ������ ��������� ����� �� ���������� ���� A5
  {mstFrequencySoilMoistureMeter,A4, 0} - ��������� ������ ��������� ����� �� ���������� ���� A4
  {mstFrequencySoilMoistureMeter,A3, 0} - ��������� ������ ��������� ����� �� ���������� ���� A3
  

  ���� � ����� ��������
    {mstNone,0, 0}
  �� ��� ������, ��� ������� �� ���� ����� ���   

 */

};
//----------------------------------------------------------------------------------------------------------------
// ������ ������ - ��������������� :)
//----------------------------------------------------------------------------------------------------------------
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// ||
// \/
//----------------------------------------------------------------------------------------------------------------
// ����� ���� ��������� ��������� � ��� - ������ � ������ ���������� ����, ��� ����� ������� !!!
//----------------------------------------------------------------------------------------------------------------
// ��������� 1-Wire
Pin oneWireData(2); // �� ������ ���� � ��� ����� 1-Wire
const byte owROM[7] = { 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }; // ����� �������, ������ �� �����������, �.�. � ��� �� ������� 1-Wire
// ������� 1-Wire
const byte COMMAND_START_CONVERSION = 0x44; // ��������� �����������
const byte COMMAND_READ_SCRATCHPAD = 0xBE; // ��������� ������ ��������� �������
const byte COMMAND_WRITE_SCRATCHPAD = 0x4E; // ��������� �������� ���������, ������ ����� ���������
const byte COMMAND_SAVE_SCRATCHPAD = 0x25; // ��������� ��������� ��������� � EEPROM
enum DeviceState {
  DS_WaitingReset,
  DS_WaitingCommand,
  DS_ReadingScratchpad,
  DS_SendingScratchpad
};
volatile DeviceState state = DS_WaitingReset;
volatile byte scratchpadWritePtr = 0; // ��������� �� ���� � ����������, ���� ���� �������� ��������� �� ������� ����
volatile byte scratchpadNumOfBytesReceived = 0; // ������� ���� ��������� �� �������
//-------------------------------------------------------------------------------------------------�---------------
#ifdef USE_POWER_SAVING
Pin linesPowerDown(LINES_POWER_DOWN_PIN);
#endif
//----------------------------------------------------------------------------------------------------------------
#define ROM_ADDRESS (void*) 123 // �� ������ ������ � ��� ���������?
//----------------------------------------------------------------------------------------------------------------
t_scratchpad scratchpadS, scratchpadToSend;
volatile char* scratchpad = (char *)&scratchpadS; //��� �� ���������� � scratchpad ��� � ��������� �������

volatile bool scratchpadReceivedFromMaster = false; // ����, ��� �� �������� ������ � �������
volatile bool needToMeasure = false; // ����, ��� �� ������ ��������� �����������
volatile unsigned long sensorsUpdateTimer = 0; // ������ ��������� ���������� � �������� � ���������� ������ � ����������
volatile bool measureTimerEnabled = false; // ����, ��� �� ������ ��������� ������ � �������� ����� ������ ���������
volatile unsigned long query_interval = MEASURE_MIN_TIME; // ��� ����� �������� ������
unsigned long last_measure_at = 0; // ����� � ��������� ��� ��������� �����������

volatile bool connectedViaOneWire = false; // ����, ��� �� ������������ � ����� 1-Wire, ��� ���� �� �� ����� � ���� �� nRF � �� ��������� ��������� �� RS-485
volatile bool needResetOneWireLastCommandTimer = false;
volatile unsigned long oneWireLastCommandTimer = 0;
//----------------------------------------------------------------------------------------------------------------
#ifdef USE_RS485_GATE // ������� �������� ��� � ����� RS-485
//----------------------------------------------------------------------------------------------------------------
/*
 ��������� ������, ������������� �� RS-495:
 
   0xAB - ������ ���� ���������
   0xBA - ������ ���� ���������

   ������, � ����������� �� ���� ������
   
   0xDE - ������ ���� ���������
   0xAD - ������ ���� ���������
   CRC - ����������� ����� ������

 
 */
//----------------------------------------------------------------------------------------------------------------
RS485Packet rs485Packet; // �����, � ������� �� ��������� ������
volatile byte* rsPacketPtr = (byte*) &rs485Packet;
volatile byte  rs485WritePtr = 0; // ��������� ������ � �����
//----------------------------------------------------------------------------------------------------------------
bool GotRS485Packet()
{
  // ���������, ���� �� � ��� �������� RS-485 �����
  return rs485WritePtr > ( sizeof(RS485Packet)-1 );
}
//----------------------------------------------------------------------------------------------------------------
void ProcessRS485Packet()
{

  // ������������ �������� �����. ��� ����� ���������� �������� � ��������������
  // ������ ������, ������� �� ������� ���� ��������� � ����������, ��� �� ��������. 
  // ���� �� ����� ��������� � �� �� � ������ ������ - ������, � �������������� ��������,
  // � �� ������ �������� ��������� � ������ ������, ����� ����� �������� �������.
  if(!(rs485Packet.header1 == 0xAB && rs485Packet.header2 == 0xBA))
  {
     // ��������� ������������, ���� ��������� ������ ������
     byte readPtr = 0;
     bool startPacketFound = false;
     while(readPtr < sizeof(RS485Packet))
     {
       if(rsPacketPtr[readPtr] == 0xAB)
       {
        startPacketFound = true;
        break;
       }
        readPtr++;
     } // while

     if(!startPacketFound) // �� ����� ������ ������
     {
        rs485WritePtr = 0; // ���������� ��������� ������ � �������
        return;
     }

     if(readPtr == 0)
     {
      // ��������� ���� ��������� ������, �� �� � ������� �������, ������������� - ���-�� ����� �� ���
        rs485WritePtr = 0; // ���������� ��������� ������ � �������
        return;       
     } // if

     // ������ ������ �������, �������� ��, ��� ����� ����, ��������� � ������ ������
     byte writePtr = 0;
     byte bytesWritten = 0;
     while(readPtr < sizeof(RS485Packet) )
     {
      rsPacketPtr[writePtr++] = rsPacketPtr[readPtr++];
      bytesWritten++;
     }

     rs485WritePtr = bytesWritten; // ����������, ���� ������ ��������� ����
     return;
         
  } // if
  else
  {
    // ��������� ����������, ��������� ���������
    if(!(rs485Packet.tail1 == 0xDE && rs485Packet.tail2 == 0xAD))
    {
      // ��������� ������������, ���������� ��������� ������ � �������
      rs485WritePtr = 0;
      return;
    }
    // ������ �� ��������, ����� �������� ��������� ������, ����� �� ������
    rs485WritePtr = 0;

    // ��������� ����������� �����
    byte crc = OneWireSlave::crc8((const byte*) rsPacketPtr,sizeof(RS485Packet) - 1);
    if(crc != rs485Packet.crc8)
    {
      // �� �������, ����������
      return;
    }


    // �� � ������ ���������, ����������� � ���������
    // ���������, ��� �� �����
    if(rs485Packet.direction != RS485FromMaster) // �� �� ������� �����
      return;

    if(rs485Packet.type != RS485SensorDataPacket) // ����� �� c �������� ��������� �������
      return;

     // ������ �������� ����� � ������� ����
     byte* readPtr = rs485Packet.data;
     // � ������ ����� � ��� ��� ��� ������� ��� ������
     byte sensorType =  *readPtr++;
     // �� ������ - ������ ������� � �������
     byte sensorIndex = *readPtr++;

     // ������ ��� ���� �����, ���� �� � ��� ���� ������
     sensor* sMatch = NULL;
     if(scratchpadS.sensor1.type == sensorType && scratchpadS.sensor1.index == sensorIndex)
        sMatch = &(scratchpadS.sensor1);

     if(!sMatch)
     {
     if(scratchpadS.sensor2.type == sensorType && scratchpadS.sensor2.index == sensorIndex)
        sMatch = &(scratchpadS.sensor2);        
     }

     if(!sMatch)
     {
     if(scratchpadS.sensor3.type == sensorType && scratchpadS.sensor3.index == sensorIndex)
        sMatch = &(scratchpadS.sensor3);        
     }

     if(!sMatch) {// �� ����� � ��� ������ �������

      return;
     }

     memcpy(readPtr,sMatch->data,4); // � ��� 4 ����� �� ���������, �������� �� ���

     // ���������� ������ ����������� ������
     rs485Packet.direction = RS485FromSlave;
     rs485Packet.type = RS485SensorDataPacket;

     // ������������ CRC
     rs485Packet.crc8 = OneWireSlave::crc8((const byte*) &rs485Packet,sizeof(RS485Packet)-1 );

     // ������ ������������� �� ��������
     RS485Send();

     // ����� � ���� ������
     Serial.write((const uint8_t *)&rs485Packet,sizeof(RS485Packet));

     // ��� ��������� ��������
     RS485waitTransmitComplete();
     
    // ������������� �� ����
    RS485Receive();


    
  } // else
}
//----------------------------------------------------------------------------------------------------------------
void ProcessIncomingRS485Packets() // ������������ �������� ������ �� RS-485
{
  while(Serial.available())
  {
    rsPacketPtr[rs485WritePtr++] = (byte) Serial.read();
   
    if(GotRS485Packet())
      ProcessRS485Packet();
  } // while
  
}
//----------------------------------------------------------------------------------------------------------------
void RS485Receive()
{
  digitalWrite(RS485_DE_PIN,LOW); // ��������� ���������� RS-485 �� ����
}
//----------------------------------------------------------------------------------------------------------------
void RS485Send()
{
  digitalWrite(RS485_DE_PIN,HIGH); // ��������� ���������� RS-485 �� ��������
}
//----------------------------------------------------------------------------------------------------------------
void InitRS485()
{
  memset(&rs485Packet,0,sizeof(RS485Packet));
  // ��� ����������� RS-485 �� ����
  pinMode(RS485_DE_PIN,OUTPUT);
  RS485Receive();
}
//----------------------------------------------------------------------------------------------------------------
void RS485waitTransmitComplete()
{
  // ��� ���������� �������� �� UART
  while(!(UCSR0A & _BV(TXC0) ));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // USE_RS485_GATE
//-------------------------------------------------------------------------------------------------------------------------------------------------------
byte GetSensorType(const SensorSettings& sett)
{
  switch(sett.Type)
  {
    case mstNone:
      return uniNone;
    
    case mstDS18B20:
      return uniTemp;
      
    case mstBH1750:
    case mstMAX44009:
      return uniLuminosity;

    case mstSi7021:
    case mstDHT11:
    case mstDHT22:
    case mstSHT10:
      return uniHumidity;

    case mstChinaSoilMoistureMeter:
    case mstFrequencySoilMoistureMeter:
      return uniSoilMoisture;

    case mstPHMeter:
      return uniPH;
    
  }

  return uniNone;
}
//----------------------------------------------------------------------------------------------------------------
void SetDefaultValue(const SensorSettings& sett, byte* data)
{
  switch(sett.Type)
  {
    case mstNone:
      *data = 0xFF;
    break;
    
    case mstDS18B20:
    case mstChinaSoilMoistureMeter:
    case mstPHMeter:
    case mstFrequencySoilMoistureMeter:
    {
      *data = NO_TEMPERATURE_DATA;
    }
    break;
      
    case mstBH1750:
    case mstMAX44009:
    {
    long lum = NO_LUMINOSITY_DATA;
    memcpy(data,&lum,sizeof(lum));
    }
    break;

    case mstSi7021:
    case mstDHT11:
    case mstDHT22:
    case mstSHT10:
    {
    *data = NO_TEMPERATURE_DATA;
    data++; data++;
    *data = NO_TEMPERATURE_DATA;
    }
    break;

  }
}
//----------------------------------------------------------------------------------------------------------------
void* SensorDefinedData[3] = {NULL}; // ������, ����������� ��������� ��� �������������
//----------------------------------------------------------------------------------------------------------------
void* InitSensor(const SensorSettings& sett)
{
  switch(sett.Type)
  {
    case mstNone:
      return NULL;
    
    case mstDS18B20:
      return InitDS18B20(sett);

    case mstFrequencySoilMoistureMeter:
        return InitFrequencySoilMoistureMeter(sett);
    break;
      
    case mstBH1750:
      return InitBH1750(sett);

   case mstMAX44009:
    return InitMax44009(sett);

    case mstSi7021:
      return InitSi7021(sett);

    case mstDHT11:
      return InitDHT(sett,DHT_11);

    case mstDHT22:
      return InitDHT(sett,DHT_2x);

    case mstChinaSoilMoistureMeter:
      return NULL;

    case mstSHT10:
      return NULL;

    case mstPHMeter: // �������������� ��������� ��� ������ pH
    {      
      PHMeasure* m = new PHMeasure;
      m->samplesDone = 0;
      m->samplesTimer = 0;
      m->data = 0;
      m->inMeasure = false; // ������ �� ��������
      return m;
    }
    break;
  }

  return NULL;  
}
//----------------------------------------------------------------------------------------------------------------
void ReadROM()
{
    memset((void*)&scratchpadS,0,sizeof(scratchpadS));
    eeprom_read_block((void*)&scratchpadS, ROM_ADDRESS, 29);

    // ����� ����� ������ �� ���������
    if(scratchpadS.rf_id == 0xFF || scratchpadS.rf_id == 0)
      scratchpadS.rf_id = DEFAULT_RF_CHANNEL; 

    scratchpadS.packet_type = ptSensorsData; // �������, ��� ��� ��� ������ - ������ � ���������
    scratchpadS.packet_subtype = 0;


    // ���� ��������� ������ �� ��������� - ���������� �� ���������
    if(scratchpadS.query_interval_min == 0xFF)
      scratchpadS.query_interval_min = 0;
      
    if(scratchpadS.query_interval_sec == 0xFF)
      scratchpadS.query_interval_sec =  MEASURE_MIN_TIME/1000;

   if(scratchpadS.query_interval_min == 0 && scratchpadS.query_interval_sec < 5) // ������� 5 ������ ����� ������������ ��������
    scratchpadS.query_interval_sec = 5;

    // ��������� �������� ������
    query_interval = (scratchpadS.query_interval_min*60 + scratchpadS.query_interval_sec)*1000;

    #ifdef _DEBUG
      Serial.print(F("Query interval: "));
      Serial.println(query_interval);
    #endif
    
    scratchpadS.sensor1.type = GetSensorType(Sensors[0]);
    scratchpadS.sensor2.type = GetSensorType(Sensors[1]);
    scratchpadS.sensor3.type = GetSensorType(Sensors[2]);

    SetDefaultValue(Sensors[0],scratchpadS.sensor1.data);
    SetDefaultValue(Sensors[1],scratchpadS.sensor2.data);
    SetDefaultValue(Sensors[2],scratchpadS.sensor3.data);

    // �������, ���� �� � ��� ����������?
    byte calibration_enabled = false;
    for(byte i=0;i<3;i++)
    {
        switch(Sensors[i].Type)
        {
            case mstChinaSoilMoistureMeter:
            {
              calibration_enabled = true;
              // ������������� �������� �� ���������
              if(scratchpadS.calibration_factor1 == 0xFF && scratchpadS.calibration_factor2 == 0xFF)
              {
                // � EEPROM ������ ��� �� ����� ������, ��� �������� ���������
                scratchpadS.calibration_factor1 = map(SOIL_MOISTURE_0_PERCENT,0,1023,0,255);
                scratchpadS.calibration_factor2 = map(SOIL_MOISTURE_100_PERCENT,0,1023,0,255);
              }

              // �� ������������ ��� ������� ����������
              scratchpadS.config |= (4 | 8);
            }
            break; // mstChinaSoilMoistureMeter

            case mstPHMeter:
            {
              calibration_enabled = true;
              // ������������� �������� �� ���������
              if(scratchpadS.calibration_factor1 == 0xFF)
              {
                // ��������� � ��� ����������� ���� - �������� 127 ������������� ���������� 0,
                // ��, ��� ������ - ������������� ����������, ��� ������ - ������������� ����������
                scratchpadS.calibration_factor1 = 127; 
              }

              scratchpadS.config |= 4; // ������������ ����� ���� ������ ����������
            }
            break; // mstPHMeter
          
        } // switch

        if(calibration_enabled)
          break;
    
    } // for

    if(calibration_enabled)
    {
      // ������� ������ ����������
      scratchpadS.config |= 2; // ������������� ������ ���, ������, ��� �� ������������ ����������
    } // if
    else
    {
      scratchpadS.config &= ~2; // ������ ��� ������� ��-������
    }

}
//----------------------------------------------------------------------------------------------------------------
void WakeUpSensor(const SensorSettings& sett, void* sensorDefinedData)
{  
  // ��������� �������
  switch(sett.Type)
  {
    case mstNone:
      break;
    
    case mstDS18B20:
      InitDS18B20(sett);
    break;

    case mstBH1750:
    {
      BH1750Support* bh = (BH1750Support*) sensorDefinedData;
      bh->begin((BH1750Address)sett.Pin);
    }
    break;

    case mstMAX44009:
    {
      Max44009* bh = (Max44009*) sensorDefinedData;
      bh->begin((MAX44009_ADDRESS) sett.Pin);
    }
    break;

    case mstSi7021:
    {
      Si7021* si = (Si7021*) sensorDefinedData;
      si->begin();
    }
    break;

    case mstChinaSoilMoistureMeter:
    case mstDHT11:
    case mstDHT22:
    case mstSHT10:
    break;

    case mstFrequencySoilMoistureMeter:
    {
      Pin pin(sett.Pin);
      pin.inputMode();
      pin.writeHigh();      
    }
    break;
    
    case mstPHMeter:
    {
      // ���� ��������� ��� � �������
      Pin pin(sett.Pin);
      pin.inputMode();
      pin.writeHigh();
      analogRead(sett.Pin);
    }
    break;
  }    
}
//----------------------------------------------------------------------------------------------------------------
void WakeUpSensors() // ����� ��� �������
{
#ifdef USE_POWER_SAVING  
  #ifdef _DEBUG
    Serial.println(F("Wake up sensors..."));
  #endif
  
  // �������� ��� �����
  linesPowerDown.write(LINES_POWER_UP_LEVEL);
  // ���, ���� ������� �����������
  delay(LINES_POWER_UP_DELAY);
  
 if(HasI2CSensors())
  PowerUpI2C(); // ��������� I2C
 
   // ����� �������
   for(byte i=0;i<3;i++)
    WakeUpSensor(Sensors[i],SensorDefinedData[i]);

#endif
   
}
//----------------------------------------------------------------------------------------------------------------
void PowerDownSensors()
{
#ifdef USE_POWER_SAVING
  
   #ifdef _DEBUG
    Serial.println(F("Power down sensors..."));
  #endif 
  // ��������� ��� �����
  linesPowerDown.write(LINES_POWER_DOWN_LEVEL);
  
  PowerDownI2C(); // ������ ���� I2C
  
#endif  
      
}
//----------------------------------------------------------------------------------------------------------------
void* InitDHT(const SensorSettings& sett, DHTType dhtType) // �������������� ������ ��������� DHT*
{
  UNUSED(sett);
  
  DHTSupport* dht = new DHTSupport(dhtType);
  
  return dht;
}
//----------------------------------------------------------------------------------------------------------------
void* InitSi7021(const SensorSettings& sett) // �������������� ������ ��������� Si7021
{
  UNUSED(sett);
  
  Si7021* si = new Si7021();
  si->begin();

  return si;
}
//----------------------------------------------------------------------------------------------------------------
void* InitFrequencySoilMoistureMeter(const SensorSettings& sett)
{
    UNUSED(sett);
    return NULL;  
}
//----------------------------------------------------------------------------------------------------------------
void* InitMax44009(const SensorSettings& sett) // �������������� ������ ������������ MAX44009
{
  Max44009* bh = new Max44009();
  
  bh->begin((MAX44009_ADDRESS)sett.Pin);
  
  return bh;
}
//----------------------------------------------------------------------------------------------------------------
void* InitBH1750(const SensorSettings& sett) // �������������� ������ ������������ BH1750
{
  BH1750Support* bh = new BH1750Support();
  
  bh->begin((BH1750Address)sett.Pin);
  
  return bh;
}
//----------------------------------------------------------------------------------------------------------------
void* InitDS18B20(const SensorSettings& sett) // �������������� ������ �����������
{
  #ifdef _DEBUG
    Serial.println(F("Init DS18B20..."));
  #endif
  
  if(!sett.Pin) {
    #ifdef _DEBUG
      Serial.println(F("DS18B20 - no pin number!!!"));
    #endif
    return NULL; 
  }  

   OneWire ow(sett.Pin);

  if(!ow.reset()) // ��� �������
  {
     #ifdef _DEBUG
      Serial.println(F("DS18B20 - not found during init!!!"));
    #endif
    return NULL;  
  }

   ow.write(0xCC); // ����� �� ������ (SKIP ROM)
   ow.write(0x4E); // ��������� ������ � scratchpad

   ow.write(0); // ������� ������������� ����� 
   ow.write(0); // ������ ������������� �����
   ow.write(0x7F); // ���������� ������� 12 ���

   ow.reset();
   ow.write(0xCC); // ����� �� ������ (SKIP ROM)
   ow.write(0x48); // COPY SCRATCHPAD
   delay(10);
   ow.reset();

  #ifdef _DEBUG
    Serial.println(F("DS18B20 - inited."));
  #endif

   return NULL;
    
}
//----------------------------------------------------------------------------------------------------------------
void InitSensors()
{
  #ifdef _DEBUG
    Serial.println(F("Init sensors..."));
  #endif
  
  // �������������� �������
  for(byte i=0;i<3;i++)
    SensorDefinedData[i] = InitSensor(Sensors[i]);
         
}
//----------------------------------------------------------------------------------------------------------------
 void ReadDS18B20(const SensorSettings& sett, struct sensor* s) // ������ ������ � ������� �����������
{ 
  
  #ifdef _DEBUG
    Serial.println(F("Read DS18B20..."));
  #endif
  
  s->data[0] = NO_TEMPERATURE_DATA;
  s->data[1] = 0;
  
  if(!sett.Pin)
  {
    #ifdef _DEBUG
      Serial.println(F("DS18B20 - no pin number!!!"));
    #endif       
    return;
  }

   OneWire ow(sett.Pin);
    
    if(!ow.reset()) // ��� ������� �� �����
    {
    #ifdef _DEBUG
      Serial.println(F("DS18B20 - not found!"));
    #endif   
    return; 
    }

  byte data[9] = {0};
  
  ow.write(0xCC); // ����� �� ������ (SKIP ROM)
  ow.write(0xBE); // ������ scratchpad ������� �� ����

  for(uint8_t i=0;i<9;i++)
    data[i] = ow.read();


 if (OneWire::crc8(data, 8) != data[8]) // ��������� ����������� �����
 {
  #ifdef _DEBUG
    Serial.println(F("DS18B20 - bad checksum!!!"));
  #endif     
      return;
 }
  int loByte = data[0];
  int hiByte = data[1];

  int temp = (hiByte << 8) + loByte;
  
  bool isNegative = (temp & 0x8000);
  
  if(isNegative)
    temp = (temp ^ 0xFFFF) + 1;

  int tc_100 = (6 * temp) + temp/4;

  if(isNegative)
    tc_100 = -tc_100;
   
  s->data[0] = tc_100/100;
  s->data[1] = abs(tc_100 % 100);

  #ifdef _DEBUG
    Serial.print(F("DS18B20: "));
    Serial.print(s->data[0]);
    Serial.print(F(","));
    Serial.println(s->data[1]);
    
  #endif     
    
}
//----------------------------------------------------------------------------------------------------------------
void ReadFrequencySoilMoistureMeter(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s)
{
    UNUSED(sensorDefinedData);

 int highTime = pulseIn(sett.Pin,HIGH);
 
 if(!highTime) // always HIGH ?
 {
   s->data[0] = NO_TEMPERATURE_DATA;

  // Serial.println("ALWAYS HIGH,  BUS ERROR!");

   return;
 }
 highTime = pulseIn(sett.Pin,HIGH);
 int lowTime = pulseIn(sett.Pin,LOW);


 if(!lowTime)
 {
  return;
 }
  int totalTime = lowTime + highTime;
  // ������ ������� ��������� highTime � ����� ����� ��������� - ��� � ����� ��������� �����
  // totalTime = 100%
  // highTime = x%
  // x = (highTime*100)/totalTime;

  float moisture = (highTime*100.0)/totalTime;

  int moistureInt = moisture*100;

   s->data[0] = moistureInt/100;
   s->data[1] = moistureInt%100;
 
}
//----------------------------------------------------------------------------------------------------------------
void ReadMax44009(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s) // ������ ������ � ������� ������������ MAX44009
{
  UNUSED(sett);
  Max44009* bh = (Max44009*) sensorDefinedData;

  long lum;
  float curLum =  bh->readLuminosity();

   if(curLum < 0)
      lum = NO_LUMINOSITY_DATA;
    else
    {
      unsigned long ulLum = (unsigned long) curLum;
      if(ulLum > 65535)
        ulLum = 65535;

      lum = ulLum;
    }
  
  memcpy(s->data,&lum,sizeof(lum));

}
//----------------------------------------------------------------------------------------------------------------
void ReadBH1750(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s) // ������ ������ � ������� ������������ BH1750
{
  UNUSED(sett);
  BH1750Support* bh = (BH1750Support*) sensorDefinedData;
  long lum = bh->GetCurrentLuminosity();
  memcpy(s->data,&lum,sizeof(lum));

}
//----------------------------------------------------------------------------------------------------------------
void ReadDHT(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s) // ������ ������ � ������� ��������� Si7021
{
  DHTSupport* dht = (DHTSupport*) sensorDefinedData;
    
  HumidityAnswer ha;
  dht->read(sett.Pin,ha);

  memcpy(s->data,&ha,sizeof(ha));

}
//----------------------------------------------------------------------------------------------------------------
void ReadSi7021(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s) // ������ ������ � ������� ��������� Si7021
{
  UNUSED(sett);
  Si7021* si = (Si7021*) sensorDefinedData;
  HumidityAnswer ha;
  si->read(ha);

  memcpy(s->data,&ha,sizeof(ha));



}
//----------------------------------------------------------------------------------------------------------------
void ReadSHT10(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s) // ������ ������ � ������� ��������� SHT10
{
  UNUSED(sett);
  UNUSED(sensorDefinedData);

  SHT1x sht(sett.Pin,sett.Pin2);
  float temp = sht.readTemperatureC();
  float hum = sht.readHumidity();

  HumidityAnswer ha;
  ha.Temperature = NO_TEMPERATURE_DATA;
  ha.Humidity =   NO_TEMPERATURE_DATA;

  if(((int)temp) != -40)
  {
    // has temperature
    int conv = temp * 100;
    ha.Temperature = conv/100;
    ha.TemperatureDecimal = abs(conv%100);
  }

  if(!(hum < 0))
  {
    // has humidity
    int conv = hum*100;
    ha.Humidity = conv/100;
    ha.HumidityDecimal = conv%100;
  }

  memcpy(s->data,&ha,sizeof(ha));

}
//----------------------------------------------------------------------------------------------------------------
void ReadChinaSoilMoistureMeter(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s)
{
   UNUSED(sensorDefinedData);
   
   int val = analogRead(sett.Pin);

#ifdef _DEBUG
   Serial.print(F("Val: "));
   Serial.println(val); 
 /*  Serial.print(s->data[0]);
   Serial.print(F(","));
   Serial.println(s->data[1]);*/

#endif  
   
   int soilMoisture0Percent = map(scratchpadS.calibration_factor1,0,255,0,1023);
   int soilMoisture100Percent = map(scratchpadS.calibration_factor2,0,255,0,1023);

   int percentsInterval = map(val,min(soilMoisture0Percent,soilMoisture100Percent),max(soilMoisture0Percent,soilMoisture100Percent),0,10000);
   
  // ������, ���� � ��� �������� 0% ��������� ������, ��� �������� 100% ��������� - ���� �� 10000 ������ ���������� ��������
  if(soilMoisture0Percent > soilMoisture100Percent)
    percentsInterval = 10000 - percentsInterval;

   int8_t sensorValue;
   byte sensorFract;

   sensorValue = percentsInterval/100;
   sensorFract = percentsInterval%100;

   if(sensorValue > 99)
   {
      sensorValue = 100;
      sensorFract = 0;
   }

   if(sensorValue < 0)
   {
      sensorValue = NO_TEMPERATURE_DATA;
      sensorFract = 0;
   }

   s->data[0] = sensorValue;
   s->data[1] = sensorFract;

#ifdef _DEBUG
   Serial.print(F("data: "));
   Serial.println(s->data[0]);

#endif  

   
}
//----------------------------------------------------------------------------------------------------------------
void ReadPHValue(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s)
{
  
  UNUSED(sett);
  
  // ������������ ���������� �������� pH
 PHMeasure* pm = (PHMeasure*) sensorDefinedData;

 pm->inMeasure = false; // �������, ��� ��� ������ �� ��������

 s->data[0] = NO_TEMPERATURE_DATA;
 
 if(pm->samplesDone > 0)
 {
  // ������� �������� �������� ����������, �������������� ��� � �������� �����
  int8_t calibration = map(scratchpadS.calibration_factor1,0,255,-128,127);
  
  // ����������� ���������� �������� � �������
  float avgSample = (pm->data*1.0)/pm->samplesDone;
  
  // ������ ������� �������
  float voltage = avgSample*5.0/1024;
        
  // ������ �������� �������� pH
  //unsigned long phValue = voltage*350 + calibration;
  float coeff = 700000/PH_MV_PER_7_PH;
  unsigned long phValue = voltage*coeff + calibration;
  
  #ifdef PH_REVERSIVE_MEASURE
    // ������� �������� pH � �������� ����������� ���������
    int16_t rev = phValue - 700; // ��������� � ��� 7 pH - ��� ������� �����, �� � ������� ����������� ��������� ��
    // ������� ����� pH (7.0) ���� ������ ������� ����� ��������� 7 pH � ���������� ���������, ��� �� � ������
    phValue = 700 - rev;
   #endif
             
    if(avgSample > 1000)
    {
      // �� ��������� �� ����� ������, ������ ��� � ��� �������� �������� � �������
    }
    else
    {
      s->data[0] = phValue/100;
      s->data[1] = phValue%100;
    } // else
  
 } // pm->samplesDone > 0

 // ���������� ������ � 0
 pm->samplesDone = 0;
 pm->samplesTimer = 0;
 pm->data = 0;  
  
}
//----------------------------------------------------------------------------------------------------------------
void ReadSensor(const SensorSettings& sett, void* sensorDefinedData, struct sensor* s)
{
  switch(sett.Type)
  {
    case mstNone:
      
    break;

    case mstDS18B20:
    ReadDS18B20(sett,s);
    break;

    case mstBH1750:
    ReadBH1750(sett,sensorDefinedData,s);
    break;

    case mstMAX44009:
    ReadMax44009(sett,sensorDefinedData,s);
    break;

    case mstSi7021:
    ReadSi7021(sett,sensorDefinedData,s);
    break;

    case mstSHT10:
    ReadSHT10(sett,sensorDefinedData,s);
    break;

    case mstChinaSoilMoistureMeter:
      ReadChinaSoilMoistureMeter(sett,sensorDefinedData,s);
    break;

    case mstPHMeter:
      ReadPHValue(sett,sensorDefinedData,s);
    break;

    case mstDHT11:
    case mstDHT22:
      ReadDHT(sett,sensorDefinedData,s);
    break;

    case mstFrequencySoilMoistureMeter:
      ReadFrequencySoilMoistureMeter(sett,sensorDefinedData,s);
    break;
  }
}
//----------------------------------------------------------------------------------------------------------------
void ReadSensors()
{
  #ifdef _DEBUG
    Serial.println(F("Read sensors..."));
  #endif  
  // ������ ���������� � ��������
    
  ReadSensor(Sensors[0],SensorDefinedData[0],&scratchpadS.sensor1);
  ReadSensor(Sensors[1],SensorDefinedData[1],&scratchpadS.sensor2);
  ReadSensor(Sensors[2],SensorDefinedData[2],&scratchpadS.sensor3);

}
//----------------------------------------------------------------------------------------------------------------
void MeasureDS18B20(const SensorSettings& sett)
{
    
  #ifdef _DEBUG
    Serial.println(F("DS18B20 - start conversion..."));
  #endif
  
  if(!sett.Pin)
  {
    #ifdef _DEBUG
      Serial.println(F("DS18B20 - no pin number!!!"));
    #endif
    return;
  }

   OneWire ow(sett.Pin);
    
    if(!ow.reset()) // ��� ������� �� �����
    {
      #ifdef _DEBUG
        Serial.println(F("DS18B20 - not found!!!"));
      #endif    
      return; 
    }

    ow.write(0xCC);
    ow.write(0x44); // �������� ������� �� ����� ���������
    
    ow.reset();    

  #ifdef _DEBUG
    Serial.println(F("DS18B20 - converted."));
  #endif    
  
}
//----------------------------------------------------------------------------------------------------------------
bool HasI2CSensors()
{
  // ���������, ���� �� � ��� ���� ���� ������ �� I2C
  for(byte i=0;i<3;i++)
  {
    switch(Sensors[i].Type)
    {
      case mstBH1750:
      case mstSi7021:
      case mstMAX44009:
        return true;
    }
    
  } // for
  return false;
}
//----------------------------------------------------------------------------------------------------------------
#ifdef USE_POWER_SAVING
inline void PowerUpI2C()
{
  power_twi_enable();
  delay(20);
}
//----------------------------------------------------------------------------------------------------------------
inline void PowerDownI2C()
{
  power_twi_disable();
}
#endif
//----------------------------------------------------------------------------------------------------------------
void MeasurePH(const SensorSettings& sett,void* sensorDefinedData)
{
 // �������� ��������� pH ����� 
 PHMeasure* pm = (PHMeasure*) sensorDefinedData;
 
 if(pm->inMeasure) // ��� ��������
  return;
  
 pm->samplesDone = 0;
 pm->samplesTimer = millis();
 pm->data = 0;
 // ������ �� ���� � ���������� ��� ��������
 analogRead(sett.Pin);
 pm->inMeasure = true; // �������, ��� ������ ��������
}
//----------------------------------------------------------------------------------------------------------------
void MeasureSensor(const SensorSettings& sett,void* sensorDefinedData) // ��������� ����������� � �������, ���� ����
{
  switch(sett.Type)
  {
    case mstNone:    
    break;

    case mstDS18B20:
    MeasureDS18B20(sett);
    break;

    case mstPHMeter:
      MeasurePH(sett,sensorDefinedData);
    break;

    case mstBH1750:
    case mstMAX44009:
    case mstSi7021:
    case mstChinaSoilMoistureMeter:
    case mstDHT11:
    case mstDHT22:
    case mstFrequencySoilMoistureMeter:
    case mstSHT10:
    break;
  }  
}
//----------------------------------------------------------------------------------------------------------------
void UpdatePH(const SensorSettings& sett,void* sensorDefinedData, unsigned long curMillis)
{
  PHMeasure* pm = (PHMeasure*) sensorDefinedData;
  
  if(!pm->inMeasure) // ������ �� ������
    return;
    
  if(pm->samplesDone >= PH_NUM_SAMPLES) // ��������� ���������
  {    
    pm->inMeasure = false;
    return;
  }
    
  if((curMillis - pm->samplesTimer) > PH_SAMPLES_INTERVAL)
  {
    
    pm->samplesTimer = curMillis; // ����������, ����� ��������
    // ���� ��������� �� �����
    pm->samplesDone++;
    pm->data += analogRead(sett.Pin);
  }
}
//----------------------------------------------------------------------------------------------------------------
void UpdateSensor(const SensorSettings& sett,void* sensorDefinedData, unsigned long curMillis)
{
  // ��������� ������� �����
  switch(sett.Type)
  {

    case mstPHMeter:
      UpdatePH(sett,sensorDefinedData,curMillis);
    break;

    case mstNone:    
    case mstDS18B20:
    case mstBH1750:
    case mstMAX44009:
    case mstSi7021:
    case mstChinaSoilMoistureMeter:
    case mstDHT11:
    case mstDHT22:
    case mstFrequencySoilMoistureMeter:
    case mstSHT10:
    break;
  }  
}
//----------------------------------------------------------------------------------------------------------------
void UpdateSensors()
{
  unsigned long thisMillis = millis();
  for(byte i=0;i<3;i++)
    UpdateSensor(Sensors[i],SensorDefinedData[i],thisMillis);  
}
//----------------------------------------------------------------------------------------------------------------
void StartMeasure()
{  
  #ifdef _DEBUG
    Serial.println(F("Start measure..."));
  #endif
    
 WakeUpSensors(); // ����� ��� �������
  
  // ��������� �����������
  for(byte i=0;i<3;i++)
    MeasureSensor(Sensors[i],SensorDefinedData[i]);

  last_measure_at = millis();
}
//----------------------------------------------------------------------------------------------------------------
#ifdef USE_NRF
//----------------------------------------------------------------------------------------------------------------
//uint64_t controllerStatePipe = 0xF0F0F0F0E0LL; // �����, � ������� �� ������� ��������� �����������
// �����, � ������� �� ����� ������
const uint64_t writingPipes[5] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL, 0xF0F0F0F0E4LL, 0xF0F0F0F0E5LL };
//----------------------------------------------------------------------------------------------------------------
#include "RF24.h"
RF24 radio(NRF_CE_PIN,NRF_CSN_PIN);
bool nRFInited = false;
//----------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
int serial_putc( char c, FILE * ) {
  Serial.write( c );
  return c;
}

void printf_begin(void) {
  fdevopen( &serial_putc, 0 );
  Serial.begin(57600);
  Serial.println(F("Init nRF..."));
}
#endif
//----------------------------------------------------------------------------------------------------------------
void initNRF()
{
  #ifdef _DEBUG
  Serial.begin(57600);
  printf_begin();
  #endif
  
  // �������������� nRF
  nRFInited = radio.begin();

  if(nRFInited) {
  delay(200); // ����-���� �������

  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(scratchpadS.rf_id);
  radio.setRetries(15,15);
  radio.setPayloadSize(sizeof(t_scratchpad)); // � ��� 30 ���� �� �����
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(
    #ifdef NRF_AUTOACK_INVERTED
      false
    #else
    true
    #endif
    );

  #ifdef _DEBUG
    radio.printDetails();
  #endif

  radio.powerDown(); // ������ � ����� ����������������
  
  } // nRFInited
  
}
//----------------------------------------------------------------------------------------------------------------
void sendDataViaNRF()
{
  if(!nRFInited) {
 #ifdef _DEBUG
  Serial.println(F("nRF not inited!"));
 #endif    
    return;
  }
    
  if(!((scratchpadS.config & 1) == 1))
  {
    #ifdef _DEBUG
    Serial.println(F("Transiever disabled."));
    #endif
    return;
  }
  
  radio.powerUp(); // �����������
  
  #ifdef _DEBUG
    Serial.println(F("Send sensors data via nRF..."));
  #endif

    bool sendDone = false;

    for(int i=0;i<5;i++) // �������� ������� 5 ���, � ������ �����
    {
      // �������� ������ ����� nRF
        uint8_t writePipeNum = random(0,5);
        radio.openWritingPipe(writingPipes[writePipeNum]); // ��������� ����� ��� ������
    
        // ������������ ����������� �����
        scratchpadS.crc8 = OneWireSlave::crc8((const byte*)&scratchpadS,sizeof(scratchpadS)-1);
            
        if(radio.write(&scratchpadS,sizeof(scratchpadS))) // ����� � ����
        {
          sendDone = true;
          break;
        }
    } // for

    if(!sendDone)
    {
      #ifdef _DEBUG
        Serial.println(F("NO RECEIVING SIDE FOUND!"));
      #endif      
    }
    else
    {
      #ifdef _DEBUG
        Serial.println(F("Sensors data sent."));
      #endif
    }
 radio.powerDown(); // ������ � ����� ����������������

}
//----------------------------------------------------------------------------------------------------------------
#endif // USE_NRF
//----------------------------------------------------------------------------------------------------------------
#ifdef USE_LORA
//----------------------------------------------------------------------------------------------------------------
#include "LoRa.h"
bool loRaInited = false;
//----------------------------------------------------------------------------------------------------------------
void initLoRa()
{
  #ifdef _DEBUG
  Serial.begin(57600);
  #endif
  
  // �������������� LoRa
  LoRa.setPins(LORA_SS_PIN,LORA_RESET_PIN,-1);
  loRaInited = LoRa.begin(LORA_FREQUENCY);

  if(loRaInited)
  {
    LoRa.setTxPower(LORA_TX_POWER);
    //LoRa.receive(); // �������� �������
    LoRa.sleep(); // ��������
  } // nRFInited
  
}
//----------------------------------------------------------------------------------------------------------------
void sendDataViaLoRa()
{
  if(!loRaInited) {
 #ifdef _DEBUG
  Serial.println(F("LoRa not inited!"));
 #endif    
    return;
  }
    
  if(!((scratchpadS.config & 1) == 1))
  {
    #ifdef _DEBUG
    Serial.println(F("LoRa: transiever disabled."));
    #endif
    return;
  }
    
  #ifdef _DEBUG
    Serial.println(F("Send sensors data via LoRa..."));
  #endif

  bool sendDone = false;

    for(int i=0;i<5;i++) // �������� ������� 5 ���
    {
        // ������������ ����������� �����
        scratchpadS.crc8 = OneWireSlave::crc8((const byte*)&scratchpadS,sizeof(scratchpadS)-1);  
        LoRa.beginPacket();
        LoRa.write((byte*)&scratchpadS,sizeof(scratchpadS)); // ����� � ����
        if(LoRa.endPacket()) // ����� � ����
        {
          sendDone = true;
          break;
        }
        else
        {
          delay(random(10));
        }
    } // for

    if(!sendDone)
    {
      #ifdef _DEBUG
        Serial.println(F("NO RECEIVING SIDE FOUND!"));
      #endif      
    }
    else
    {
      #ifdef _DEBUG
        Serial.println(F("Sensors data sent."));
      #endif
    }
    
  //LoRa.receive();

  // ��������� ��������
  delay(random(50));

  LoRa.sleep(); // ��������

}
//----------------------------------------------------------------------------------------------------------------
#endif // USE_LORA
//----------------------------------------------------------------------------------------------------------------
void WriteROM()
{

    scratchpadS.sensor1.type = GetSensorType(Sensors[0]);
    scratchpadS.sensor2.type = GetSensorType(Sensors[1]);
    scratchpadS.sensor3.type = GetSensorType(Sensors[2]);
  
    eeprom_write_block( (void*)scratchpad,ROM_ADDRESS,29);
    memcpy(&scratchpadToSend,&scratchpadS,sizeof(scratchpadS));
    scratchpadToSend.crc8 = OneWireSlave::crc8((const byte*)&scratchpadToSend,sizeof(scratchpadS)-1);

    #ifdef USE_NRF
      // ������������� ����� �����
      if(nRFInited)
        radio.setChannel(scratchpadS.rf_id);
        
    #endif
    

}
//----------------------------------------------------------------------------------------------------------------
void owReceive(OneWireSlave::ReceiveEvent evt, byte data);
//----------------------------------------------------------------------------------------------------------------
void setup()
{
  #ifdef _DEBUG
    Serial.begin(57600);
  #endif
  
 #ifdef USE_RS485_GATE // ���� ������� �������� ����� RS-485 - �������� 
 
    #ifndef _DEBUG
      Serial.begin(RS485_SPEED);
    #endif
    
    InitRS485(); // ����������� RS-485 �� ����
 #endif

  #ifdef USE_POWER_SAVING
    // ����������� ��� ���������� ������� �� �����
    linesPowerDown.outputMode();
    
    // �������� ��� ����� �� ������ ���������
   linesPowerDown.write(LINES_POWER_UP_LEVEL);
  #endif
    
  ReadROM();
  
  scratchpadS.crc8 = OneWireSlave::crc8((const byte*) scratchpad,sizeof(scratchpadS)-1);
  memcpy(&scratchpadToSend,&scratchpadS,sizeof(scratchpadS));

   InitSensors(); // �������������� �������   
   PowerDownSensors(); // � ��������� �� ����� ��� ������
    
    #ifdef USE_NRF
      initNRF();
    #endif

    #ifdef USE_LORA
      initLoRa();
    #endif

  oneWireLastCommandTimer = millis();
  
  OWSlave.setReceiveCallback(&owReceive);
  OWSlave.begin(owROM, oneWireData.getPinNumber());


   InitTimersSafe();                                  //�������������� ��� �������, ����� 0,
  //Timer2_Initialize();
  SetPinFrequencySafe(PWM_PIN, frequency);           //������������� ������� ��� ���������� pin
  SetPinFrequency(PWM_PIN, frequency);               //������������� ������� ��� ���������� pin
  pwmWrite(PWM_PIN, brightness);                     //0-255   ����������� ��� ������� ������ analogWrite 

  
}
//----------------------------------------------------------------------------------------------------------------
void owSendDone(bool error) {
  UNUSED(error);
 // ��������� �������� ��������� �������
 state = DS_WaitingReset;
}
//----------------------------------------------------------------------------------------------------------------
// ���������� ���������� �� ����
void owReceive(OneWireSlave::ReceiveEvent evt, byte data)
{
  connectedViaOneWire = true; // �������, ��� �� ���������� ����� 1-Wire
  needResetOneWireLastCommandTimer = true; // ������, ����� �������� ������ ������� ��������� ��������� �������
  
  switch (evt)
  {
  case OneWireSlave::RE_Byte:
    switch (state)
    {

     case DS_ReadingScratchpad: // ������ ��������� �� �������

        // ����������� ���-�� ����������� ����
        scratchpadNumOfBytesReceived++;

        // ����� � ��������� �������� ����
        scratchpad[scratchpadWritePtr] = data;
        // ����������� ��������� ������
        scratchpadWritePtr++;

        // ���������, �� �� ���������
        if(scratchpadNumOfBytesReceived >= sizeof(scratchpadS)) {
          // �� ���������, ���������� ��������� �� �������� ������
          state = DS_WaitingReset;
          scratchpadNumOfBytesReceived = 0;
          scratchpadWritePtr = 0;
          scratchpadReceivedFromMaster = true; // �������, ��� �� �������� ��������� �� �������
          // ��������� ����� �������� ������
          //query_interval = (scratchpadS.query_interval_min*60 + scratchpadS.query_interval_sec)*1000;
        }
        
     break; // DS_ReadingScratchpad
      
    case DS_WaitingCommand:
      switch (data)
      {
      case COMMAND_START_CONVERSION: // ��������� �����������
        state = DS_WaitingReset;
        if(!measureTimerEnabled && !needToMeasure) // ������ ���� ��� ��� �� ��������
          needToMeasure = true;
        break;

      case COMMAND_READ_SCRATCHPAD: // ��������� ������ ��������� �������
        state = DS_SendingScratchpad;
        OWSlave.beginWrite((const byte*)&scratchpadToSend, sizeof(scratchpadToSend), owSendDone);
        break;

      case COMMAND_WRITE_SCRATCHPAD:  // ��������� �������� ���������, ������ ����� ���������
          state = DS_ReadingScratchpad; // ��� ����������
          scratchpadWritePtr = 0;
          scratchpadNumOfBytesReceived = 0;
        break;

        case COMMAND_SAVE_SCRATCHPAD: // ��������� ��������� � ������
          state = DS_WaitingReset;
          WriteROM();
        break;

        default:
          state = DS_WaitingReset;
        break;
        

      } // switch (data)
      break; // case DS_WaitingCommand

      case DS_WaitingReset:
      break;

      case DS_SendingScratchpad:
      break;
    } // switch(state)
    break; 

  case OneWireSlave::RE_Reset:
    state = DS_WaitingCommand;
    break;

  case OneWireSlave::RE_Error:
    state = DS_WaitingReset;
    break;
    
  } // switch (evt)
}
//----------------------------------------------------------------------------------------------------------------
void loop()
{

  if(scratchpadReceivedFromMaster) 
  {
    scratchpadReceivedFromMaster = false;

    
    // ������ ��� ������� �� �������, ��� ����� ���-�� ������
    memcpy(&scratchpadToSend,&scratchpadS,sizeof(scratchpadS));
    scratchpadToSend.crc8 = OneWireSlave::crc8((const byte*) &scratchpadToSend,sizeof(scratchpadS)-1);

    // ��������� ����� �������� ������
    query_interval = (scratchpadToSend.query_interval_min*60 + scratchpadToSend.query_interval_sec)*1000;
          
     #ifdef _DEBUG
        Serial.println(F("Scratch received from master!"));
     #endif
      
  } // scratchpadReceivedFromMaster

  
  unsigned long curMillis = millis();

  // ���� ��������� �������� ������ ��������� ��������� ������� �� ����� 1-Wire - ������ ���
  if(needResetOneWireLastCommandTimer) 
  {
    oneWireLastCommandTimer = curMillis;
    needResetOneWireLastCommandTimer = false;
  }

  // ��������� - ����� ��������� ��������� ������� �� 1-Wire: ���� � �� ���� ������ 15 ������ - ���������� nRF � RS-485
  if(connectedViaOneWire) 
  {
      if((curMillis - oneWireLastCommandTimer) > 15000) 
      {
         #ifdef _DEBUG
            Serial.print(F("Last command at: "));
            Serial.print(oneWireLastCommandTimer);
            Serial.print(F("; curMillis: "));
            Serial.print(curMillis);
            Serial.print(F("; diff = "));
            Serial.println((curMillis - oneWireLastCommandTimer));
         #endif
        
          connectedViaOneWire = false; // ���������� ����� 1-Wire ���������
      }
  }
  


  if(!connectedViaOneWire && ((curMillis - last_measure_at) > query_interval) && !measureTimerEnabled && !needToMeasure) 
  {
    // ����-�� ����� �� ��������� �����������, ��������, �������
      // � �������� ������ �����, ����� �� �� ���������� � 1-Wire, ����� - ������ ��� �������� �����������.
        needToMeasure = true;
        #ifdef _DEBUG
          Serial.println(F("Want measure by timeout..."));
        #endif        
  }

  // ������ ���� ������ �� ������ �� ����� 1-Wire � ��������� �����������
  
  if(needToMeasure && !measureTimerEnabled) 
  {
    #ifdef _DEBUG
      Serial.println(F("Want measure..."));
    #endif    

    measureTimerEnabled = true; // �������� ����, ��� �� ������ ��������� ������ � ��������
    sensorsUpdateTimer = curMillis; // ���������� ������ ����������
    StartMeasure();

    needToMeasure = false;
    
    #ifdef _DEBUG
      Serial.println(F("Wait for measure complete..."));
    #endif    
  }
 

  if(measureTimerEnabled) 
  {
    UpdateSensors(); // ��������� �������, ���� ����-�� �� ��� ����� ������������� ����������
  }
  
  if(measureTimerEnabled && ((curMillis - sensorsUpdateTimer) > MEASURE_MIN_TIME)) 
  {
    
    if(state != DS_SendingScratchpad)
    {

          #ifdef _DEBUG
            Serial.println(F("Measure completed, start read..."));
          #endif
        
             // ����� ������ ���������� � ��������
             ReadSensors();
             
             //noInterrupts();
             memcpy(&scratchpadToSend,&scratchpadS,sizeof(scratchpadS));
             scratchpadToSend.crc8 = OneWireSlave::crc8((const byte*) &scratchpadToSend,sizeof(scratchpadS)-1);
             //interrupts();
        

          #ifdef _DEBUG
            Serial.println(F("Sensors data readed."));
          #endif     
        

        
        
             // ������ �������� ��� �������
             PowerDownSensors();
        
             // ���������, ����������
             #ifdef USE_NRF
              if(!connectedViaOneWire)
                sendDataViaNRF();
             #endif

             #ifdef USE_LORA
              if(!connectedViaOneWire)
                sendDataViaLoRa();
             #endif             
        
          #ifdef _DEBUG
            Serial.println(F(""));
          #endif 
          
             sensorsUpdateTimer = curMillis;
             measureTimerEnabled = false;

           // �������� ������� �� ���� �������
           #ifdef USE_LOW_POWER
            #ifndef USE_RS485_GATE
                if(!connectedViaOneWire)
                {
                   LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);  
                   extern volatile unsigned long timer0_millis;
                   noInterrupts();
                    timer0_millis += 1000;
                   interrupts();
                }
            #endif // USE_RS485_GATE
           #endif // USE_LOW_POWER
             
      } // if(state != DS_SendingScratchpad)      
  }

  #ifdef USE_RS485_GATE
    if(!connectedViaOneWire)
      ProcessIncomingRS485Packets(); // ������������ �������� ������ �� RS-485
  #endif  

}
//----------------------------------------------------------------------------------------------------------------
void yield()
{
   #ifdef USE_RS485_GATE
    if(!connectedViaOneWire)
      ProcessIncomingRS485Packets(); // ������������ �������� ������ �� RS-485
  #endif   
}
//----------------------------------------------------------------------------------------------------------------
