#include "stm32f10x_gpio.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
 
 
 
/*******************************************************************/
// Определяем адреса, по которым будем записывать данные
// Для записи данных
#define LCD_DATA    			    ((uint32_t)0x60020000)    
// Для записи команд
#define LCD_REG   		  	    ((uint32_t)0x60000000)	  
 
 
 
/*******************************************************************/
// Простенькая функция задержки
void delay(uint32_t delayTime)
{	
    uint32_t i;
    for(i = 0; i < delayTime; i++); 
} 
 
 
/*******************************************************************/
 // Так мы будем писать команды в регистры LCD 
void writeLCDCommand(unsigned int reg,unsigned int value) 
{ 
    *(uint16_t *) (LCD_REG) = reg; 
    *(uint16_t *) (LCD_DATA) = value; 
} 
 
 
 
/*******************************************************************/ 
// А так данные.. 
void writeLCDData(unsigned int data) 
{ 
    *(uint16_t *) (LCD_DATA)= data;
 } 
 
 
 
/*******************************************************************/ 
void initAll() 
{ 
    FSMC_NORSRAMInitTypeDef fsmc;
    FSMC_NORSRAMTimingInitTypeDef fsmcTiming; 
    GPIO_InitTypeDef gpio; 
	 // FSMC_NORSRAMTimingInitTypeDef amp; 
 
    // Включаем тактирование портов 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 
    // И тактирование FSMC 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); 
 
    // Инициализация пинов, задейстованных в общении по FSMC 
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15; 
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOD, &gpio); 
 
    gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
    gpio.GPIO_Mode = GPIO_Mode_AF_PP; 
    gpio.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOE, &gpio); 
 
    gpio.GPIO_Mode = GPIO_Mode_Out_PP; 
    gpio.GPIO_Pin = GPIO_Pin_6; 
    GPIO_Init(GPIOD, &gpio);
 
    // Здесь у нас Reset 
    gpio.GPIO_Pin = GPIO_Pin_1 ;
    GPIO_Init(GPIOE, &gpio); 
 
    // CS 
    gpio.GPIO_Mode = GPIO_Mode_AF_PP; 
    gpio.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOD, &gpio); 
 
    // RS 
    gpio.GPIO_Pin = GPIO_Pin_11 ;
    GPIO_Init(GPIOD, &gpio);
 
    // CS -> 1
    // Reset -> 0
    // RD -> 1
    // RW -> 1
 
    GPIO_SetBits(GPIOD, GPIO_Pin_7);			
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);		
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		    
    GPIO_SetBits(GPIOD, GPIO_Pin_5);			
 
    // Настройка FSMC
		
		
		// Настройка FSMC
fsmcTiming.FSMC_AddressSetupTime = 0x02;
fsmcTiming.FSMC_AddressHoldTime = 0x00;
fsmcTiming.FSMC_DataSetupTime = 0x05;
fsmcTiming.FSMC_BusTurnAroundDuration = 0x00;
fsmcTiming.FSMC_CLKDivision = 0x00;
fsmcTiming.FSMC_DataLatency = 0x00;
fsmcTiming.FSMC_AccessMode = FSMC_AccessMode_B;


fsmc.FSMC_Bank = FSMC_Bank1_NORSRAM1;
fsmc.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
fsmc.FSMC_MemoryType = FSMC_MemoryType_NOR;
fsmc.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
fsmc.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
fsmc.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
fsmc.FSMC_WrapMode = FSMC_WrapMode_Disable;
fsmc.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
fsmc.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
fsmc.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
fsmc.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
fsmc.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
fsmc.FSMC_ReadWriteTimingStruct = &fsmcTiming;
fsmc.FSMC_WriteTimingStruct = &fsmcTiming;

FSMC_NORSRAMInit(&fsmc);
FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
//    fsmcTiming.FSMC_AddressSetupTime = 0x02;
//    fsmcTiming.FSMC_AddressHoldTime = 0x00;
//    fsmcTiming.FSMC_DataSetupTime = 0x05;
//    fsmcTiming.FSMC_BusTurnAroundDuration = 0x00;
//    fsmcTiming.FSMC_CLKDivision = 0x00;
//    fsmcTiming.FSMC_DataLatency = 0x00;
//    fsmcTiming.FSMC_AccessMode = FSMC_AccessMode_B;
// 
//    fsmc.FSMC_Bank = FSMC_Bank1_NORSRAM1;
//    fsmc.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
//    fsmc.FSMC_MemoryType = FSMC_MemoryType_NOR;
//    fsmc.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
//    fsmc.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
//    fsmc.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
//    fsmc.FSMC_WrapMode = FSMC_WrapMode_Disable;
//    fsmc.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
//    fsmc.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
//    fsmc.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
//    fsmc.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
//    fsmc.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
//    fsmc.FSMC_ReadWriteTimingStruct = &amp;fsmcTiming;
//    fsmc.FSMC_WriteTimingStruct = &fsmcTiming;	   
// 
//    FSMC_NORSRAMInit(&fsmc); 
//    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);	
}
 
 
 
/*******************************************************************/
void initLCD()
{
    // Глобальный Reset дисплея
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    delay(0x0FFFFF);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
    delay(0x0FFFFF);
 
    // Пляски с  бубном от китайских товарищей	
    writeLCDCommand(0x0000,0x0001);
    delay(10);
 
    writeLCDCommand(0x0015,0x0030);
    writeLCDCommand(0x0011,0x0040);
    writeLCDCommand(0x0010,0x1628);
    writeLCDCommand(0x0012,0x0000);
    writeLCDCommand(0x0013,0x104d);
    delay(10);
    writeLCDCommand(0x0012,0x0010);
    delay(10);
    writeLCDCommand(0x0010,0x2620);
    writeLCDCommand(0x0013,0x344d);
    delay(10);
 
    writeLCDCommand(0x0001,0x0100);
    writeLCDCommand(0x0002,0x0300);
    writeLCDCommand(0x0003,0x1030);
    writeLCDCommand(0x0008,0x0604);
    writeLCDCommand(0x0009,0x0000);
    writeLCDCommand(0x000A,0x0008);
 
    writeLCDCommand(0x0041,0x0002);
    writeLCDCommand(0x0060,0x2700);
    writeLCDCommand(0x0061,0x0001);
    writeLCDCommand(0x0090,0x0182);
    writeLCDCommand(0x0093,0x0001);
    writeLCDCommand(0x00a3,0x0010);
    delay(10);
 
    // Настройки гаммы
    writeLCDCommand(0x30,0x0000);		
    writeLCDCommand(0x31,0x0502);		
    writeLCDCommand(0x32,0x0307);		
    writeLCDCommand(0x33,0x0305);		
    writeLCDCommand(0x34,0x0004);		
    writeLCDCommand(0x35,0x0402);		
    writeLCDCommand(0x36,0x0707);		
    writeLCDCommand(0x37,0x0503);		
    writeLCDCommand(0x38,0x1505);		
    writeLCDCommand(0x39,0x1505);
    delay(10);
 
    // Включение дисплея
    writeLCDCommand(0x0007,0x0001);
    delay(10);
    writeLCDCommand(0x0007,0x0021);
    writeLCDCommand(0x0007,0x0023);
    delay(10);
    writeLCDCommand(0x0007,0x0033);
    delay(10);
    writeLCDCommand(0x0007,0x0133);
}
 
 
 
/*******************************************************************/
int main()
{
    initAll();
    initLCD();	
 
    while(1)
    {
       int i;		
 
        // Начальный и конечный адреса по горизонтали
        writeLCDCommand(0x0050, 0); 
        writeLCDCommand(0x0051, 239); 
        // Начальный и конечный адреса по вертикали
        writeLCDCommand(0x0052, 0); 
        writeLCDCommand(0x0053, 319);  
 
        writeLCDCommand(32, 0);
        writeLCDCommand(33, 0);
        *(uint16_t *) (LCD_REG) = 34;
 
        // Красный
        for (i = 0; i < 76800; i++)
	{
	    writeLCDData(0xF800);
	}
	delay(0x0FFFFF);
	// Зеленый
	for (i = 0; i < 76800; i++)
	{
	    writeLCDData(0x07E0);
	}
	delay(0x0FFFFF);
	//Синий
	for (i = 0; i < 76800; i++)
	{
	    writeLCDData(0x001F);
	}
	delay(0x0FFFFF);
    }
}
 
 
 
/*******************************************************************/
