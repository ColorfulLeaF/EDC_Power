#ifndef PLL_H
#define PLL_H


#include "arm_math.h"
#include "PID.h"


typedef struct {
    PID_HandleTypeDef pll_pi;
    float32_t q;
    float32_t w0, angle;
} PLL_HandleTypeDef;


/**
  * @brief  锁相环状态清空
  * @param  hpll: 锁相环结构体句柄
  * @retval none
  */
void PLL_Clear(PLL_HandleTypeDef *hpll);

/**
  * @brief  锁相环状态清空
  * @param  hpll: 锁相环句柄
  * @param  alpha: SOGI输出的alpha值
  * @param  beta: SOGI输出的beta值
  * @param  step: 执行周期（步长），单位：秒
  * @retval none
  */
void PLL_Update(PLL_HandleTypeDef *hpll, float32_t alpha, float32_t beta, float32_t step);


#endif
