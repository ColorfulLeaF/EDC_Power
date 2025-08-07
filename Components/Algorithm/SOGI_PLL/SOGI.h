#ifndef SOGI_H
#define SOGI_H

#include "arm_math.h"

// SOGI结构体定义
typedef struct {
    // 传递函数系数
    float32_t a1, a2;
    float32_t b0, b1;

    // 状态变量
    float32_t alpha_prev1, alpha_prev2;
    float32_t beta_prev1, beta_prev2;
    float32_t u_prev1, u_prev2;

    // 输出
    float32_t alpha;        // 同相输出
    float32_t beta;       // 正交输出
} SOGI_HandleTypeDef;

/**
  * @brief SOGI初始化
  * @param hsogi: SOGI句柄指针
  * @retval None
  */
void SOGI_Clear(SOGI_HandleTypeDef *hsogi);

/**
  * @brief SOGI处理函数 (每次采样调用)
  * @param hsogi: SOGI句柄指针
  * @param u_k: 当前输入值
  * @retval None
  */
void SOGI_Update(SOGI_HandleTypeDef *hsogi, float32_t u_k);


#endif
