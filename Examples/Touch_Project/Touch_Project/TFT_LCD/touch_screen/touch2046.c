
#include "touch2046.h"


//#define SPI_PORT	SPI1

#define TOUCH_CS_PORT	 GPIOB
#define TOUCH_CS_PIN	 GPIO_Pin_7

//  TS_IRQ -  PD9

// TS_CS - PD.4
#define T_CS()   GPIO_ResetBits ( TOUCH_CS_PORT, TOUCH_CS_PIN );
#define T_DCS()  GPIO_SetBits ( TOUCH_CS_PORT, TOUCH_CS_PIN );

static uint16_t SPI_Write ( uint16_t cmd );
static void SpiDelay ( uint16_t DelayCnt );

static uint16_t TPReadX ( void );
static uint16_t TPReadY ( void );

static bool TouchVerifyCoef ( void );

// было касание или нет
bool isTouch ( void );

// Корректирующие коэффициенты
static int diffX, diffY;

// Размеры дисплея
static uint16_t dispSizeX, dispSizeY;

#define LCD_PIXEL_WIDTH 240
#define LCD_PIXEL_HEIGHT 320

// variables
extern char sBuffer[256];
static int16_t axc[2], ayc[2], bxc[2], byc[2];
static int16_t ax, bx, ay, by;

static uint16_t SPI_Write ( uint16_t cmd )
{
	uint16_t val, MSB, LSB;

	SpiDelay ( 10 );
	SPI_I2S_SendData ( SPI1, cmd );

	// Wait to receive a byte
	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_RXNE ) == RESET ) { ; }
	MSB = SPI_I2S_ReceiveData ( SPI1 );
	SPI_I2S_SendData ( SPI1, 0 );
	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_RXNE ) == RESET ) { ; }
	// Return the byte read from the SPI bus
	LSB = SPI_I2S_ReceiveData ( SPI1 );
	val = ( ((MSB<<4) & 0x0FF0) | ((LSB>>12) & 0x000F) )<<1;

	SpiDelay ( 10 );
	return val;

/*
	while ( !(SPI1 -> SR & SPI_SR_TXE) );  //устанавливается при опустошении буфера передатчика
	SPI1 -> DR = cmd; //записываем данные в SPI
	while ( !(SPI1 -> SR & SPI_SR_RXNE) ); //устанавливается когда буфер приемника содержит данные, то есть ждём когда закончится приём данных.
	return SPI1 -> DR; //возвращаем полученные данные из SPI
*/
} // SPI_Write

void TouchInit ( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef  SPI_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  // SPI pins - PC10 .. PC12
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init ( GPIOA, &GPIO_InitStruct );

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( GPIOA, &GPIO_InitStruct );
   // SPI1 configuration
  RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SPI1, ENABLE );

  // настройка SPI
/*
  SPI1->CR1 &= ~SPI_CR1_SPE;    // выключаем SPI модуль
  RCC->APB1ENR |= RCC_APB1ENR_SPI1EN;    // включение тактирования SPI1 (PC10-12) модуля

  // 0000.x011.y0zz.z1fg x(0-8бит,1-16бит);y(0-LSB,1-MSB);z(делитель 0-2,1-4...7-256)
  SPI1->CR1 = 0x033C; //задаём конфигурацию = 0000 0011 0011 1100
  SPI1->I2SCFGR &= ~0x0800; // сбрасываем 11 бит отвечающий за режим:  0-SPI mode is selected, 1-I2S mode is selected
  SPI1->CR1 |= SPI_CR1_SPE; // включаем SPI модуль
*/
  SPI_Cmd ( SPI1, DISABLE );
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init ( SPI1, &SPI_InitStructure );
  SPI_CalculateCRC ( SPI1, DISABLE );
  SPI_Cmd ( SPI1, ENABLE );
  SPI_SSOutputCmd ( SPI1, DISABLE );
  SPI_NSSInternalSoftwareConfig ( SPI1, SPI_NSSInternalSoft_Set );

  // CS - PD4 - OUT
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Pin = TOUCH_CS_PIN;
  GPIO_Init ( TOUCH_CS_PORT, &GPIO_InitStruct );
  T_DCS();

  // T_PEN - TS_IRQ -  PD9
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init ( GPIOB, &GPIO_InitStruct );

  diffX = diffY = 0;
  dispSizeX = dispSizeY = 0;
} // TouchInit

static void SpiDelay ( uint16_t DelayCnt )
{
	uint16_t i;
	for ( i = 0; i < DelayCnt; i ++ );
} // SpiDelay

static uint16_t TPReadX ( void)
{
	uint16_t x;

	T_CS ( );
	x = SPI_Write ( 0x9000 );		// 10010100 - Read Y, 12bit mode
									//  001       - A2..A0 - X+
									//     0      - 12 bit
									//      1     = SER/nDFR = 1
	T_DCS ( );

	return x;
} // TPReadX

static uint16_t TPReadY ( void )
{
	uint16_t y;

	T_CS ( );
	y = SPI_Write ( 0xD000 );			// 11010100 - Read Y, 12bit mode
										//  101       - A2..A0 - X+
										//     0      - 12 bit
										//      1     = SER/nDFR = 1
	T_DCS ( );

	return y;
} // TPReadY

bool TouchReadXY ( uint16_t *px, uint16_t *py, bool isReadCorrected )
{
	bool flag;
//	uint16_t t, t1, temp;
	// 0 - x, 1 - y
//	uint16_t databuffer[2][10];
//	int i, count = 10;
	uint16_t x, y;

	flag = isTouch ( );
	if ( flag )
	{ // касание обнаружено
		y = 4095 - TPReadX ( );
		x = TPReadY ( );
		if ( isReadCorrected && !TouchVerifyCoef ( ) )
		{
			*px = ( x / ax ) + bx;
			*py = ( y / ay ) + by;
		} // if
		else
		{ // без коррекции
			*px = x;
			*py = y;
		} // else
	} // if (касание)

	return flag;
} // TouchReadXY

// Задание размеров дисплея
void TouchSetScreenSize ( uint16_t sizeX, uint16_t sizeY )
{
	dispSizeX = sizeX;
	dispSizeY = sizeY;
} // TouchSetScreenSize

// было касание или нет (TS_IRQ -  PD9)
bool isTouch ( void )
{
    if ( GPIO_ReadInputDataBit ( GPIOB, GPIO_Pin_6 ) == RESET )
    	return true;	// касание было

	return false;
} // isTouch

/////////////////////////////
// Калибровка

// Задать калибровочные коэффициенты
void TouchSetCoef ( int16_t _ax, int16_t _bx, int16_t _ay, int16_t _by )
{
	ax = _ax;
	bx = _bx;
	ay = _ay;
	by = _by;
} // TouchSetCoef

static bool TouchVerifyCoef ( void )
{
	if ( ax == 0 || ax == 0xFFFF || bx == 0xFFFF || ay == 0 || ay == 0xFFFF || by == 0xFFFF )
		return true;

	return false;
} // TouchVerifyCoef

static const int16_t xCenter[] = { 35, LCD_PIXEL_WIDTH-35, 35, LCD_PIXEL_WIDTH-35 };
static const int16_t yCenter[] = { 35, 35, LCD_PIXEL_HEIGHT-35, LCD_PIXEL_HEIGHT-35 };
static int16_t xPos[5], yPos[5];

// Калибровка
void TouchCalibrate ( void )
{
	uint16_t x, y;

	// Если калибровочные коэффициенты уже установлены - выход
	if ( !TouchVerifyCoef ( ) )
		return;

	// left top corner draw
	LCD_FillScr ( 0xFFFF );
	LCD_DrawString ( "Калибровка", 50, 100, 0xFFE0, 0x0000, 1 );
	LCD_drawLine ( 10, 10+25, 10+50, 10+25, 0x0000 );
	LCD_drawLine ( 10+25, 10, 10+25, 10+50, 0x0000 );

	LCD_DrawString ( "Нажимайте ", 50, 120, 0xFFE0, 0x0000, 1 );
	while (1)
	{
		// ждать нажатия
		while ( !isTouch ( ) );
		TouchReadXY ( &x, &y, false );
		if (x < 4090 && y < 4090)
		{
			xPos[0] = x;
			yPos[0] = y;
			break;
		} // if
	} // while
	LCD_DrawString ( "Отпускайте", 50, 120, 0xFFE0, 0x0000, 1 );

	// ждать отпускания
	while ( isTouch ( ) );
	LCD_DrawString ( "          ", 50, 120, 0xFFE0, 0x0000, 1 );


	// right top corner draw
	LCD_FillScr ( 0xFFFF );
	LCD_DrawString ( "Калибровка", 50, 100, 0xFFE0, 0x0000, 1 );
	LCD_drawLine ( LCD_PIXEL_WIDTH-10-50, 10+25, LCD_PIXEL_WIDTH-10-50+50, 10+25, 0x0000 );
	LCD_drawLine ( LCD_PIXEL_WIDTH-10-25, 10, LCD_PIXEL_WIDTH-10-25, 10+50, 0x0000 );

	LCD_DrawString ( "Нажимайте ", 50, 120, 0xFFE0, 0x0000, 1 );
	while (1)
	{
		// ждать нажатия
		while ( !isTouch ( ) );
		TouchReadXY ( &x, &y, false );
		if (x < 4090 && y < 4090)
		{
			xPos[1] = x;
			yPos[1] = y;
			break;
		} // if
	} // while
	LCD_DrawString ( "Отпускайте", 50, 120, 0xFFE0, 0x0000, 1 );

	// ждать отпускания
	while ( isTouch ( ) );
	LCD_DrawString ( "          ", 50, 120, 0xFFE0, 0x0000, 1 );

	// left down corner draw
	LCD_FillScr ( 0xFFFF );
	LCD_DrawString ( "Калибровка", 50, 110, 0xFFE0, 0x0000, 1 );
	LCD_drawLine ( 10, LCD_PIXEL_HEIGHT-10-25, 10+50, LCD_PIXEL_HEIGHT-10-25, 0x0000 );	// hor
	LCD_drawLine ( 10+25, LCD_PIXEL_HEIGHT-10-50, 10+25, LCD_PIXEL_HEIGHT-10-50+50, 0x0000 );	// vert

	LCD_DrawString ( "Нажимайте ", 50, 120, 0xFFE0, 0x0000, 1 );
	while (1)
	{
		// ждать нажатия
		while ( !isTouch ( ) );
		TouchReadXY ( &x, &y, false );
		if (x < 4090 && y < 4090)
		{
			xPos[2] = x;
			yPos[2] = y;
			break;
		} // if
	} // while
	LCD_DrawString ( "Отпускайте", 50, 120, 0xFFE0, 0x0000, 1 );

	// ждать отпускания
	while ( isTouch ( ) );
	LCD_DrawString ( "          ", 50, 120, 0xFFE0, 0x0000, 1 );

	// Правый нижний
	LCD_FillScr ( 0xFFFF );
	LCD_DrawString ( "Калибровка", 50, 110, 0xFFE0, 0x0000, 1 );
	LCD_drawLine ( LCD_PIXEL_WIDTH-10-50, LCD_PIXEL_HEIGHT-10-25, LCD_PIXEL_WIDTH-10-50+50, LCD_PIXEL_HEIGHT-10-25, 0x0000 );	// hor
	LCD_drawLine ( LCD_PIXEL_WIDTH-10-25, LCD_PIXEL_HEIGHT-10-50, LCD_PIXEL_WIDTH-10-25, LCD_PIXEL_HEIGHT-10-50+50, 0x0000 );	// vert

	LCD_DrawString ( "Нажимайте ", 50, 120, 0xFFE0, 0x0000, 1 );
	while (1)
	{
		// ждать нажатия
		while ( !isTouch ( ) );
		TouchReadXY ( &x, &y, false );
		if (x < 4090 && y < 4090)
		{
			xPos[3] = x;
			yPos[3] = y;
			break;
		} // if
	} // while
	LCD_DrawString ( "Отпускайте", 50, 120, 0xFFE0, 0x0000, 1 );

	// ждать отпускания
	while ( isTouch ( ) );
	LCD_DrawString ( "          ", 50, 120, 0xFFE0, 0x0000, 1 );

	// Расчёт коэффициентов
	axc[0] = (xPos[3] - xPos[0])/(xCenter[3] - xCenter[0]);
	bxc[0] = xCenter[0] - xPos[0]/axc[0];
	ayc[0] = (yPos[3] - yPos[0])/(yCenter[3] - yCenter[0]);
	byc[0] = yCenter[0] - yPos[0]/ayc[0];

	axc[1] = (xPos[2] - xPos[1])/(xCenter[2] - xCenter[1]);
	bxc[1] = xCenter[1] - xPos[1]/axc[1];
	ayc[1] = (yPos[2] - yPos[1])/(yCenter[2] - yCenter[1]);
	byc[1] = yCenter[1] - yPos[1]/ayc[1];

	// Сохранить коэффициенты
	TouchSetCoef ( axc[0], bxc[0], ayc[0], byc[0] );

	LCD_FillScr ( 0xFFFF );
	LCD_DrawString ( "Калибровка завершена", 20, 110, 0xFFE0, 0x0000, 1 );
	delay( 1000 );	// 1 sec
} // TouchCalibrate

/*
void touch_XY(unsigned int *resx,unsigned int *resy)
{
	uint16_t mas_x[6];
	uint16_t mas_y[6];
	uint16_t maxy,miny,maxx,minx,indxmax,indymax,indymin,indxmin;
	float rezx,rezy;

for(int i =0;i<6;i++)
{
	TouchReadXY(mas_x[i],mas_y[i],true);

}

maxx=mas_x[0];
maxy=mas_y[0];
	minx=mas_x[0];
	miny=mas_x[0];
for(int i=0;i<6;i++)
{
	if(mas_x[i]>maxx){ maxx= mas_x[i]; indxmax=i;}
	if(mas_y[i]>maxy){ maxy= mas_y[i]; indymax=i;}
	if(mas_x[i]<minx){ minx= mas_x[i]; indxmin=i;}
	if(mas_y[i]<miny){ miny= mas_y[i]; indymin=i;}
}
for(int i =0;i<6;i++)
{
	if (i==indxmax){} else {
		if(i==indxmin){} else {
			rezx+=mas_x[i];
		}
		}
	if (i==indymax){} else {
			if(i==indymin){} else {
				rezy+=mas_y[i];
			}
			}

}
*resx=(unsigned int)round(rezx/4);
*resy=(unsigned int)round(rezy/4);
}
*/
