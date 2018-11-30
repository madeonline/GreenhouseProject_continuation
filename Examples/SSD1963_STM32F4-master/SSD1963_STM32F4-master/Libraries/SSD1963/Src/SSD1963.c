#include "stm32f4xx.h"
#include <stdint.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "SSD1963.h"

/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech. SSD1963 LCD controller driver
 *****************************************************************************
 * FileName:        SSD1963.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:			TechToys Company
 * Remarks:			The origin of this file was the ssd1926.c driver released
 *					by Microchip Technology Incorporated. 
 *
 * Software License Agreement as below:
 *
 * Company:         Microchip Technology Incorporatedpage

 *
 * Software License Agreement
 *
 * Copyright ?2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED ?S IS?WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok     08/27/08	(Original)
 * 	
 *****************************************************************************/

/*
******************************************************************************
* Versions 120709
*
* John Leung @ TechToys Co.			12/07/09
* www.TechToys.com.hk
******************************************************************************
*/

/*
******************************************************************************
* Remarks: 
* 1. Removed hardware definition for LED_LAT_BIT and LED_TRIS_BIT
*	  because PWM pin of SSD1963 applied, therefore backlight intensity
*	  set by software
* 2. Add new function void SetBacklight(BYTE intensity)
* 3. Funny finding, PLL to 120MHz work only when 10MHz crystal applied with 
* 	  multiplier N = 35. A crystal with 4MHz attempted but the PLL frequency 
*	  failed to boost to 120MHz somehow!
*
* John Leung @ TechToys Co.			09/09/2009
* www.TechToys.com.hk
******************************************************************************
*/

/*
******************************************************************************
* Revision:
* Port ot Microchip Graphics Library v2.00
* (1) Only BLOCKING CONFIGURATION is supported
* (2) GetPixel() not working yet.
* John Leung @ TechToys Co.			15th Jan 2010
* www.TechToys.com.hk
******************************************************************************
*/


/*
******************************************************************************
* Revision:
* (1) Optimize for LCD_WriteData() by removing CS_LAT_BIT for each write cycle
*	  and append CS strobe between multiple LCD_WriteData()
*
* John Leung @ TechToys Co.			3rd Feb 2010
* www.TechToys.com.hk
******************************************************************************
*/

// Color
u16  _color;
// Clipping region control
u16 _clipRgn;
// Clipping region borders
u16 _clipLeft;
u16 _clipTop;
u16 _clipRight;
u16 _clipBottom;

// Active Page
u8  _activePage;
// Visual Page
u8  _visualPage;

// ssd1963 specific
u8 _gpioStatus = 0;

void LCD_SetArea(u16 start_x, u16 start_y, u16 end_x, u16 end_y);




/*********************************************************************
* Macros:  PMPWaitBusy()
*
* Overview: waits for PMP cycle end.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Note: 
********************************************************************/
#define PMPWaitBusy()  Nop();


/*********************************************************************
* Function:  LCD_SetArea(start_x,start_y,end_x,end_y)
*
* PreCondition: SetActivePage(page)
*
* Input: start_x, end_x	- start column and end column
*		 start_y,end_y 	- start row and end row position (i.e. page address)
*
* Output: none
*
* Side Effects: none
*
* Overview: defines start/end columns and start/end rows for memory access
*			from host to SSD1963
* Note: none
********************************************************************/
void LCD_SetArea(u16 start_x, u16 start_y, u16 end_x, u16 end_y)
{
	long offset;

	offset = (u16)_activePage*(GetMaxY()+1);

	start_y = offset + start_y;
	end_y   = offset + end_y;



	LCD_WriteCommand(CMD_SET_COLUMN);
	Clr_Cs;
	LCD_WriteData(start_x>>8);
	LCD_WriteData(start_x);
	LCD_WriteData(end_x>>8);
	LCD_WriteData(end_x);
	Set_Cs;
	LCD_WriteCommand(CMD_SET_PAGE);
	Clr_Cs;
	LCD_WriteData(start_y>>8);
	LCD_WriteData(start_y);
	LCD_WriteData(end_y>>8);
	LCD_WriteData(end_y);
	Set_Cs;
}

/*********************************************************************
* Function:  SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* PreCondition: none
*
* Input: top - Top Fixed Area in number of lines from the top
*				of the frame buffer
*		 scroll - Vertical scrolling area in number of lines
*		 bottom - Bottom Fixed Area in number of lines
*
* Output: none
*
* Side Effects: none
*
* Overview:
*
* Note: Reference: section 9.22 Set Scroll Area, SSD1963 datasheet Rev0.20
********************************************************************/
void LCD_SetScrollArea(u16 top, u16 scroll, u16 bottom)
{
	LCD_WriteCommand(CMD_SET_SCROLL_AREA);
	Clr_Cs;
	LCD_WriteData(top>>8);
	LCD_WriteData(top);
	LCD_WriteData(scroll>>8);
	LCD_WriteData(scroll);
	LCD_WriteData(bottom>>8);
	LCD_WriteData(bottom);
	Set_Cs;	
}

/*********************************************************************
* Function:  void  SetScrollStart(SHORT line)
*
* Overview: First, we need to define the scrolling area by SetScrollArea()
*			before using this function. 
*
* PreCondition: SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* Input: line - Vertical scrolling pointer (in number of lines) as 
*		 the first display line from the Top Fixed Area defined in SetScrollArea()
*
* Output: none
*
* Note: Example -
*
*		SHORT line=0;
*		SetScrollArea(0,272,0);
*		for(line=0;line<272;line++) {SetScrollStart(line);LCD_Delay(100);}
*		
*		Code above scrolls the whole page upwards in 100ms interval 
*		with page 2 replacing the first page in scrolling
********************************************************************/
void LCD_SetScrollStart(u16 line)
{
	LCD_WriteCommand(CMD_SET_SCROLL_START);
	Clr_Cs;
	LCD_WriteData(line>>8);
	LCD_WriteData(line);	
	Set_Cs;
}

/*********************************************************************
* Function:  void EnterSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 enters sleep mode
* Note: Host must wait 5mS after sending before sending next command
********************************************************************/
void LCD_EnterSleepMode (void)
{
	LCD_WriteCommand(CMD_ENT_SLEEP);
}

/*********************************************************************
* Function:  void ExitSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 enters sleep mode
* Note:   none
********************************************************************/
void LCD_ExitSleepMode (void)
{
	LCD_WriteCommand(CMD_EXIT_SLEEP);
}

/*********************************************************************
* Function:  void  SetBacklight(BYTE intensity)
*
* Overview: This function makes use of PWM feature of ssd1963 to adjust
*			the backlight intensity. 
*
* PreCondition: Backlight circuit with shutdown pin connected to PWM output of ssd1963.
*
* Input: 	(BYTE) intensity from 
*			0x00 (total backlight shutdown, PWM pin pull-down to VSS)
*			0xff (99% pull-up, 255/256 pull-up to VDD)
*
* Output: none
*
* Note: The base frequency of PWM set to around 300Hz with PLL set to 120MHz.
*		This parameter is hardware dependent
********************************************************************/
void LCD_SetBacklight(u8 intensity)
{
	LCD_WriteCommand(0xBE);			// Set PWM configuration for backlight control
	Clr_Cs;
	LCD_WriteData(0x0E);			// PWMF[7:0] = 2, PWM base freq = PLL/(256*(1+5))/256 = 
								// 300Hz for a PLL freq = 120MHz
	LCD_WriteData(intensity);		// Set duty cycle, from 0x00 (total pull-down) to 0xFF 
								// (99% pull-up , 255/256)
	LCD_WriteData(0x01);			// PWM enabled and controlled by host (mcu)
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	Set_Cs;
}

/*********************************************************************
* Function:  void  SetTearingCfg(BOOL state, BOOL mode)
*
* Overview: This function enable/disable tearing effect
*
* PreCondition: none
*
* Input: 	BOOL state -	1 to enable
*							0 to disable
*			BOOL mode -		0:  the tearing effect output line consists
*								of V-blanking information only
*							1:	the tearing effect output line consists
*								of both V-blanking and H-blanking info.
* Output: none
*
* Note:
********************************************************************/
void LCD_SetTearingCfg(bool state, bool mode)
{


	if(state == 1)
	{
		LCD_WriteCommand(0x35);
		Clr_Cs;
		LCD_WriteData(mode&0x01);
		Set_Cs;
	}
	else
	{
		LCD_WriteCommand(0x34);
	}


}

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Resets LCD, initializes PMP
*			Initialize low level IO port for mcu,
*			initialize SSD1963 for PCLK,
*			HSYNC, VSYNC etc
*
* Note: Need to set the backlight intensity by SetBacklight(BYTE intensity)
*		in main()
*
********************************************************************/
void LCD_ResetDevice(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; // Initialization of Data pins
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


    Set_Rst;		
	Set_Cs;        	// SSD1963 is not selected by default
	Set_nRd;
	Set_nWr;

	Clr_Rst;
	LCD_Delay(1);						// wait stablize
	Set_Rst;       // release from reset state to sleep state

	//Set MN(multipliers) of PLL, VCO = crystal freq * (N+1)
	//PLL freq = VCO/M with 250MHz < VCO < 800MHz
	//The max PLL freq is around 120MHz. To obtain 120MHz as the PLL freq



	LCD_WriteCommand(0xE2);				// Set PLL with OSC = 10MHz (hardware)
									// Multiplier N = 35, VCO (>250MHz)= OSC*(N+1), VCO = 360MHz
	Clr_Cs;
	LCD_WriteData(0x23);
	LCD_WriteData(0x02);				// Divider M = 2, PLL = 360/(M+1) = 120MHz
	LCD_WriteData(0x54);				// Validate M and N values
	Set_Cs;	

	LCD_WriteCommand(0xE0);				// Start PLL command
	Clr_Cs;
	LCD_WriteData(0x01);				// enable PLL
	Set_Cs;	

	LCD_Delay(1);						// wait stablize
	LCD_WriteCommand(0xE0);				// Start PLL command again
	Clr_Cs;
	LCD_WriteData(0x03);				// now, use PLL output as system clock	
	Set_Cs;	

	LCD_WriteCommand(0x01);				// Soft reset
	LCD_Delay(10);

	//Set LSHIFT freq, i.e. the DCLK with PLL freq 120MHz set previously
	//Typical DCLK for TY700TFT800480 is 33.3MHz(datasheet), experiment shows 30MHz gives a stable result
	//30MHz = 120MHz*(LCDC_FPR+1)/2^20
	//LCDC_FPR = 262143 (0x3FFFF)
	//Time per line = (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)/30 us = 1056/30 = 35.2us
	LCD_WriteCommand(0xE6);
	Clr_Cs;
	LCD_WriteData(0x04);
	LCD_WriteData(0xff);
	LCD_WriteData(0xff);
	
	Set_Cs;
	
	//Set panel mode, varies from individual manufacturer
	LCD_WriteCommand(0xB0);

	Clr_Cs;
	LCD_WriteData(0x10);				// set 18-bit for 7" panel TY700TFT800480
	LCD_WriteData(0x80);				// set TTL mode
	LCD_WriteData((DISP_HOR_RESOLUTION-1)>>8); //Set panel size
	LCD_WriteData(DISP_HOR_RESOLUTION-1);
	LCD_WriteData((DISP_VER_RESOLUTION-1)>>8);
	LCD_WriteData(DISP_VER_RESOLUTION-1);
	LCD_WriteData(0x00);				//RGB sequence	
	Set_Cs;


	//Set horizontal period
	LCD_WriteCommand(0xB4);
	#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
	Clr_Cs;
	LCD_WriteData((HT-1)>>8);	
	LCD_WriteData(HT-1);
	#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
	LCD_WriteData((HPS-1)>>8);
	LCD_WriteData(HPS-1);
	LCD_WriteData(DISP_HOR_PULSE_WIDTH-1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	Set_Cs;
	//Set vertical period
	LCD_WriteCommand(0xB6);
	#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
	Clr_Cs;
	LCD_WriteData((VT-1)>>8);
	LCD_WriteData(VT-1);
	#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)
	LCD_WriteData((VSP-1)>>8);
	LCD_WriteData(VSP-1);
	LCD_WriteData(DISP_VER_PULSE_WIDTH-1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	Set_Cs;
	
	//Set pixel format, i.e. the bpp
	LCD_WriteCommand(0x3A);
	Clr_Cs;
	LCD_WriteData(0x55); 				// set 16bpp
	Set_Cs;

	//Set pixel data interface
	LCD_WriteCommand(0xF0);
	Clr_Cs;
	LCD_WriteData(0x03);				//16-bit(565 format) data for 16bpp PIC32MX only
	/*LCD_WriteData(0x00);				//8-bit data for 16bpp, PIC24 series
	Set_Cs;*/

	LCD_WriteCommand(0x29);				// Turn on display; show the image on display	
}

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: 
********************************************************************/
void LCD_PutPixel(u16 x, u16 y, u16 color)
{
	if(_clipRgn){
		if(x<_clipLeft)
			return;
		if(x>_clipRight)
			return;
		if(y<_clipTop)
			return;
		if(y>_clipBottom)
			return;
	}
  	
	LCD_SetArea(x,y,GetMaxX(),GetMaxY());
	LCD_WriteCommand(CMD_WR_MEMSTART);
	Clr_Cs;
	LCD_WriteData(color);
	Set_Cs;
}

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color 
*
* Note: none
*
********************************************************************/
void LCD_Clear(u16 color)
{
	u32     counter;
	u32 xcounter, ycounter;

	LCD_SetArea(0,0,GetMaxX(),GetMaxY());

	LCD_WriteCommand(CMD_WR_MEMSTART);

	Clr_Cs;
	for(ycounter=0;ycounter<GetMaxY()+1;ycounter++)
	{
		for(xcounter=0;xcounter<GetMaxX()+1;xcounter++)
		{
			LCD_WriteData(color);
		}
	}

	Set_Cs;
}


void LCD_Delay(vu32 nCount)
{
  nCount = nCount * 1000;
  for(; nCount != 0; nCount--);
}

void LCD_DelayUs(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}