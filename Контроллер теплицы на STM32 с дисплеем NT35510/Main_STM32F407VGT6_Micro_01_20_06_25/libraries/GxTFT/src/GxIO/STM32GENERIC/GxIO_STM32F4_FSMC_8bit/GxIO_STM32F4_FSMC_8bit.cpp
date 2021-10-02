// created by Jean-Marc Zingg to be the GxIO_STM32F4_FSMC io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F407V, STM32F407Z boards with FMSC TFT connector, e.g. for
// https://www.aliexpress.com/item/Free-shipping-STM32F407VET6-development-board-Cortex-M4-STM32-minimum-system-learning-board-ARM-core-board/32618222721.html
//
// e.g. for display with matching connector
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F407V doc.
//
// this io class can be used with or adapted to other STM32F407V/Z processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package STM32GENERIC, board "BLACK F407VE/ZE/ZG boards".
// https://github.com/danieleff/STM32GENERIC

#if defined(ARDUINO_ARCH_STM32) && (defined(STM32F407VE) || defined(STM32F407ZE) || defined(STM32F407ZG) || defined(STM32F407VG))

#include "GxIO_STM32F4_FSMC_8bit.h"


// Для записи команд
#define ADDR_CMD    		0x60000000  
 // Для записи данных
#define ADDR_DATA    		 0x60040000  


GxIO_STM32F4_FSMC_8Bit::GxIO_STM32F4_FSMC_8Bit()
{
  _cs   = PD7;  // FSMC_NE1
  _rs   = PD13; // FSMC_A18
  _rst  = 0;    // not available, driven from NRST
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
//  _bl   = PB1;
//  _bl_active_high = bl_active_high;
}

void GxIO_STM32F4_FSMC_8Bit::reset()
{
	// reset 
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);   
}


void GxIO_STM32F4_FSMC_8Bit::init()
{

//Serial.println("INIT FSMC...");

//*

/* Peripheral clock enable */
    __HAL_RCC_FSMC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
	
	
 GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  /** FSMC GPIO Configuration  
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PD13   ------> FSMC_A18
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0 
                          |GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);  
  
  
  FSMC_NORSRAM_TimingTypeDef Timing = {0};
  SRAM_HandleTypeDef hsram1;

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 2;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 15;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
   //Serial.println("ERROR FSMC!"); while(1);
  }
  
 //Serial.println("INIT FSMC DONE!");

}

uint8_t GxIO_STM32F4_FSMC_8Bit::readDataTransaction()
{
  return *((uint8_t*)ADDR_DATA);
}

uint16_t GxIO_STM32F4_FSMC_8Bit::readData16Transaction()
{
  return *((uint16_t*)ADDR_DATA);
}

uint8_t GxIO_STM32F4_FSMC_8Bit::readData()
{
  return *((uint8_t*)ADDR_DATA);
}

uint16_t GxIO_STM32F4_FSMC_8Bit::readData16()
{
  return *((uint16_t*)ADDR_DATA);
}

uint32_t GxIO_STM32F4_FSMC_8Bit::readRawData32(uint8_t part)
{
  return *((uint32_t*)ADDR_DATA);
}

void GxIO_STM32F4_FSMC_8Bit::writeCommandTransaction(uint8_t c)
{
   *((volatile uint8_t*)ADDR_CMD) = 0;     // Это важно
   *((volatile uint8_t*)ADDR_CMD) = c;   
}

void GxIO_STM32F4_FSMC_8Bit::writeDataTransaction(uint8_t d)
{
    *((volatile uint8_t*)ADDR_DATA) = (d >> 8);
    *((volatile uint8_t*)ADDR_DATA) = (d & 0xFF); 
}

void GxIO_STM32F4_FSMC_8Bit::writeData16Transaction(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *((volatile uint8_t*)ADDR_DATA) = (d >> 8);
    *((volatile uint8_t*)ADDR_DATA) = (d & 0xFF); 
    num--;
  }
}

void GxIO_STM32F4_FSMC_8Bit::writeCommand(uint8_t c)
{
   *((volatile uint8_t*)ADDR_CMD) = 0;     // Это важно
   *((volatile uint8_t*)ADDR_CMD) = c;   
}

void GxIO_STM32F4_FSMC_8Bit::writeCommand16(uint16_t c)
{
  *((volatile uint16_t*)ADDR_CMD) = c;
}

void GxIO_STM32F4_FSMC_8Bit::writeData(uint8_t d)
{
    *((volatile uint8_t*)ADDR_DATA) = d;
}

void GxIO_STM32F4_FSMC_8Bit::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    *((volatile uint8_t*)ADDR_DATA) = *d;	
    d++;
    num--;
  }
}

void GxIO_STM32F4_FSMC_8Bit::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *((volatile uint8_t*)ADDR_DATA) = (d >> 8);
    *((volatile uint8_t*)ADDR_DATA) = (d & 0xFF); 
    num--;
  }
}

void GxIO_STM32F4_FSMC_8Bit::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}


void GxIO_STM32F4_FSMC_8Bit::startTransaction()
{
}

void GxIO_STM32F4_FSMC_8Bit::endTransaction()
{
	
}

void GxIO_STM32F4_FSMC_8Bit::selectRegister(bool rs_low)
{
}

void GxIO_STM32F4_FSMC_8Bit::setBackLight(bool lit)
{
 // digitalWrite(_bl, (lit == _bl_active_high));
}

#endif

