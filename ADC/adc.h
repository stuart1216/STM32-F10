#ifndef _adc_H  
#define _adc_H  
#include "stm32f10x.h"  
#include "stm32f10x_dma.h"  
#include "stm32f10x_adc.h"  
#define ADC1_DR_Address  ((uint32_t)0x4001244c);  
  
void adc_init(void);  
  
#endif  