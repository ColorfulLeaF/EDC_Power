#include "PID.h"

float32_t PID_Update(PID_HandleTypeDef *pack, float32_t actual, float32_t target) {
    float32_t out;
    pack->err = target - actual;
    pack->err_sum += pack->err;
    if (pack->err_sum > pack->i_limit)
        pack->err_sum = pack->i_limit;
    else if (pack->err_sum < -pack->i_limit)
        pack->err_sum = -pack->i_limit;
    out = pack->Kp * pack->err + pack->Ki * pack->err_sum + pack->Kd * (pack->err - pack->err_last);
    pack->err_last = pack->err;
    return out;
}

void PID_Clear(PID_HandleTypeDef *pack) {
    pack->err = 0;
    pack->err_last = 0;
    pack->err_sum = 0;
}
