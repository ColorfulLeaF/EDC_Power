//
// Created by 15082 on 2025/7/7.
//

#ifndef BSP_HRTIM_H
#define BSP_HRTIM_H
#include "hrtim.h"
#include "main.h"

/*
 * brief:我讨厌配置stm32底层，由此我希望实现如下这些功能
 *      1. 我希望配置完一遍cubemx后不用再去动hrtim相关的狗屎了
 *      2. 我希望后面的使用者只需要关注顶层信息，不用动底层
 *      3. 实现输入：频率，通道，完成初始化
 *      4. 可以更改占空比，死区
 */


typedef enum {TIM_A = 0, TIM_B, TIM_C, TIM_D, TIM_E, TIM_F, MASTER} e_tim;
typedef struct
{
    e_tim tim_port;
    float rate;
    HRTIM_HandleTypeDef *hrtimx;
}struct_hrtim;


void init_hrtim(HRTIM_HandleTypeDef *hrtimx, e_tim tim_port, float target_frequency);
void set_duty(e_tim tim_port, float target_duty);
void set_deadtime(e_tim tim_port,float deadtime_rising, float deadtime_falling);


#endif
//APP_HRTIM_H
