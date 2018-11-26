/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "tft_lcd.h"
#include "TouchPanel.h" 
#include "DS18B20.h"
//#include "delay.h"
//#include "flag.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_SPI3_Init();

  /* USER CODE BEGIN 2 */
LCD_ini();
LCD_Clear(BLACK); // заливка цветом, RGB_565, некоторые цвета есть в виде дефайнов в файле #include "tft_lcd.h"


	
if (1) // 1 - калибруем, 0- вносим значения в матрицу (значения (Matrix) должны быть заранее внесены Вами, например посмотрев их в Дебаггере)
{
TouchPanel_Calibrate();
}
else
{
	Touch_Cal_Read (&matrix);
}

next_search_ROM (6); // 6 - количество считываний/сравнений ROM, максимум 64 датчика на одной линии
//search_ROM (); 

	   sprintf( array, "Найдено датчиков - %u шт.   ", ds_cnt); //заносим в массив
   WriteString( 1, 5, array , YELLOW);	// пишем строку 1 - номер СТРОКИ (!), 5 - отступ слева в пикселях
	   sprintf( array, "chipmk.ru");
	 WriteString( 0, 610, array , ORANGE );


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		static unsigned char 		   i, m, count=28, count_2=0;
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
if (count++ > 100) // считаем, опрос 1 раз в секунду
{
	count = 0;


		
		for (i = 0; i < ds_cnt; i++) 
		{
	 	 sprintf( array, "t %c%u.%u°C       ", ds1820[i].sign, ds1820[i].termo/10, ds1820[i].termo%10); // %c - знак, %u - десятичное значение
    WriteString( i+3, 5, array , GREEN);	 
		}		
		
		m = i;
		for (i = 0; i < ds_cnt; i++)
		{		
	   sprintf( array, "ROM %u - го датчика  %u %u %u %u %u %u %u %u"   , i + 1 ,
			ds1820[i].rom[0], ds1820[i].rom[1], ds1820[i].rom[2], ds1820[i].rom[3], 
			ds1820[i].rom[4], ds1820[i].rom[5], ds1820[i].rom[6], ds1820[i].rom[7]);
   WriteString( m+i+4, 5, array , BLUE);	
		}
	}
else if (count == 30)
{
	 CONV_TRMO ();			// конвертируем температуру
}
else if (count == 70)
{
	 DS1820_Task(); // получаем результат
}



// рисуем на экране
		getDisplayPoint(&display, Read_Ads7846(), &matrix ); // опрашиваем ТАЧ, результат лежит в реальных коорд. дисплея в display.x и display.у
	  LCD_DrawPoint (display.x, display.y, WHITE); // ставим точку

    if (count_2++ > 5) // счетчик, чтоб цифры не мелькали ;)
    {
     count_2 = 0;
		 sprintf( array, "Touch.X - %u      ",  display.x);
		 WriteString( 18, 10, array , RED );
		 sprintf( array, "Touch.Y - %u     ",display.y);
		 WriteString( 19, 10, array , RED );
			}
		if (((count_2 == 0)||(count_2 == 2)||(count_2 == 4))&&(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1))// опрос кнопки
		{
					static unsigned char br = 128, flg=1;
					
					if (flg == 0)
					{
						br++; //яркость больше
					}
					else
					{
						br--; //..меньше
					}
					 if ((br == 1)||(br == 255))
					{
						flg = !flg;
					}
					bright_PWM_ssd1963(br); // отправляем команду яркости
							 sprintf( array, "Уровень яркости %u     ", br);
		           WriteString( 21, 10, array , MAGENTA );				
		}

HAL_Delay (10); //пауза 10 микросекунд (0,01сек)
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
