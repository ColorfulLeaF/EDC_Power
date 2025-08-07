#include "PR.h"


/**
  * @brief PR控制器初始化
  * @param hpr: PR控制器句柄指针
  * @retval None
  */
void PRController_Init(PR_HandleTypeDef *hpr, float32_t Kp, float32_t Kr, float32_t w0, float32_t limit) {
    hpr->b0 = Kp + Kr * 2.4e5f / (1.6e9f + 2.4e5f + w0 * w0);
    hpr->b1 = Kp * (2 * w0 * w0 - 3.2e9f) / (1.6e9f + 2.4e5f + w0 * w0);
    hpr->b2 = Kp * (1.6e9f - 2.4e5f + w0 * w0) / (1.6e9f + 2.4e5f + w0 * w0)
            - Kr * 2.4e5f / (1.6e9f + 2.4e5f + w0 * w0);
    hpr->a1 = (2 * w0 * w0 - 3.2e9f) / (1.6e9f + 2.4e5f + w0 * w0);
    hpr->a2 = (1.6e9f - 2.4e5f + w0 * w0) / (1.6e9f + 2.4e5f + w0 * w0);
    hpr->Upper = limit;
    hpr->Lower = -limit;
    hpr->ctrl_prev1 = 0.0f;
    hpr->ctrl_prev2 = 0.0f;
    hpr->err_prev1 = 0.0f;
    hpr->err_prev2 = 0.0f;
    hpr->ctrl = 0.0f;
}
/**
  * @brief PR控制器状态清零
  * @param hpr: PR控制器句柄指针
  * @retval None
  */
void PRController_Clear(PR_HandleTypeDef *hpr) {
    hpr->ctrl_prev1 = 0.0f;
    hpr->ctrl_prev2 = 0.0f;
    hpr->err_prev1 = 0.0f;
    hpr->err_prev2 = 0.0f;
    hpr->ctrl = 0.0f;
}
/**
  * @brief PR控制器迭代输出函数
  * @param hpr: PR控制器句柄指针
  * @param actual: 反馈测量值
  * @param target: 目标值
  * @retval None
  */
void PRController_Update(PR_HandleTypeDef *hpr, float32_t actual, float32_t target) {
    float32_t err = target - actual;
    hpr->ctrl = hpr->b0 * err
              + hpr->b1 * hpr->err_prev1
              + hpr->b2 * hpr->err_prev2
              - hpr->a1 * hpr->ctrl_prev1
              - hpr->a2 * hpr->ctrl_prev2;
    // 保存状态用于下一次迭代
    hpr->ctrl_prev2 = hpr->ctrl_prev1;
    hpr->ctrl_prev1 = hpr->ctrl;
    hpr->err_prev2 = hpr->err_prev1;
    hpr->err_prev1 = err;

    if(hpr->ctrl > hpr->Upper) {
        hpr->ctrl = hpr->Upper;
    }
    if(hpr->ctrl < hpr->Lower) {
        hpr->ctrl = hpr->Lower;
    }
}
