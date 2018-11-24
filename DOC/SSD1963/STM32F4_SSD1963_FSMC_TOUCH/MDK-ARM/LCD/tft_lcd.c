#include "stm32f4xx.h"

#include "tft_lcd.h"
#include "font.h"				   // шрифт 
#include "correct_ANSI.h" // коррекция выкинутых мною символов между рус. и ингл.
//#include "delay.h"         // требуется для ф-ции инициализации дисплея
//#include "img.h"
#include "stdlib.h"


char array [100]; 
/*******************************************************************/
//команда
void LCD_SendCommand(uint16_t com)
{
	CMD_ADR = com;
}
/*******************************************************************/
// данные..
void LCD_SendData(uint16_t data)
{   
	
    DAT_ADR= data; 	
}


void LCD_Clear( uint16_t color)
{	     
	LCD_SetCursorPosition(0, 0,  LCD_WIDTH - 1, LCD_HEIGHT - 1);
	  uint32_t n = LCD_PIXEL_COUNT;
		while (n) 
			{
				n--;
        DAT_ADR= color; 	
	  	}     
} 


 

// ********* Выделяем поле в памяти LCD ************// 
// только в это поле будет выведена выдаваемая инфа, остального как бы не существует
// x1 - начало вертикаль, у1 - начало горизонталь, x2 - конец вертикаль, у2 - конец горизонталь
void LCD_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) 
{
	LCD_SendCommand(ILI9341_COLUMN_ADDR); // для понимания см.дефайн
	LCD_SendData (y1 >> 8);                // "делим" два байта "пополам" и отправляем по одному
	LCD_SendData(y1 & 0xFF);
	LCD_SendData (y2 >> 8);
	LCD_SendData(y2 & 0xFF);


	LCD_SendCommand(ILI9341_PAGE_ADDR); // для понимания см.дефайн
  LCD_SendData(x1 >> 8);
	LCD_SendData(x1 & 0xFF);
	LCD_SendData(x2 >> 8);
	LCD_SendData(x2 & 0xFF);

  LCD_SendCommand(ILI9341_GRAM); // отправляем в память LCD
}




void WriteString(unsigned char x0,unsigned int y0,  char *s,unsigned int color)//, unsigned int back_color) заменить, если надо выводить конкретный цвет фона
{

		
  	unsigned char z, y, nb, he, wi, d, j, q, st1, en1,y2;
	  unsigned int x, i, k;
	  y2 = y0;            // копируем начальнуюу позицию для случая переноса длинной строки
		d = FONT_info [0];  // ширина символа, бАйт, берется из файла шрифта
		wi=d*8;             // вычисляемая ширина символа , бИт 
	  he = FONT_info [1]; // высота символа, строк, берется из файла шрифта
		nb = (he*d);        //вычисляем кол-во байт на один символ

  for(z = 0;s[z]!='\0';z++) // перебираем все символы в строке, пока не дойдем до пустышки
    {
		if(s [z] < 128) 										// если символ латинница, то..
    {i = (s [z]-32)*nb;}                //корректируем только на первые 32 символа
    else if (s [z] > 191)  							// если символ русский, то..
		{i =(s [z]-32-(FONT_info [2]))*nb;} //пропуск кол-ва симв., между 127-м и 192-м по ANSI		(см.файл шрифта в конце)
		else 																// если между рус. и англ, т.е. между 127 и 192, то	
		{ 																	// идем корректировать в файл correct_ANSI.c в соответствии с нашими требованиями 
			i = (corr_ANSI ((unsigned char*)s, z))*nb;				// , т.е. смотря сколько мы выкинули из шрифта всяких символов - кракозябр
		}
																     // теперь получаем реальную ширину текущего символа, т.е. вычисляем ширину пустоты слева и справа от символа
																     // чтобы отрезать слишком широкие поля, скажем, от маленькой точки (PS. высота символа не меняется)
		x = i;                         // копируем номер первого байта символа в массиве (указываем байт, с которого начинается символ в массиве)
		st1=0;											   // просто очистка, от результатов предыдущего символа
		en1=0;											   // -*-*-
		for (q = 0; q < he; q++)       // перебираем (проходим) "строки" символа
		{				
			for(j = 0, k = 0; j < d; j++)// перебираем все байты строки, зайдя сюда выводим все байты текущей строки, 
			{ 													 // PS. "к" - счетчик байт с этой строке, ессно сбрасуется перед новой строкой
				 	y = 8;      						 // счетчик бИт одного байта строки
          while (y--) 						 // перебираем и проверяем бит за битом
            {
							k++;					       // прибавляем счетчик бита в строке							
		      		if (((FONT[x]&(1<<y)) && (st1 == 0))||((FONT[x]&(1<<y)) && (k < st1))) {st1 = k;} // ищем среди всех строк самый левый бит =1							
	      			if  ((FONT[x]&(1<<y)) && (en1 < k))  	{en1 = k;}							// ищем среди всех строк самый ПРАВЫЙ бит =1
		      	}
						x++; 									// следующий байт этого символа
			}	
		}	
		
		if (st1 > 0) {st1--;} 				// немного уменьшаем, чтобы символы не "слипались"
		if (en1 < 2){en1 = wi/3;}				// если символ пустой, то это пробел, задаем ему ширину 1/3 от ширины символа
		else if (en1 < wi){en1 = (en1 - st1 + indent);} // высчитываем реальную ширину и прибавляем отступ (см. font.h), дабы обеспечить расстояние между симв.

		j=0;      // обнуляем "отделитель-указатель" что это первый байт в строке
		k = nb+i; // получаем номер последнего (в массиве) байта  этого символа
		q = en1;  // копируем ширину символа, БИТ , en1 остается "хранительницей" ширины этого символа, пока выводятся "строки"
		
		
		if ((y0+en1) > LCD_HEIGHT) {y0=y2; x0++;}  // если последний символ не вмещается, то переносим на следующую строку (LCD_HEIGHT - см.дефайн)
																							 // если не нужен перенос, то оставшиеся символы следует "убить" иначе в конце сроки будет клякса
		LCD_SetCursorPosition(x0*he, y0, (x0*he)+(he-1), (y0)+(en1-1)); // выделяем поле в памяти LCD, х - вертикаль, у - горизонталь
		y0=y0+en1; 																											// указываем у0, где в строке будет начало следующего символа
		
		
     for(x = i; x < k; x++) // проходим (в массиве) по очереди все байты текущего символа
    {
					if (j++ == 0) {y = 10-st1;} // если это первый байт строки (j=0), то отнимаем пустые биты сначал строки, но прибавляем промежуток 2 pt
					else {y = 8;}               // значит это не первый байт строки
       while (y--)                    // выводим байт строки, т.е. проверяем все биты (?=0 или ?=1) этого байта
        {									
       if((FONT[x]&(1<<y))!=0)        // бит =1 ? или =0 ?
       { LCD_SendData(color);}     // если ДА (=1) пишем цвет  символа (16-bit SPI)
			 		 
       else
				 { LCD_SendData (BLACK);}     // если НЕТ (=0) пишем цвет	фона  (16-bit SPI) //BLACK <- back_color);} заменить, если надо выводить конкретный цвет фона

				if(!--q)                      // смотрим сколько действительных бит строки вывели, если уже все (значит конец строки)
					{
						if (j != d){x = x+(d-j);} // то.. проверим, вдруг фактическая ширина меньше начальной больше чем на байт, если так, то пропустим ненужные 
						                          // (пустые байты в массиве) (допустим: фактически точка = 3бит, а на 1 символ отведено 3 байта = 24бит, 2 байт "пустые")
					  y = 0; j = 0; q = en1;    // но в любом случае обнуляем счетчик бит, байт строки и заносим ширину строки
					} 			
        }
     }  
    }		
}

void LCD_DrawPoint(uint16_t ysta, uint16_t xsta, uint16_t color)
{
	LCD_SetCursorPosition(xsta, ysta, xsta, ysta);
	LCD_SendData(color); 
}



void LCD_DrawHLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t color)
{
	unsigned int y;

	LCD_SetCursorPosition(xsta, ysta, xend, ysta);  		
	y =  ((xend-xsta)+1);

	while(y--)
	{
		DAT_ADR= color; 	
	}
}



/*********************************************************************************************************
** Functoin name:       LCD_DrawVLine
*********************************************************************************************************/
void LCD_DrawVLine(uint16_t xsta, uint16_t ysta, uint16_t yend, uint16_t color)
{
	unsigned int y;

	LCD_SetCursorPosition(xsta, ysta, xsta, yend);  		
	y =  ((yend-ysta)+1);
	while(y--)
	{
		DAT_ADR= color; 	
	}
}



void LCD_DrawLine(uint16_t ysta, uint16_t xsta, uint16_t yend, uint16_t xend, uint16_t color)
{
    uint16_t x, y, t;	 
	if((xsta==xend)&&(ysta==yend))
		LCD_DrawPoint(xsta, ysta, color);
	else if(xsta==xend)
	{
		LCD_DrawVLine(xsta,ysta,yend,color);
	}
	else if(ysta==yend)
	{	
		LCD_DrawHLine(xsta,ysta,xend,color);
	}
	else{ 
		if(abs(yend-ysta)>abs(xend-xsta))
		{
			if(ysta>yend) 
			{
				t=ysta;
				ysta=yend;
				yend=t; 
				t=xsta;
				xsta=xend;
				xend=t; 
			}
			for(y=ysta;y<yend;y++)
			{
				x = (uint32_t)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
				LCD_DrawPoint(x, y, color);  
			}
		}
		else
		{
			if(xsta>xend)
			{
				t=ysta;
				ysta=yend;
				yend=t;
				t=xsta;
				xsta=xend;
				xend=t;
			}   
			for(x=xsta;x<=xend;x++)
			{
				y = (uint32_t)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
				LCD_DrawPoint(x, y, color); 
			}
		}
	} 
}

void bright_PWM_ssd1963(uint8_t bright)
{
LCD_SendCommand(0xBE);    // PWM configuration 
LCD_SendData(0x08);     // set PWM signal frequency to 170Hz when PLL frequency is 100MHz 
LCD_SendData(bright);     // PWM duty cycle  
	LCD_SendData(0x01);     //
}




//******************************************************************************
//***       ИНИЦИАЛИЗАЦИЯ ДИСПЛЕЯ
//******************************************************************************
void LCD_ini (void)
	{
	//1. Power up the system platform and assert the RESET# signal (‘L’ state) for a minimum of 100us to reset the controller. 
		TFT_RST_SET
    HAL_Delay (100);
    TFT_RST_RESET
    HAL_Delay (120);
    TFT_RST_SET
    HAL_Delay (120);
	/***************************
	2. Configure SSD1961’s PLL frequency 
VCO = Input clock x (M + 1) 
PLL frequency  = VCO / (N + 1) 
* Note : 
1.  250MHz < VCO < 800MHz 
PLL frequency < 110MHz 
2.  For a 10MHz input clock to obtain 100MHz PLL frequency, user cannot program M = 19 and N = 1.  The 
closet setting in this situation is setting M=29 and N=2, where 10 x 30 / 3 = 100MHz. 
3.  Before PLL is locked, SSD1961/2/3 is operating at input clock frequency (e.g. 10MHz), registers 
programming cannot be set faster than half of the input clock frequency (5M words/s in this example). 
Example to program SSD1961 with M = 29, N = 2, VCO = 10M x 30 = 300 MHz, PLL frequency = 300M / 3 = 100 
MHz 
	******************************/
		

	LCD_SendCommand(0xE2);  //установка частоты
LCD_SendData(0x1D);  // делитель(M=29) 
LCD_SendData(0x02);  //множитель(N=2) 
LCD_SendData(0xFF);  //вкл/выкл множит и дел.

//3. Turn on the PLL 
LCD_SendCommand(0xE0);  
LCD_SendData(0x01); 
 HAL_Delay (120); // Wait for 100us to let the PLL stable and read the PLL lock status bit. 
LCD_SendCommand(0xE0); 
//READ COMMAND “0xE4);   (Bit 2 = 1 if PLL locked) 
LCD_SendData(0x03); // 5. Switch the clock source to PLL 
  HAL_Delay (120);
LCD_SendCommand(0x01); //6. Software Reset
HAL_Delay (120);
/*************
Dot clock Freq = PLL Freq x (LCDC_FPR + 1) / 2(в 20 степени)
For example,  22MHz = 100MHz * (LCDC_FPR+1) / 2 (в 20 степени)
LCDC_FPR = 230685 = 0x3851D 
********************/
LCD_SendCommand(0xE6);  // 7. Configure the dot clock frequency // Настройте частоту синхросигнала
LCD_SendData(0x03); 
LCD_SendData(0x85);  
LCD_SendData(0x1D);  

//8. Configure the LCD panel  
//a. Set the panel size to 480 x 800 and polarity of LSHIFT, LLINE and LFRAME to active low 
LCD_SendCommand(0xB0); 
LCD_SendData(0x0C);   // 18bit panel, disable dithering, LSHIFT: Data latch in rising edge, LLINE and LFRAME: active low 
LCD_SendData(0x00);     // TFT type 
LCD_SendData(0x03);     // Horizontal Width:  480 - 1 = 0x031F 
LCD_SendData(0x1F);  
LCD_SendData(0x01);     // Vertical Width :  800 -1 = 0x01DF
LCD_SendData(0xDF);  
LCD_SendData(0x00);     // 000 = режим RGB

//b. Set the horizontal period 
LCD_SendCommand(0xB4);    // Horizontal Display Period  
LCD_SendData(0x03);    // HT: horizontal total period (display + non-display) – 1 = 520-1 =  519 =0x0207
LCD_SendData(0xA0);      
LCD_SendData(0x00);    // HPS: Horizontal Sync Pulse Start Position = Horizontal Pulse Width + Horizontal Back Porch = 16 = 0x10 
LCD_SendData(0x2E);    
LCD_SendData(0x30);     // HPW: Horizontal Sync Pulse Width - 1=8-1=7 
LCD_SendData(0x00);    // LPS: Horizontal Display Period Start Position = 0x0000 
LCD_SendData(0x0F); 
LCD_SendData(0x00);    // LPSPP: Horizontal Sync Pulse Subpixel Start Position(for serial TFT interface).  Dummy value for TFT interface. 

//c. Set the vertical period 
LCD_SendCommand(0xB6);    // Vertical Display Period  
LCD_SendData(0x02);     // VT: Vertical Total (display + non-display) Period – 1  =647=0x287 
LCD_SendData(0x0D);    
LCD_SendData(0x00);     // VPS: Vertical Sync Pulse Start Position  =     Vertical Pulse Width + Vertical Back Porch = 2+2=4  
LCD_SendData(0x10);    
LCD_SendData(0x10);     //VPW: Vertical Sync Pulse Width – 1 =1 
LCD_SendData(0x08);     //FPS: Vertical Display Period Start Position = 0 
LCD_SendData(0x00);  
/****
9. Set the back light control PWM clock frequency  // установка ШИМ подсветки
PWM signal frequency = PLL clock / (256 * (PWMF[7:0] + 1)) / 256 
********/
LCD_SendCommand(0xBE);    // PWM configuration 
LCD_SendData(0x08);     // set PWM signal frequency to 170Hz when PLL frequency is 100MHz 
LCD_SendData(0xFF);     // PWM duty cycle  (50%) 
LCD_SendData(0x01);     // 0x09 = enable DBC, 0x01 = disable DBC  //включаем


LCD_SendCommand(0x36);     // set address_mode
LCD_SendData(0x02);        // установка ориентации, отражения, RGB/BGR и др.

//13. Setup the MCU interface for 16-bit data write (565 RGB)
LCD_SendCommand(0xF0);     // mcu interface config 
LCD_SendData(0x03);     // 16 bit interface (565)

//10. Turn on the display 
LCD_SendCommand(0x29);     // display on 
}
