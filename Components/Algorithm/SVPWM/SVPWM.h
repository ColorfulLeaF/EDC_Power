#ifndef SVPWM_H
#define SVPWM_H


#include "main.h"
#include "arm_math.h"


#define SQRT3 1.7320508075688772935274463415059f

// SVPWM结构体定义
typedef struct {
    float32_t duty_a, duty_b, duty_c;
} SVPWM_HandleTypeDef;


/**
  * @brief  SVPWM波形发生器
  * @param  hsvpwm：SVPWM句柄指针
  * @param  Uq：q轴电压
  * @param  Ud：d轴电压
  * @param  angle：相位角（弧度）
  * @param  VDC：直流母线电压
  * @retval None
  */
void SVPWM(SVPWM_HandleTypeDef *hsvpwm, float32_t Uq, float32_t Ud, float32_t angle, float32_t VDC);
/**
  * @brief  三次谐波注入
  * @param  hsvpwm：SVPWM句柄指针
  * @param  a：
  * @param  b：
  * @param  c：标幺量的abc
  * @retval None
  */
void SVPWM_ThirdHarmonicInjection(SVPWM_HandleTypeDef *hsvpwm, float32_t a, float32_t b, float32_t c);


#endif
