#ifndef PR_H
#define PR_H


#include "arm_math.h"


// PR控制器结构体定义
typedef struct {
    // 传递函数系数
    float32_t a0, a1, a2;
    float32_t b0, b1, b2;

    // 状态变量
    float32_t err_prev1, err_prev2;
    float32_t ctrl_prev1, ctrl_prev2;

    // 输出
    float32_t ctrl;        // 控制量输出

    float32_t Upper, Lower;//输出限幅
} PR_HandleTypeDef;


/**
  * @brief PR控制器初始化
  * @param hpr: PR控制器句柄指针
  * @retval None
  */
void PRController_Init(PR_HandleTypeDef *hpr, float32_t Kp, float32_t Kr, float32_t w0, float32_t limit);
/**
  * @brief PR控制器状态清零
  * @param hpr: PR控制器句柄指针
  * @retval None
  */
void PRController_Clear(PR_HandleTypeDef *hpr);
/**
  * @brief PR控制器迭代输出函数
  * @param hpr: PR控制器句柄指针
  * @param actual: 反馈测量值
  * @param target: 目标值
  * @retval None
  */
void PRController_Update(PR_HandleTypeDef *hpr, float32_t actual, float32_t target);


#endif
