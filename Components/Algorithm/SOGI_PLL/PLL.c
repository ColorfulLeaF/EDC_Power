#include "PLL.h"
#include "arm_math.h"


/**
  * @brief  锁相环状态清空
  * @param  hpll: 锁相环句柄
  * @retval none
  */
void PLL_Clear(PLL_HandleTypeDef *hpll) {
    PID_Clear(&hpll->pll_pi);
}

/**
  * @brief  锁相环状态清空
  * @param  hpll: 锁相环句柄
  * @param  alpha: SOGI输出的alpha值
  * @param  beta: SOGI输出的beta值
  * @param  step: 执行周期（步长），单位：秒
  * @retval none
  */
void PLL_Update(PLL_HandleTypeDef *hpll, float32_t alpha, float32_t beta, float32_t step) {
    arm_park_f32(alpha, beta, NULL, &hpll->q, arm_sin_f32(hpll->angle), arm_cos_f32(hpll->angle));
    hpll->w0 += PID_Update(&hpll->pll_pi, hpll->q, 0);
    hpll->angle += hpll->w0 * step;
    if (hpll->angle > 2.0f * PI){
        hpll->angle -= 2.0f * PI;
    }
    else if (hpll->angle < 0) {
        hpll->angle += 2.0f * PI;
    }
}
