#include "arm_math.h"
#include "SOGI.h"

/**
  * @brief SOGI复位清零
  * @param hsogi: SOGI句柄指针
  * @retval None
  */
void SOGI_Clear(SOGI_HandleTypeDef *hsogi) {
    hsogi->alpha_prev1 = 0.0f;
    hsogi->alpha_prev2 = 0.0f;
    hsogi->beta_prev1 = 0.0f;
    hsogi->beta_prev2 = 0.0f;
    hsogi->u_prev1 = 0.0f;
    hsogi->u_prev2 = 0.0f;
    hsogi->alpha = 0.0f;
    hsogi->beta = 0.0f;
}

/**
  * @brief SOGI处理函数 (每次采样调用)
  * @param hsogi: SOGI句柄指针
  * @param u: 当前输入值
  * @retval None
  */
void SOGI_Update(SOGI_HandleTypeDef *hsogi, float32_t u) {
    hsogi->alpha = hsogi->b0 * u - hsogi->b0 * hsogi->u_prev2 - hsogi->a1 * hsogi->alpha_prev1 - hsogi->a2 * hsogi->alpha_prev2;
//    hsogi->beta =  hsogi->b1 * u + 2 * hsogi->b1 * hsogi->u_prev1 + hsogi->b1 * hsogi->u_prev2 - hsogi->a1 * hsogi->beta_prev1 - hsogi->a2 * hsogi->beta_prev2;

    // 保存状态用于下一次迭代
    hsogi->alpha_prev2 = hsogi->alpha_prev1;
    hsogi->alpha_prev1 = hsogi->alpha;
//    hsogi->beta_prev2 = hsogi->beta_prev1;
//    hsogi->beta_prev1 = hsogi->beta;
    hsogi->u_prev2 = hsogi->u_prev1;
    hsogi->u_prev1 = u;
}
