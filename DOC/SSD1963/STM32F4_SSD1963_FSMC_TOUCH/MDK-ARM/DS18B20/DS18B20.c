/*****************************************************************************************/
#include "DS18B20.h"
//#include "Init_Perif.h"
#include "stm32f4xx_hal.h"
#include "tft_lcd.h"
#include  <stdbool.h>
//#include "BMP180.h"
#include <string.h>

/*****************************************************************************************/
static bool ds1820_bit(bool out_val);
/*****************************************************************************************/

 unsigned char zn_ds;
 signed int TERMO_DS;
 char ROM_NO [64];

unsigned char			  ds_cnt, ds_cnt_next=0, srch_count=0;			// Кол-во найденых датчиков

_ds1820 ds1820[64];
/****************************************************************************************/

void __delay_us(unsigned int a)
{
	a = ((a * _XTAL_FREQ_us) / 1000)-10;

	while(a--);	
}



void next_search_ROM (unsigned char set)
{
	 if (srch_count < set )
	  {
			ds_cnt = 0;
		  HAL_Delay (3);
			search_ROM ();
			srch_count++;
		  if (ds_cnt_next < ds_cnt)
		   {
			   ds_cnt_next = ds_cnt;
		   }
		  if (ds_cnt_next > ds_cnt)
		   {
				 srch_count--;
		   } 
		 }
	
}


/****************************************************************************************/

//****************************************************************************************/
//
//  передача команды начала получения - конвертации температуры
//
//****************************************************************************************/
void CONV_TRMO (void)
{
	volatile unsigned char		i;
		i = 200;
		while((!ds1820_presence()) && (i--)){	HAL_Delay (10);}
		ds1820_write(0xcc);					// Обращаемся ко всем девайсам
		ds1820_write(0x44); 				// Команда преобразования температуры


}
//****************************************************************************************/
//
//  выбор датчика и запрос температуры
//
//****************************************************************************************/
void DS1820_Task(void){
	
	static unsigned char 		   i, a; //state=0,
  volatile unsigned char	  crc = 0;// z = 255;
	char		arr[9];
	bool	 end_fl = 0;		// Флаг окончания преобразования температур
			while(!DS_GET_PORT()) {};
	    //end_fl = 0;
			//z=250;
		while (end_fl == 0)//&& (z--))
		{	

    	for(a=0;a<ds_cnt;a++){				// Перебираем все датчики
		    __delay_us(100);					//
		    if(!ds1820_presence()){	break;} //state=0; 
			//--- Адресуем устройство ------//
			ds1820_write(0x55);
			for (i=0;i<8;i++)	ds1820_write(ds1820[a].rom[i]);
			//------------------------------//
			ds1820_write(0xbe);				// Команда чтения температуры
		    for(i=0;i<9;i++){				// Читаем 9 байт с линии
		        arr[i]=ds1820_read();		//
		    }								//
				
		    crc=ds1820_crc(arr, 9);			// Вычисляем контрольную сумму. 0 - crc совпало.
				if (crc){a--;}

		    ds1820[a].termo = *(short int*)&arr[0];	// Укладываем 2 первых байта в переменную temp
		    ds1820[a].termo *= 10;				// Фиксированная точка с 1 знаком.
		    ds1820[a].termo/= 16;
					if ((ds1820[a].termo)< 0)
				{ 
				  ds1820[a].termo = -ds1820[a].termo;
			    ds1820[a].sign	= '-';
				}
				else
				{
					ds1820[a].sign	= ' ';
				}
			}
		end_fl=1;
		}
}

/****************************************************************************************/

void Write_config_ds1820 (void)
{
		volatile unsigned char		i;
		i = 200;
		while((!ds1820_presence()) && (i--)){	HAL_Delay (10);}
	ds1820_write(0xCC);					// Обращаемся ко всем девайсам
	ds1820_write(0x4E);
	ds1820_write(0xFF);
	ds1820_write(0xFF);
	ds1820_write(0xFF);
}

/****************************************************************************************/

void search_ROM (void)
{
 HAL_GPIO_WritePin(DS1820_PORT, DS1820_PIN_NUM, GPIO_PIN_RESET);
 DS_PORT_1();
 if (SET_Write_Conf_DS)
 {Write_config_ds1820 ();}

//------------------------
if(ds1820_first_search(ds1820[0].rom)){
	 while((++ds_cnt<200) && ds1820_next_search(ds1820[ds_cnt].rom));
}

//------------------------
if(ds_cnt==0)
	{
	
			  sprintf( array,"Датчики не найдены"); 
	 WriteString( 11, 5, array , WHITE);
	HAL_Delay (1500);
		return;
	}
//------------------------
}

/*****************************************************************************************/
bool ds1820_presence(void){
	HAL_GPIO_WritePin(DS1820_PORT, DS1820_PIN_NUM, GPIO_PIN_RESET);
	DS_PORT_0();
	__delay_us(500);
	IE(0);
	DS_PORT_1();
	__delay_us(2);
	if(!DS_GET_PORT())  {IE(1); return 0;}
	__delay_us(100);
	if(DS_GET_PORT())   {IE(1); return 0;}
	IE(1);
	__delay_us(200);
	if(!DS_GET_PORT())  {return 0;}
	return 1;
}
/*****************************************************************************************/
bool ds1820_bit(bool out_val){
static bool ret;
IE(0);
    ret = 0;
    DS_PORT_0();
    __delay_us(5);
    if(out_val)	{DS_PORT_1();}
    else		{DS_PORT_0();}
    __delay_us(5);
    if(DS_GET_PORT())	{ret = 1;}
    __delay_us(60);
    DS_PORT_1();
IE(1);
return ret;
}
/*****************************************************************************************/
unsigned char ds1820_write(unsigned char out_val){
unsigned char ret=0, i;
    for(i=0;i<8;i++){
        ret = ret>>1;
        if(ds1820_bit(out_val & 1))	ret|=0x80;
        out_val = out_val>>1;
    }
return ret;
}
/*****************************************************************************************/
flash_t char crcval[256]={0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};
//--------------------------------------------------------------------------------//
unsigned char ds1820_crc(char *arr, unsigned char len){
unsigned char ret=0;
    do{
        ret=crcval[ret^*arr++];
    }while(--len);
return ret;
}
/****************************************************************************************/
short int LastDiscrepancy;
short int LastFamilyDiscrepancy;
short int LastDeviceFlag;
extern bool OWSearch(char *ROM_BUF);

bool ds1820_first_search(char *ROM_BUF){
   LastDiscrepancy = 0;
   LastDeviceFlag = 0;
   LastFamilyDiscrepancy = 0;
return OWSearch(ROM_BUF);
}
/*****************************************************************************************/
bool ds1820_next_search(char *ROM_BUF){
return OWSearch(ROM_BUF);
}
/*****************************************************************************************/
bool OWSearch(char *ROM_BUF){
short int id_bit_number;
short int last_zero, rom_byte_number;
static bool id_bit, cmp_id_bit, search_result;
unsigned char rom_byte_mask, search_direction;

   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;

   if (!LastDeviceFlag){
      if (!ds1820_presence()){
         LastDiscrepancy = 0;		// Последнее несоответствие
         LastDeviceFlag = 0;		// флаг последнего (найденного) устройства
         LastFamilyDiscrepancy = 0;
         return 0;
      }

      ds1820_write(0xF0);

      do{
         id_bit = ds1820_bit(1);																		// читаем первый ответ
         cmp_id_bit = ds1820_bit(1);																// читаем второй ответ (инверсию первого бита)

         if ((id_bit == 1) && (cmp_id_bit == 1))            break;  // если оба чтения =1 датчиков нет
         else{   
             if (id_bit != cmp_id_bit)					 
							{search_direction = id_bit;}														// записываем первый бит (он уже известен) может быть как 0 так и 1
             else
							{																		// бит А = 0 и бит В = 0, значит где-то есть первый бит =0, (далее запишем его в ROM)
                if (id_bit_number < LastDiscrepancy)												// счётчик выбора бита < Последнее несоответствие
							   {
									 search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0); // какой бит (1 ? 0)был последним в прошлом поиске
							   }							 
                else
							   {
									 search_direction = (id_bit_number == LastDiscrepancy);// счётчик выбора бита = Последнее несоответствие, здесь мы вписуем 1, т.к. в прошлый проход мы деактивировали ДС-ки нулевыым битом 
								 }
								 
                if (search_direction == 0) 
								 {
                   last_zero = id_bit_number;   // запоминаем номер бита последнего несоответствия
                 }
              }
             
							if (search_direction == 1)      // смотрим какой бит пришёл и вписуем его в массив результата        
						   {							
					      	ROM_BUF[rom_byte_number] |= rom_byte_mask;  // бит = 1
					     }
              else
						   {
				  	      ROM_BUF[rom_byte_number] &= ~rom_byte_mask; // бит = 0
						   }

              ds1820_bit(search_direction);		// возвращаем на шину этот-же бит	
              id_bit_number++;						    // увеличиваем счётчик выбора бит 
							 
              if (!(rom_byte_mask <<= 1))     // и сдвигаем один бит влево // если прошли все биты маски , то
						   {  				    
                  rom_byte_number++;					// увеличиваем номер байта массива
                  rom_byte_mask = 1;					// и ставим 1-й бит маски на "старт"
               }

            }
        }while(rom_byte_number < 8);				 // проходим все восем байт  (восемь кругов)

      if (id_bit_number > 64)
			 {
				 
         search_result = 1;		   // и "положительный результат"	 
				 strcpy (ROM_NO, ROM_BUF);
         LastDiscrepancy = last_zero;
         if (!LastDiscrepancy) // если нет несоответствий, то мы нашли все устройства
				  {LastDeviceFlag = 1;}	   // флаг, что все устройства
       }
   }
   if (!search_result || !ROM_BUF[0])
		{
      LastDiscrepancy = 0;
      LastDeviceFlag = 0;
      LastFamilyDiscrepancy = 0;
      search_result = 0;
    }
return search_result;
}


 GPIO_InitTypeDef GPIO_InitStruct_DS;
void	DS_PORT_0(void)
{ GPIO_InitStruct_DS.Pin = DS1820_PIN_NUM;
	GPIO_InitStruct_DS.Mode = GPIO_MODE_OUTPUT_OD;
	HAL_GPIO_Init(DS1820_PORT, &GPIO_InitStruct_DS);
}

void	DS_PORT_1(void)		
{
	GPIO_InitStruct_DS.Pin = DS1820_PIN_NUM; 
	GPIO_InitStruct_DS.Mode = GPIO_MODE_INPUT; 
	HAL_GPIO_Init(DS1820_PORT, &GPIO_InitStruct_DS);
}

/****************************************** конец текста ***********************************************/
