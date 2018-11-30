#include <stm32f4xx.h>

#include "delay.h"

void delay_init ( uint8_t a)
{
	// for 168 MHz TIM6
	// Если d=1 то время задержки в микросекундах
	// Если d=10 то время задержки в микросекундах*10
	// Если d=100 то время задержки в микросекундах*100
	// можно задавать только 3 значения 1,10,100

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->PSC = (84*a)-1;
	TIM6->CR1 = TIM_CR1_OPM;
}

void delay ( uint16_t a )
{
	TIM6->ARR=a; 				// до скольки таймер считает
	TIM6->CNT=0; 				// обнуляем текущее значение счётчика
	TIM6->CR1 |= TIM_CR1_CEN;	// включаем счетчик

	while((TIM6->SR & TIM_SR_UIF)==0);	//проверка переполнения счётчика
	TIM6->SR &= ~TIM_SR_UIF; 			//сброс бита переполнения счётчика
	TIM6->CR1 &= ~TIM_CR1_CEN;			//выключаем счётчик
}

