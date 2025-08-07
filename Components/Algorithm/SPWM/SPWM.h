#ifndef SPWM_H
#define SPWM_H


#include "arm_math.h"


#define SQRT2 1.4142135623730950488016887242097f

typedef struct {
    float32_t duty_l, duty_n;
} SPWM_HandleTypeDef;


/**
  * @brief  SPWM调制器
  * @param  hspwm：SPWM句柄指针
  * @param  Vsin:目标正弦电压瞬时值
  * @param  VDC：直流母线电压
  * @retval None
  */
void SPWM_VoltigeInput(SPWM_HandleTypeDef *hspwm, float32_t Vsin, float32_t VDC);
/**
  * @brief  比率式SPWM调制器
  * @param  hspwm：SPWM句柄指针
  * @param  rate:电压瞬时值/直流母线电压
  * @retval None
  */
void SPWM_RateInput(SPWM_HandleTypeDef *hspwm, float32_t rate);


#endif
