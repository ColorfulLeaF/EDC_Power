#ifndef _PID_H
#define _PID_H

#include "arm_math.h"

#define ABS(value) ((value) > 0 ? value : -value)

typedef struct
{
    float32_t Kp, Ki, Kd;
    float32_t i_limit;
    float32_t err, err_last, err_sum;
} PID_HandleTypeDef;

float32_t PID_Update(PID_HandleTypeDef *pack, float32_t actual, float32_t target);
void PID_Clear(PID_HandleTypeDef *pack);

#endif
