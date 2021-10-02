/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
 
/**
 * Fonts library
 */
#ifndef TM_FONTS_H
#define TM_FONTS_H

#include "stm32f4xx.h"
//#include "stm32f4xx.h"

//#define indent 5   // ????? (px)        // ??????? ????? ???????? ???? ????? ??????? (?????? ?????? ?? ??????? ?? ?????????? ???????)
//#define FONT      cousine_18ptBitmaps   // ????????? ?????? ?????
//#define FONT_info cousine_18ptFontInfo  // ???????? ??????, ?????? ? ?.?.
//extern const uint8_t cousine_18ptBitmaps[];
//extern const uint8_t cousine_18ptFontInfo[];	


/**
 * Font struct
 */
typedef struct {
	uint8_t FontWidth;
	uint8_t FontHeight;
	const uint16_t *data;
} FontDef_t;



#endif

