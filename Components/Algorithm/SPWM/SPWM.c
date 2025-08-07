#include "SPWM.h"


/**
  * @brief  电压式SPWM调制器
  * @param  hspwm：SPWM句柄指针
  * @param  Vsin:目标正弦电压瞬时值
  * @param  VDC：直流母线电压
  * @retval None
  */
void SPWM_VoltigeInput(SPWM_HandleTypeDef *hspwm, float32_t Vsin, float32_t VDC) {
    hspwm->duty_l = 0.5f * Vsin / VDC + 0.5f;
    hspwm->duty_n = 0.5f * (-Vsin) / VDC + 0.5f;
}
/**
  * @brief  比率式SPWM调制器
  * @param  hspwm：SPWM句柄指针
  * @param  rate:电压瞬时值/直流母线电压
  * @retval None
  */
void SPWM_RateInput(SPWM_HandleTypeDef *hspwm, float32_t rate) {
    hspwm->duty_l = 0.5f * rate + 0.5f;
    hspwm->duty_n = 0.5f * -rate + 0.5f;
}
