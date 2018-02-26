#ifndef ADCSAMPLER_H
#define ADCSAMPLER_H

#include <Arduino.h>

#define NUM_CHANNELS 3   // Установить количество аналоговых входов
/* 
ch7:A0 
ch6:A1
ch5:A2 
ch4:A3 
ch3:A4
ch2:A5
ch1:A6
ch0:A7 
ch10:A8
ch11:A9
ch12:A10
ch13:A11
*/
#define ADC_CHANNELS ADC_CHER_CH4 | ADC_CHER_CH3 | ADC_CHER_CH2  // Определить входа (A3, A4, A5)
#define BUFFER_SIZE 200*NUM_CHANNELS                             // Оределить размер буфера хранения измеряемого сигнала     
#define NUMBER_OF_BUFFERS 3                                      // Установить количество буферов
#define VOLT_REF        (3300)      // Величина опорного напряжения
/* The maximal digital value */
#define MAX_DIGITAL     (4095)      // Разрядность АЦП (максимальная для DUE)

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
	unsigned int samplingRate = 3000;                          // Частота вызова (стробирования) АЦП 50мс
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

