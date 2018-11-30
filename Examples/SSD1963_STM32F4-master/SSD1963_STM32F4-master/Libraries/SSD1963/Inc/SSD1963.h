#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdint.h>
#include "SSD1963_Configuration.h"

/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech. SSD1963 LCD controller driver
 *****************************************************************************
 * FileName:        SSD1963.h
 * Dependencies:    p24Fxxxx.h or plib.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:			TechToys Company
 * Remarks:			The origin of this file was the ssd1926.c driver released
 *					by Microchip Technology Incorporated. 
 *
 * Company:         Microchip Technology Incorporated
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
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS?WITHOUT WARRANTY OF ANY
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
 * Anton Alkhimenok     08/27/08
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
* Remarks:
* Port ot Microchip Graphics Library v2.00
* (1) Only BLOCKING CONFIGURATION is supported
* (2) GetPixel() not working yet.
* John Leung @ TechToys Co.			15th Jan 2010
* www.TechToys.com.hk
******************************************************************************
*/

#ifndef _SSD1963_H
#define _SSD1963_H

//include the command table for SSD1963
#include "SSD1963_CMD.h"


/*
#ifndef DISP_HOR_RESOLUTION
#error  DISP_HOR_RESOLUTION must be defined in HardwareProfile_xxx.h
#endif

#ifndef DISP_VER_RESOLUTION
#error  DISP_VER_RESOLUTION must be defined in HardwareProfile_xxx.h
#endif

#ifndef COLOR_DEPTH
#error  COLOR_DEPTH must be defined in HardwareProfile_xxx.h
#endif

#ifndef DISP_ORIENTATION
#error  DISP_ORIENTATION must be defined in HardwareProfile_xxx.h
#endif
*/
/*********************************************************************
* Overview: Horizontal synchronization timing in pixels
*                  (from the glass datasheet).
*********************************************************************/
/*
#ifndef DISP_HOR_PULSE_WIDTH
#error  DISP_HOR_PULSE_WIDTH must be defined in HardwareProfile_xxx.h
#endif
#ifndef DISP_HOR_BACK_PORCH
#error  DISP_HOR_BACK_PORCH must be defined in HardwareProfile_xxx.h
#endif
#ifndef DISP_HOR_FRONT_PORCH
#error  DISP_HOR_FRONT_PORCH must be defined in HardwareProfile_xxx.h
#endif
*/

/*********************************************************************
* Overview: Vertical synchronization timing in lines
*                  (from the glass datasheet).
*********************************************************************/
/*
#ifndef DISP_VER_PULSE_WIDTH
#error  DISP_VER_PULSE_WIDTH must be defined in HardwareProfile_xxx.h
#endif
#ifndef DISP_VER_BACK_PORCH
#error  DISP_VER_BACK_PORCH must be defined in HardwareProfile_xxx.h
#endif
#ifndef DISP_VER_FRONT_PORCH
#error  DISP_VER_FRONT_PORCH must be defined in HardwareProfile_xxx.h
#endif
*/

/*********************************************************************
* PARAMETERS VALIDATION
*********************************************************************/
#if COLOR_DEPTH != 16
#error This driver supports 16 BPP only.
#endif

#if (DISP_HOR_RESOLUTION % 8) != 0
#error Horizontal resolution must be divisible by 8.
#endif

#if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180) && (DISP_ORIENTATION != 90) && (DISP_ORIENTATION != 270)
#error The display orientation selected is not supported. It can be only 0,90,180 or 270.
#endif

/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function. 
*********************************************************************/
#define CLIP_DISABLE       0 	// Disables clipping.
#define CLIP_ENABLE        1	// Enables clipping.


/*********************************************************************
* Macros: RGB565CONVERT(red, green, blue)
*
* Overview: Converts true color into 5:6:5 RGB format.
*
* PreCondition: none
*
* Input: Red, Green, Blue components.
*
* Output: 5 bits red, 6 bits green, 5 bits blue color.
*
* Side Effects: none
*
********************************************************************/
    #define RGB565CONVERT(red, green, blue) (u16) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))
    

/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
#define BLACK               RGB565CONVERT(0,    0,      0)
#define BRIGHTBLUE          RGB565CONVERT(0,    0,      255)
#define BRIGHTGREEN         RGB565CONVERT(0,    255,    0)
#define BRIGHTCYAN          RGB565CONVERT(0,    255,    255)
#define BRIGHTRED           RGB565CONVERT(255,  0,      0)
#define BRIGHTMAGENTA       RGB565CONVERT(255,  0,      255)
#define BRIGHTYELLOW        RGB565CONVERT(255,  255,    0)
#define BLUE                RGB565CONVERT(0,    0,      128)
#define GREEN               RGB565CONVERT(0,    128,    0)
#define CYAN                RGB565CONVERT(0,    128,    128)
#define RED                 RGB565CONVERT(128,  0,      0)
#define MAGENTA             RGB565CONVERT(128,  0,      128)
#define BROWN               RGB565CONVERT(255,  128,    0)
#define LIGHTGRAY           RGB565CONVERT(128,  128,    128)
#define DARKGRAY            RGB565CONVERT(64,   64,     64)
#define LIGHTBLUE           RGB565CONVERT(128,  128,    255)
#define LIGHTGREEN          RGB565CONVERT(128,  255,    128)
#define LIGHTCYAN           RGB565CONVERT(128,  255,    255)
#define LIGHTRED            RGB565CONVERT(255,  128,    128)
#define LIGHTMAGENTA        RGB565CONVERT(255,  128,    255)
#define YELLOW              RGB565CONVERT(255,  255,    128)
#define WHITE               RGB565CONVERT(255,  255,    255)
                            
#define GRAY0       	    RGB565CONVERT(224,  224,    224)
#define GRAY1         	    RGB565CONVERT(192,  192,    192)   
#define GRAY2               RGB565CONVERT(160,  160,    160)   
#define GRAY3               RGB565CONVERT(128,  128,    128)
#define GRAY4               RGB565CONVERT(96,   96,     96)
#define GRAY5               RGB565CONVERT(64,   64,     64)
#define GRAY6	            RGB565CONVERT(32,   32,     32)


#define Set_Cs  GPIO_SetBits(GPIOC,GPIO_Pin_8);
#define Clr_Cs  GPIO_ResetBits(GPIOC,GPIO_Pin_8);

#define Set_Rs  GPIO_SetBits(GPIOC,GPIO_Pin_9);
#define Clr_Rs  GPIO_ResetBits(GPIOC,GPIO_Pin_9);

#define Set_nWr GPIO_SetBits(GPIOC,GPIO_Pin_10);
#define Clr_nWr GPIO_ResetBits(GPIOC,GPIO_Pin_10);

#define Set_nRd GPIO_SetBits(GPIOC,GPIO_Pin_11);
#define Clr_nRd GPIO_ResetBits(GPIOC,GPIO_Pin_11);

#define Set_Rst GPIO_SetBits(GPIOC,GPIO_Pin_12);
#define Clr_Rst GPIO_ResetBits(GPIOC,GPIO_Pin_12);


/*********************************************************************
* Overview: Clipping region control and border settings.
*********************************************************************/
// Clipping region enable control
extern u16 _clipRgn;

// Left clipping region border
extern u16 _clipLeft;
// Top clipping region border
extern u16 _clipTop;
// Right clipping region border
extern u16 _clipRight;
// Bottom clipping region border
extern u16 _clipBottom;


// Active and Visual Pages 
extern u8 _activePage;
extern u8	_visualPage;


/*********************************************************************
* Macros:  LCD_WriteCommand(cmd)
*
* PreCondition: 
*
* Input: cmd - controller command
*
* Output: none
*
* Side Effects: none
*
* Overview: writes command
*
* Note: none
********************************************************************/
#define LCD_WriteCommand(cmd) {Clr_Rs; GPIOB->ODR=((GPIOB->ODR&0x00ff)|(cmd<<8)); GPIOC->ODR=((GPIOC->ODR&0xff00)|(cmd>>8)); Clr_Cs; Clr_nWr;  Set_nWr; Set_Cs;};

	
	

/*********************************************************************
* Function:  void  LCD_WriteData(WORD data)
*
* PreCondition: 
*
* Input:  value - value to be written in WORD format
*
* Output: none
*
* Side Effects: none
*
* Overview: 
********************************************************************/
#define LCD_WriteData(data) {Set_Rs; GPIOB->ODR=((GPIOB->ODR&0x00ff)|(data<<8)); GPIOC->ODR=((GPIOC->ODR&0xff00)|(data>>8));  Clr_nWr; Set_nWr;};


void LCD_Clear(u16 color);

/*********************************************************************
* Function:  void ResetDevice()
*
* Overview: Initializes LCD module.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void LCD_ResetDevice(void);

/*********************************************************************
* Macros:  GetMaxX()
*
* Overview: Returns maximum horizontal coordinate for visual display
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum horizontal coordinates.
*
* Side Effects: none
*
* Note: 
********************************************************************/
#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

#define	GetMaxX() 			(DISP_VER_RESOLUTION-1)
	

#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

#define	GetMaxX() 			(DISP_HOR_RESOLUTION-1)

#endif

/*********************************************************************
* Macros:  GetMaxY()
*
* Overview: Returns maximum vertical coordinate for visual display
*			
* PreCondition: none
*
* Input: none
*
* Output: Maximum vertical coordinates.
*
* Side Effects: none
*
* Note: 
********************************************************************/
#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

#define	GetMaxY() 			(DISP_HOR_RESOLUTION-1)

#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

#define	GetMaxY() 			(DISP_VER_RESOLUTION-1)

#endif

// Definition for memory page size with virtual display taken into account
// The maximum memory space of SSD1928 is 262kbyte
#ifndef PAGE_MEM_SIZE
// Memory allocation for a single page 
#define PAGE_MEM_SIZE 	(GetMaxX()+1)*(GetMaxY()+1)*COLOR_DEPTH/8

#if PAGE_MEM_SIZE > 1215000ul	//this is the memory boundary of ssd1963
#error MEMORY ALLOCATION FAILED. CHECK SSD1963.h
#endif

#endif

/*********************************************************************
* Macros:  SetActivePage(page)
*
* Overview: Sets active graphic page.
*
* PreCondition: none
*
* Input: page - Graphic page number.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define LCD_SetActivePage(page)	_activePage = page

/*********************************************************************
* Macros: SetVisualPage(page)
*
* Overview: Sets graphic page to display.
*
* PreCondition: none
*
* Input: page - Graphic page number
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
//debug
#define LCD_SetVisualPage(page) {_visualPage = page; \
LCD_SetScrollArea(0,LCD_GetMaxY()+1,0); \
LCD_SetScrollStart((SHORT)_visualPage*(LCD_GetMaxY()+1));}
//debug

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* Overview: Puts pixel with the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void LCD_PutPixel(u16 x, u16 y, u16 color);

/*********************************************************************
* Macros: SetClipRgn(left, top, right, bottom)
*
* Overview: Sets clipping region.
*
* PreCondition: none
*
* Input: left - Defines the left clipping region border.
*		 top - Defines the top clipping region border.
*		 right - Defines the right clipping region border.
*	     bottom - Defines the bottom clipping region border.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define LCD_SetClipRgn(left,top,right,bottom) _clipLeft=left; _clipTop=top; _clipRight=right; _clipBottom=bottom;

/*********************************************************************
* Macros: GetClipLeft()
*
* Overview: Returns left clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Left clipping border.
*
* Side Effects: none
*
********************************************************************/
#define LCD_GetClipLeft() _clipLeft

/*********************************************************************
* Macros: GetClipRight()
*
* Overview: Returns right clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Right clipping border.
*
* Side Effects: none
*
********************************************************************/
#define LCD_GetClipRight() _clipRight

/*********************************************************************
* Macros: GetClipTop()
*
* Overview: Returns top clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Top clipping border.
*
* Side Effects: none
*
********************************************************************/
#define LCD_GetClipTop() _clipTop

/*********************************************************************
* Macros: GetClipBottom()
*
* Overview: Returns bottom clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Bottom clipping border.
*
* Side Effects: none
*
********************************************************************/
#define LCD_GetClipBottom() _clipBottom

/*********************************************************************
* Macros: SetClip(control)
*
* Overview: Enables/disables clipping.
*
* PreCondition: none
*
* Input: control - Enables or disables the clipping.
*			- 0: Disable clipping
*			- 1: Enable clipping
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define LCD_SetClip(control) _clipRgn=control;

/*********************************************************************
* Macros: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
#define LCD_IsDeviceBusy()  0

/*********************************************************************
* Macros: SetPalette(colorNum, color)
*
* Overview:  Sets palette register.
*
* PreCondition: none
*
* Input: colorNum - Register number.
*        color - Color.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define LCD_SetPalette(colorNum, color)

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
*
********************************************************************/
void LCD_SetScrollArea(u16 top, u16 scroll, u16 bottom);

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
*		SetScrollArea(0,272,272);
*		for(line=0;line<272;line++) {SetScrollStart(line);DelayMs(100);}
*		
*		Code above scrolls the whole page upwards in 100ms interval 
*		with page 2 replacing the first page in scrolling
********************************************************************/
void LCD_SetScrollStart(u16 line);

/*********************************************************************
* Function:  void EnterSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 enters sleep mode
* Note: Host must wait 5mS after sending before sending next command
********************************************************************/
void LCD_EnterSleepMode (void);

/*********************************************************************
* Function:  void ExitSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 exits sleep mode
* Note:   cannot be called sooner than 15
********************************************************************/
void LCD_ExitSleepMode (void);

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
void LCD_SetBacklight(u8 intensity);

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
void LCD_SetTearingCfg(bool state, bool mode);


/************************************************************************
* Macro: Lo                                                             *
* Preconditions: None                                                   *
* Overview: This macro extracts a low byte from a 2 byte word.          *
* Input: None.                                                          *
* Output: None.                                                         *
************************************************************************/
#define Lo(X)   (u8)(X&0x00ff)

/************************************************************************
* Macro: Hi                                                             *
* Preconditions: None                                                   *
* Overview: This macro extracts a high byte from a 2 byte word.         *
* Input: None.                                                          *
* Output: None.                                                         *
************************************************************************/
#define Hi(X)   (u8)((X>>8)&0x00ff)

void LCD_Delay(vu32 nCount);

#endif // _SSD1963_H
