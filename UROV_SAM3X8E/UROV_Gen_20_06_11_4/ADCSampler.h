#ifndef ADCSAMPLER_H
#define ADCSAMPLER_H

#include <Arduino.h>
#include "Config.h"


class ADCSampler {
  public:
    ADCSampler();
    void begin(unsigned int samplingRate);
    void end();
    void handleInterrupt();
    bool available();

    void setLowBorder(uint32_t val) {_compare_Low = val; } 
    void setHighBorder(uint32_t val) {_compare_High = val; } 
    
	bool available_compare();
    unsigned int getSamplingRate();
    uint16_t* getFilledBuffer(int *bufferLength);

    void reset();

	void startComputeRMS();
	void getComputedRMS(uint32_t& result1, uint32_t& result2, uint32_t& result3);


	volatile bool dataHigh;                                    // Признак превышения порога компаратора АЦП
//	unsigned int samplingRate = 3000;                          // Частота вызова (стробирования) АЦП 50мс
  private:
    unsigned int samplingRate;
    volatile bool dataReady;
    uint16_t adcBuffer[NUMBER_OF_BUFFERS][BUFFER_SIZE];
    unsigned int adcDMAIndex;                                  //!< This hold the index of the next DMA buffer
    unsigned int adcTransferIndex;                             //!< This hold the last filled buffer
	uint32_t _compare_High = TRANSFORMER_HIGH_DEFAULT_BORDER;                                  // Верхний порог компаратора АЦП
	uint32_t _compare_Low = TRANSFORMER_LOW_DEFAULT_BORDER;                                   // Нижний порог компаратора АЦП

	volatile bool rmsComputeMode;
	volatile uint32_t rmsData1, rmsData2, rmsData3;
	volatile uint32_t rmsStartComputeTime;
};

extern ADCSampler adcSampler;


#endif /* ADCSAMPLER_H */

