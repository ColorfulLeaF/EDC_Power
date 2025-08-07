#include "bsp_dual_adc.h"


uint32_t adc_val[3];


void BSP_ADC_DualSampleInit() {
    HAL_Delay(3000);
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_val,3);
}

void BSP_ADC_DualSampleStart() {
    HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_val,3);
}
