#ifndef ADCSAMPLER_H
#define ADCSAMPLER_H

#include <Arduino.h>

#define NUM_CHANNELS 6   // Установить количество аналоговых входов
/* 
ch7:A0 
ch6:A1
ch5:A2 
ch4:A3  Вход токового трансформатора №3
ch3:A4  Вход токового трансформатора №2
ch2:A5  Вход токового трансформатора №1
ch1:A6  Вход индуктивного датчика №1 тест исправности датчика
ch0:A7  Вход индуктивного датчика №2 тест исправности датчика
ch10:A8 Вход индуктивного датчика №3 тест исправности датчика
ch11:A9 Измерение =200В
ch12:A10 Измерение 3V3
ch13:A11 Измерение +5V
*/

#define ADC_CHANNELS ADC_CHER_CH2 | ADC_CHER_CH3 | ADC_CHER_CH4 | ADC_CHER_CH11 | ADC_CHER_CH12 | ADC_CHER_CH13     // 
#define ADC_CHANNELS_DIS  ADC_CHDR_CH0 | ADC_CHDR_CH1 | ADC_CHDR_CH10 | ADC_CHDR_CH5 | ADC_CHDR_CH6 | ADC_CHDR_CH7  // Отключить не используемые входа
#define BUFFER_SIZE 200*NUM_CHANNELS                             // Определить размер буфера хранения измеряемого сигнала     
#define NUMBER_OF_BUFFERS 6                                      // Установить количество буферов
//#define VOLT_REF        (2400)                                   // Величина опорного напряжения
/* The maximal digital value */
//#define ADC_RESOLUTION		12                                // Разрядность АЦП (максимальная для DUE)
class ADCSampler {
  public:
    ADCSampler();
    void begin(unsigned int samplingRate);
    void end();
    void handleInterrupt();
    bool available();
	bool available_compare();
    unsigned int getSamplingRate();
    uint16_t* getFilledBuffer(int *bufferLength);
    void readBufferDone();
	volatile bool dataHigh;                                    // Признак превышения порога компаратора АЦП
//	unsigned int samplingRate = 3000;                          // Частота вызова (стробирования) АЦП 50мс
  private:
    unsigned int sampleingRate;
    volatile bool dataReady;
    uint16_t adcBuffer[NUMBER_OF_BUFFERS][BUFFER_SIZE];
    unsigned int adcDMAIndex;                                  //!< This hold the index of the next DMA buffer
    unsigned int adcTransferIndex;                             //!< This hold the last filled buffer
	int _compare_High = 3500;                                  // Верхний порог компаратора АЦП
	int _compare_Low = 2500;                                   // Нижний порог компаратора АЦП


};

#endif /* ADCSAMPLER_H */

