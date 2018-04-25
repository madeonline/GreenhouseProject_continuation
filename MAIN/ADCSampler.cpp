
#include "ADCSampler.h"
#include "CONFIG.h"

ADCSampler::ADCSampler()
{
  dataReady = false;
  adcDMAIndex = 0;
  adcTransferIndex = 0;
  for (int i = 0; i < NUMBER_OF_BUFFERS; i++)
  {
    memset((void *)adcBuffer[i], 0, BUFFER_SIZE);
  }
}
void ADCSampler::begin(unsigned int samplingRate)
{
  this->sampleingRate = sampleingRate;
  // Turning devices Timer on.
  pmc_enable_periph_clk(ID_TC0); 


  // Configure timer
  TC_Configure(TC0, 0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_CLEAR | TC_CMR_TCCLKS_TIMER_CLOCK1);

  // It is good to have the timer 0 on PIN2, good for Debugging 
  //int result = PIO_Configure( PIOB, PIO_PERIPH_B, PIO_PB25B_TIOA0, PIO_DEFAULT);

  // Configure ADC pin A7
  //  the below code is taken from adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);

  ADC->ADC_CR = ADC_CR_SWRST;                            // Reset the controller.
  ADC->ADC_MR = 0;                                       // Reset Mode Register.
  ADC->ADC_PTCR = (ADC_PTCR_RXTDIS | ADC_PTCR_TXTDIS);   // Reset PDC transfer.
  ADC->ADC_MR |= ADC_MR_PRESCAL(3);                      // ADC clock = MSCK/((PRESCAL+1)*2), 13 -> 750000 Sps
  ADC->ADC_MR |= ADC_MR_STARTUP_SUT0;                    // What is this by the way?
  ADC->ADC_MR |= ADC_MR_TRACKTIM(15);
  ADC->ADC_MR |= ADC_MR_TRANSFER(1);
  ADC->ADC_MR |= ADC_MR_TRGEN_EN;                         // Hardware trigger selected by TRGSEL field is enabled. Включен аппаратный триггер, выбранный по полю TRGSEL.
  ADC->ADC_MR |= ADC_MR_TRGSEL_ADC_TRIG1;                 // selecting TIOA0 as trigger.
  ADC->ADC_MR |= ADC_MR_LOWRES_BITS_12;                   // brief (ADC_MR) 12-bit resolution 
  //ADC->ADC_ACR |= ADC_ACR_TSON;                         // Включить датчик температуры    
 
  ADC->ADC_CHER = ADC_CHANNELS;                           // Записать контролируемые входа
  ADC->ADC_CHDR = ADC_CHANNELS_DIS;                       // Отключить не используемые входа
  ADC->ADC_EMR = ADC_EMR_CMPMODE_IN                       // Генерирует событие, когда преобразованные данные пересекают окно сравнения.
	//  | ADC_EMR_CMPSEL(4)                               // Compare channel 4 = A3
	  | ADC_EMR_CMPALL                                    // Compare ALL channel
	  | ADC_EMR_CMPFILTER(0);                             // Количество последовательных событий сравнения, необходимых для повышения флага = CMPFILTER + 1
														  // При запрограммированном значении 0 флаг увеличивается, как только происходит событие.

  ADC->ADC_CWR = ADC_CWR_LOWTHRES(_compare_Low) | ADC_CWR_HIGHTHRES(_compare_High); // Установить высокий и низкий порог компаратора АЦП

  //ADC->ADC_SEQR1 = 0x01234567;                            // использовать A0 до A7 в порядке в массив
  //ADC->ADC_SEQR2 = 0x00dcba00;                            // использовать для А8 А11 следующие действия по порядку в массив  

  /* Interupts */
  ADC->ADC_IDR = ~ADC_IDR_ENDRX;                        // сбросить регистры прерывания по готовности данных.
  ADC->ADC_IDR = ~ADC_IDR_COMPE;                        // сбросить регистры копаратора.
  ADC->ADC_IER =  ADC_IER_ENDRX;                        // Включить прерывание по готовности данных.
 // ADC->ADC_IER =  ADC_IER_COMPE;                        // Прерывание по совпадению сравнения компаратором
  ADC->ADC_ISR = ~ADC_ISR_COMPE;                        // ADC Interrupt Status Register Обнулить ошибку сравнения с момента последнего чтения ADC_ISR.
  /* Waiting for ENDRX as end of the transfer is set
    when the current DMA transfer is done (RCR = 0), i.e. it doesn't include the
    next DMA transfer.

    If we trigger on RXBUFF This flag is set if there is no more DMA transfer in
    progress (RCR = RNCR = 0). Hence we may miss samples.

	Ожидание окончания ENDRX в конце передачи
	когда выполняется текущая передача DMA (RCR = 0), то есть она не включает следующая передача DMA.
    Если мы запускаем RXBUFF, этот флаг устанавливается, если больше нет передачи DMA в прогресс (RCR = RNCR = 0). 
	Следовательно, мы можем пропустить образцы.
  */

  
  unsigned int cycles = 42000000 / samplingRate;

  /*  timing of ADC */
  TC_SetRC(TC0, 0, cycles);                             // TIOA0 goes HIGH on RC.
  TC_SetRA(TC0, 0, cycles / 2);                         // TIOA0 goes LOW  on RA.

  // We have to reinitalise just in case the Sampler is stopped and restarted...
  // Мы должны приступить к реинициализировать на случай, если Sampler остановлен и перезапущен ...
  dataReady = false;
  dataHigh = false;                       // Признак срабатывания компаратора
  adcDMAIndex = 0;
  adcTransferIndex = 0;
  for (int i = 0; i < NUMBER_OF_BUFFERS; i++)
  {
    memset((void *)adcBuffer[i], 0, BUFFER_SIZE);
  }

  ADC->ADC_RPR  = (unsigned long) adcBuffer[adcDMAIndex];         // DMA buffer
  ADC->ADC_RCR  = (unsigned int)  BUFFER_SIZE;                    // ADC works in half-word mode.
  ADC->ADC_RNPR = (unsigned long) adcBuffer[(adcDMAIndex + 1)];   // next DMA buffer
  ADC->ADC_RNCR = (unsigned int)  BUFFER_SIZE;

  // Enable interrupts
  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
  NVIC_DisableIRQ(ADC_IRQn);
  NVIC_ClearPendingIRQ(ADC_IRQn);  
  NVIC_SetPriority(ADC_IRQn, 6);  
  NVIC_EnableIRQ(ADC_IRQn);
  ADC->ADC_PTCR  =  ADC_PTCR_RXTEN;                               // Enable receiving data.
  ADC->ADC_CR   |=  ADC_CR_START;                                 // start waiting for trigger.

  // Start timer
  TC0->TC_CHANNEL[0].TC_SR;
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN;
  TC_Start(TC0, 0);
}
void ADCSampler::end()
{

}

void ADCSampler::handleInterrupt()
{
  unsigned long status = ADC->ADC_ISR;
  if (status & ADC_ISR_ENDRX)  
  {                     // Проверить завершение преобразования
    adcTransferIndex = adcDMAIndex;
    adcDMAIndex = (adcDMAIndex + 1) % NUMBER_OF_BUFFERS;
    ADC->ADC_RNPR  = (unsigned long) adcBuffer[(adcDMAIndex + 1) % NUMBER_OF_BUFFERS];
    ADC->ADC_RNCR  = BUFFER_SIZE;
	dataReady = true;                               // Данные сформированы  
  }

  if (status & ADC_ISR_COMPE)                       // Проверить регистр АЦП компаратора порога
  {
	  //ADC->ADC_IDR = ~ADC_IDR_COMPE;              // сбросить регистры прерывания превышения порога компаратора АЦП  
	  //ADC->ADC_IDR = ~ADC_ISR_COMPE;
	  dataHigh = true;                              // Установить признак превышения порога компаратора АЦП    
  }
  else
  {
	  dataHigh = false;
  }
}

bool ADCSampler::available()
{
  return dataReady;
}
bool ADCSampler::available_compare()
{
	return dataHigh;
}
unsigned int ADCSampler::getSamplingRate()
{
  return sampleingRate;
}

uint16_t* ADCSampler::getFilledBuffer(int *bufferLength)
{
  *bufferLength = BUFFER_SIZE;
  return adcBuffer[adcTransferIndex];
}

void ADCSampler::readBufferDone()
{
  dataReady = false;
}


