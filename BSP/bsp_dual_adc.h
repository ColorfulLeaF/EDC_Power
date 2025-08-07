#ifndef BSP_DUAL_ADC_H
#define BSP_DUAL_ADC_H


#include "main.h"
#include "adc.h"


extern uint32_t adc_val[];


void BSP_ADC_DualSampleInit();
void BSP_ADC_DualSampleStart();


#endif
