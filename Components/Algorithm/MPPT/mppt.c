//
// Created by 15082 on 2025/7/26.
//

#include "mppt.h"

void mppt_init(mppt *my_mppt, float start_duty, float step, float duty_max, float duty_min)
{
    my_mppt->duty = start_duty;
    my_mppt->step = step;
    my_mppt->duty_max = duty_max;
    my_mppt->duty_min = duty_min;
    my_mppt->max_power = -1;
}

void mppt_detect(mppt *my_mppt,float current, float voltage)
{
    my_mppt->current = current;
    my_mppt->voltage = voltage;
    float power = current*voltage;
    if (power >= my_mppt->max_power)
    {
        my_mppt->max_power = power;
        my_mppt->duty += my_mppt->step;
    }
    else if(power < my_mppt->max_power)
    {
        my_mppt->step = -my_mppt->step;
        my_mppt->duty += my_mppt->step;
    }
    if(my_mppt->duty < my_mppt->duty_min) my_mppt->duty = my_mppt->duty_min;
    else if (my_mppt->duty > my_mppt->duty_max) my_mppt->duty = my_mppt->duty_max;

}