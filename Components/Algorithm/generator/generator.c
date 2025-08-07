#include "generator.h"
#include "arm_math.h"


/**
  * @brief  角频率发生器
  * @param  w: 角频率
  * @param  step: 执行周期（步长），单位：秒
  * @retval 随时间变化的角度
  */
float32_t FrequencyGenerator(float32_t w, float32_t step) {
    static float32_t angle = 0;
    angle = angle + w * step;
    if (angle > 2.0f * PI){
        angle -= 2.0f * PI;
    }
    else if (angle < 0) {
        angle += 2.0f * PI;
    }
    return angle;
}

/**
  * @brief  -1~1余弦发生器
  * @param  angle: 角弧度
  * @retval 随时间变化的余弦值
  */
float32_t CosineGenerator(float32_t angle) {
    if (angle > 2.0f * PI){
        angle -= 2.0f * PI;
    }
    else if (angle < 0) {
        angle += 2.0f * PI;
    }
    return arm_cos_f32(angle);
}