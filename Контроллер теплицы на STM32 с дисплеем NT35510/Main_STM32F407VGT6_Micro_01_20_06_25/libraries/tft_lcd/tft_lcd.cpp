

#include "tft_lcd.h"
#include <Arduino.h>

//================================================================================
//================================================================================
//	

// подпрограмма задержки
void _delay_ms(uint32_t ms)
{
	delay(ms);
}

// *********  Отправка КОМАНДЫ  ************//
void ILI9341_SendCommand(uint16_t com) {
	//LCD_REG = com; 
	*(__IO uint16_t *)(0x60000000) = com;
}

// *********  Отправка 1 байта ДАННЫХ ************//
void ILI9341_SendData(uint16_t data) {
	//LCD_DATA = data;
	*(__IO uint16_t *)(0x60080000) = data;
}

// *********  Прием 1 байта ДАННЫХ ************//
uint16_t ILI9341_ResaveData(uint16_t data) {
	int tmp1;
	*(__IO uint16_t *)(0x60000000) = 0x0000;
	tmp1 = *(__IO uint16_t *)(0x60080000);
	return tmp1;
}




// Инициализируем FSMC контроллер, к которому подключен дисплей по шине Intel-8080 (D0...D15, RS, WR, RD, CS)
// Шина данных дисплея 16 бит
// Подключение дисплея к контроллеру:
// FSMC D0  <-> LCD D0
// FSMC D1  <-> LCD D1
// FSMC D2  <-> LCD D2
//         ...
// FSMC D15 <-> LCD D15
// FSMC NWE <-> LCD WR
// FSMC NOE <-> LCD RD
// FSMC NE1 <-> LCD CS
// FSMC A18 <-> LCD RS

#define PD_DATA_BITS 0b1100011100000011
#define PD_CTRL_BITS 0b0010000010110000  // PD13(FSMC_A18),PD7(FSMC_NE1),PD5(FSMC_NWE),PD4(FSMC_NOE)
#define PD_FSMC_BITS 0b1110011110110011
#define PD_AFR0_MASK         0xF0FF00FF
#define PD_AFR0_FSMC         0xC0CC00CC
#define PD_AFR1_MASK 0xFFF00FFF
#define PD_AFR1_FSMC 0xCCC00CCC
//                    |       |        |
#define PD_MODE_MASK 0xFC3FCF0F // all FMSC MODE bits
#define PD_MODE_FSMC 0xA82A8A0A // FMSC MODE values 10 alternate function
#define PD_OSPD_FSMC 0x54154505 // FMSC OSPEED values 01 10MHz
//                    |       |        |
#define PE_DATA_BITS 0b1111111110000000
#define PE_AFR0_MASK         0xF0000000
#define PE_AFR0_FSMC         0xC0000000
#define PE_AFR1_MASK 0xFFFFFFFF
#define PE_AFR1_FSMC 0xCCCCCCCC
//                    |       |        |
#define PE_MODE_MASK 0xFFFFC000 // all FMSC MODE bits
#define PE_MODE_FSMC 0xAAAA8000 // FMSC MODE values 10 alternate function
#define PE_OSPD_FSMC 0x55554000 // FMSC OSPEED values 01 10MHz

#define ADDSET 15 // (ADDSET+1)*6ns = CS to RW
#define DATAST 30 // (DATAST+1)*6ns = RW length

// these are defined for STM32F103V in
// ...\Arduino\hardware\STM32GENERIC\STM32\system\STM32F1\CMSIS_Inc\stm32f103xe.h
// but not for STM32F407V/Z, maybe because relocatable
#define FSMC_BASE             ((uint32_t)0x60000000) /*!< FSMC base address */
#define FSMC_BANK1            (FSMC_BASE)               /*!< FSMC Bank1 base address */

#define CommandAccess FSMC_BANK1
#define DataAccess (FSMC_BANK1 + 0x80000)


void Init_FSMC()
{

	RCC->AHB1ENR |= 0x00000078;
	volatile uint32_t t = RCC->AHB1ENR; // delay

	GPIOD->AFR[0] = (GPIOD->AFR[0] & ~PD_AFR0_MASK) | PD_AFR0_FSMC;
	GPIOD->AFR[1] = (GPIOD->AFR[1] & ~PD_AFR1_MASK) | PD_AFR1_FSMC;
	GPIOD->MODER = (GPIOD->MODER & ~PD_MODE_MASK) | PD_MODE_FSMC;
	GPIOD->OSPEEDR = (GPIOD->OSPEEDR & ~PD_MODE_MASK) | PD_OSPD_FSMC;
	GPIOD->OTYPER &= ~PD_MODE_MASK;
	GPIOD->PUPDR &= ~PD_MODE_MASK;

	GPIOE->AFR[0] = (GPIOE->AFR[0] & ~PE_AFR0_MASK) | PE_AFR0_FSMC;
	GPIOE->AFR[1] = (GPIOE->AFR[1] & ~PE_AFR1_MASK) | PE_AFR1_FSMC;
	GPIOE->MODER = (GPIOE->MODER & ~PE_MODE_MASK) | PE_MODE_FSMC;
	GPIOE->OSPEEDR = (GPIOE->OSPEEDR & ~PE_MODE_MASK) | PE_OSPD_FSMC;
	GPIOE->OTYPER &= ~PE_MODE_MASK;
	GPIOE->PUPDR &= ~PE_MODE_MASK;

	RCC->AHB3ENR |= 0x00000001;
	t = RCC->AHB1ENR; // delay
	(void)(t);

	FSMC_Bank1->BTCR[0] = 0x000010D9;
	FSMC_Bank1->BTCR[1] = (DATAST << 8) | ADDSET;

	digitalWrite(PB1, LOW);
	pinMode(PB1, OUTPUT);
	//LCD_LED_OFF;

}







void TFT_ILI9341_NEW::ILI9341_Init()
{
	Init_FSMC();

	pinMode(PB1, OUTPUT);
	digitalWrite(PB1, HIGH);
	
	//LCD_LED_ON;

	ILI9341_SendCommand(ILI9341_RESET); // software reset comand

	_delay_ms(100);
	ILI9341_SendCommand(ILI9341_DISPLAY_OFF); // display off
	
	//------------power control------------------------------
	ILI9341_SendCommand(ILI9341_POWER1); // power control
	ILI9341_SendData(0x26); // GVDD = 4.75v
	ILI9341_SendCommand(ILI9341_POWER2); // power control
	ILI9341_SendData(0x11); // AVDD=VCIx2, VGH=VCIx7, VGL=-VCIx3
	//--------------VCOM-------------------------------------
	ILI9341_SendCommand(ILI9341_VCOM1); // vcom control
	ILI9341_SendData(0x35); // Set the VCOMH voltage (0x35 = 4.025v)
	ILI9341_SendData(0x3e); // Set the VCOML voltage (0x3E = -0.950v)
	ILI9341_SendCommand(ILI9341_VCOM2); // vcom control
	ILI9341_SendData(0xbe); // 0x94 (0xBE = nVM: 1, VCOMH: VMH–2, VCOML: VML–2)

	//------------memory access control------------------------
	ILI9341_SendCommand(ILI9341_MAC); // memory access control
	  //ILI9341_SendData   (0x48); // 0048 my,mx,mv,ml,BGR,mh,0.0 (mirrors)
#if Dspl_Rotation_0_degr
	ILI9341_SendData(0x48); //0градусов
#elif Dspl_Rotation_90_degr
	ILI9341_SendData(0x28); //90градусов
#elif Dspl_Rotation_180_degr
	ILI9341_SendData(0x88); //180градусов
#elif Dspl_Rotation_270_degr
	ILI9341_SendData(0xE8); //270градусов
#endif	   

	ILI9341_SendCommand(ILI9341_PIXEL_FORMAT); // pixel format set
	ILI9341_SendData(0x55); // 16bit /pixel

	ILI9341_SendCommand(ILI9341_FRC);
	ILI9341_SendData(0);
#if Fram_Rate_61Hz
	ILI9341_SendData(0x1F);
#elif Fram_Rate_70Hz
	ILI9341_SendData(0x1B);
#elif Fram_Rate_90Hz
	ILI9341_SendData(0x15);
#elif Fram_Rate_100Hz
	ILI9341_SendData(0x13);
#elif Fram_Rate_119Hz
	ILI9341_SendData(0x10);
#endif

	//-------------ddram ----------------------------
	ILI9341_SendCommand(ILI9341_COLUMN_ADDR); // column set
	ILI9341_SendData(0x00); // x0_HIGH---0
	ILI9341_SendData(0x00); // x0_LOW----0
	ILI9341_SendData(0x00); // x1_HIGH---240
	ILI9341_SendData(0xEF); // x1_LOW----240
	ILI9341_SendCommand(ILI9341_PAGE_ADDR); // page address set
	ILI9341_SendData(0x00); // y0_HIGH---0
	ILI9341_SendData(0x00); // y0_LOW----0
	ILI9341_SendData(0x01); // y1_HIGH---320
	ILI9341_SendData(0x3F); // y1_LOW----320

	ILI9341_SendCommand(ILI9341_TEARING_OFF); // tearing effect off
	//LCD_write_cmd(ILI9341_TEARING_ON); // tearing effect on
	//LCD_write_cmd(ILI9341_DISPLAY_INVERSION); // display inversion
	ILI9341_SendCommand(ILI9341_Entry_Mode_Set); // entry mode set
	// Deep Standby Mode: OFF
	// Set the output level of gate driver G1~G320: Normal display
	// Low voltage detection: Disable
	ILI9341_SendData(0x07);
	//-----------------display------------------------
	ILI9341_SendCommand(ILI9341_DFC); // display function control
	//Set the scan mode in non-display area
	//Determine source/VCOM output in a non-display area in the partial display mode
	ILI9341_SendData(0x0a);
	//Select whether the liquid crystal type is normally white type or normally black type
	//Sets the direction of scan by the gate driver in the range determined by SCN and NL
	//Select the shift direction of outputs from the source driver
	//Sets the gate driver pin arrangement in combination with the GS bit to select the optimal scan mode for the module
	//Specify the scan cycle interval of gate driver in non-display area when PTG to select interval scan
	ILI9341_SendData(0x82);
	// Sets the number of lines to drive the LCD at an interval of 8 lines
	ILI9341_SendData(0x27);
	ILI9341_SendData(0x00); // clock divisor

	ILI9341_SendCommand(ILI9341_SLEEP_OUT); // sleep out
	_delay_ms(100);
	ILI9341_SendCommand(ILI9341_DISPLAY_ON); // display on
	_delay_ms(100);
	ILI9341_SendCommand(ILI9341_GRAM); // memory write
	_delay_ms(5);
	//ILI9341_Opts.Width = ILI9341_HEIGHT;
	//ILI9341_Opts.Height = ILI9341_WIDTH;
	//ILI9341_Opts.orientation = ILI9341_Landscape; //ILI9341_Portrait;	

	

}


uint16_t TFT_ILI9341_NEW::LCD_GetXSize()
{
	return ILI9341_HEIGHT;
}

uint16_t TFT_ILI9341_NEW::LCD_GetYSize()
{
	return ILI9341_WIDTH;
}



TFT_ILI9341_NEW TFT_ILI9341_NEW1;

