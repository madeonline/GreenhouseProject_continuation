#include "xpt2046_spi.h"

//ф-ция инициализарует SPI
void SPI_Touch_Config (void)
{
	//Включаем тактирование портов A, B и альтернативных функций
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;   
	//распиновка
	//PA5 - SCL  -  Alternative func. push-pull - OUT 
	//PB7 - CS   -  GPIO - soft - OUT      
	//PA7 - MOSI -  Alternative func.  push-pull - OUT  
	//PA6 - MISO -  Input floating / Input pull-up - IN
	//PB6 - Interrupt - GPIO - IN
	//настраиваем порт А, 5 и 7 вывод - альтернативная функция, 6 - не трогаем 
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF7_0); 	
	GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1); 
	GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_MODE7_1);	

	//настраиваем порт B, 7 вывод - двухтактный выход
	GPIOB->CRL &= ~(GPIO_CRL_CNF7_0 | GPIO_CRL_CNF6_0);  
	GPIOB->CRL |= GPIO_CRL_MODE7_1;
	//6 - вход с подтяжкой к плюсу
	GPIOB->CRL |= GPIO_CRL_CNF6_1;
	GPIOB->ODR |= GPIO_ODR_ODR6;	
		
	//конфигурируем SPI1 
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;     //Enable oscil SPI1
	SPI1->CR1 |= SPI_CR1_BR;                //Baud rate = Fpclk/256
	SPI1->CR1 &= ~SPI_CR1_CPOL;             //Polarity cls signal CPOL = 0;
	SPI1->CR1 &= ~SPI_CR1_CPHA;             //Phase cls signal    CPHA = 0;
	SPI1->CR1 &= ~SPI_CR1_DFF;              //8 bit data
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;         //MSB will be first
	SPI1->CR1 |= SPI_CR1_SSM;               //Program mode NSS
	SPI1->CR1 |= SPI_CR1_SSI;               //анналогично состоянию, когда NSS 1
	SPI1->CR1 |= SPI_CR1_MSTR;              //Mode Master
	SPI1->CR1 |= SPI_CR1_SPE;               //Enable SPI1
	
	//--------------------------
	SPI1->CR2 = 0x00000000;  		//No interrupt enable
}


void Init_XPT2046()
{	
	Spi_Master_Transmit(0X80);
	Spi_Master_Transmit(0X00);
	Spi_Master_Transmit(0X00);
	delay_us(1000);
}
///////////////////////////////////
uint8_t Spi_Master_Transmit(uint8_t out_data)
{
	int d_in = 0;
	SPI1->DR = out_data;
	while(!(SPI1->SR & SPI_SR_RXNE));       
	d_in = SPI1->DR;
	return d_in;

}
/////////////////////////////////

uint16_t Get_Touch(uint8_t adress)
{
	uint16_t data = 0;
	CS_TOUCH_LOW;//активируем XPT2046
	
		//отправляем запрос для получения интересющей нас координаты 
		Spi_Master_Transmit(adress);
		
	  //считываем старший байт 
		Spi_Master_Transmit(0X00);
		data = SPI1->DR;
		data <<= 8;
		
		//считываем младший байт 
		Spi_Master_Transmit(0X00);
		data |= SPI1->DR;
		data >>= 3;
	
	CS_TOUCH_HIGH;//деактивируем XPT2046
	
	return data;
}

///////////////////////////////
void Get_Touch_XY( volatile uint16_t *x_kor,volatile uint16_t *y_kor, uint8_t count_read, uint8_t calibration_flag)
{
	uint8_t i = 0;
	uint16_t tmpx, tmpy, touch_x, touch_y = 0;

	touch_x = Get_Touch(X);//считываем координату Х
	delay_us(100);
	touch_y = Get_Touch(Y);//считываем координату Y
		
	//считываем координаты опр. кол-во раз и каждый раз находим среднее значение
	//если одна из координат равна нулю второе значение тоже обнуляем и не учитываем его
	for( i = 0; i < count_read; i++)
	{	
		tmpx = Get_Touch(X);
		delay_us(100);
		tmpy = Get_Touch(Y);
		
		if (tmpx == 0) tmpy = 0;
    else if (tmpy == 0) tmpx = 0;
    else 
    {
			touch_x = (touch_x + tmpx) / 2;
			touch_y = (touch_y + tmpy) / 2;
    }
      
	}

	//во время калибровки возращаем вычисленные выше значения, 
	//иначе производим расчёт используя коэф. полученные при калибровке
	if (!calibration_flag)
	{
		*x_kor = touch_x/ax + bx;
		*y_kor = touch_y/ay + by;
	} 
	else
	{
		*x_kor = touch_x;
		*y_kor = touch_y; 
	}	
}

