//
// Created by 15082 on 2025/7/26.
//

#ifndef MPPT_H
#define MPPT_H

typedef struct
{
    float voltage;
    float current;
    float max_power;
    float duty;
    float step;
    float duty_max;
    float duty_min;
}mppt;
void mppt_init(mppt *my_mppt, float start_duty, float step, float duty_max, float duty_min);
void mppt_detect(mppt *my_mppt,float current, float voltage);

#endif //MPPT_H
