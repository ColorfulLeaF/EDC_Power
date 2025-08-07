//
// Created by 15082 on 2025/7/7.
//
#include "bsp_hrtim.h"


#define TIM_PORT(time_port) 0x1UL << (17U+tim_port)
#define TIM_CHANEL(time_port,CHANEL) (0x01<<(tim_port*2+CHANEL-1))
#define range(num,min,max) (((num)>=(min)) && ((num)<=(max)))
#define fHRTIM 170.0f


struct_hrtim hrtim_ary[7];
//倍频表
float prescaler[8] = {32, 16, 8, 4, 2, 1, 0.5f, 0.25f};


//理论频率范围：330Hz ~ 2.72MHz
void init_hrtim(HRTIM_HandleTypeDef *hhrtimx, e_tim tim_port, float target_frequency)
{
    if (tim_port != MASTER) {
        HAL_HRTIM_WaveformCountStop(hhrtimx, TIM_PORT(tim_port));
    } else {
        HAL_HRTIM_WaveformCountStop(hhrtimx, HRTIM_TIMERID_MASTER);
    }

    //倍频自适应，自动选择该PWM设定频率下分辨率最高的定时器倍频数
    uint8_t i;
    for (i = 0; i < 8; ++i) {
        if (tim_port != MASTER) {
            hrtim_ary[tim_port].rate = fHRTIM * 1000.0f * 1000.0f * prescaler[i] / 2.0f / target_frequency;
        } else {
            hrtim_ary[tim_port].rate = fHRTIM * 1000.0f * 1000.0f * prescaler[i] / target_frequency;
        }
        if (hrtim_ary[tim_port].rate <= 65503) {
            if (hrtim_ary[tim_port].rate >= 1000) {
                break;
            } else {
                i = 8;
            }
        }
    }

    //如果找不到合适的倍频数，说明该PWM设定频率已经不在HRTIM的调节范围内，调整为默认的60kHz
    if (i > 7) {
        hrtim_ary[tim_port].rate = 45333;
        i = 0;
    }


    HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg;
    pTimeBaseCfg.Period = (uint32_t)hrtim_ary[tim_port].rate;
    pTimeBaseCfg.RepetitionCounter = 0x00;
    pTimeBaseCfg.PrescalerRatio = i;
    pTimeBaseCfg.Mode = HRTIM_MODE_CONTINUOUS;
    if (HAL_HRTIM_TimeBaseConfig(hhrtimx, tim_port, &pTimeBaseCfg) != HAL_OK)
    {
        Error_Handler();
    }
    if (tim_port != MASTER) {
        HAL_HRTIM_WaveformOutputStart(hhrtimx, TIM_CHANEL(tim_port, 1) + TIM_CHANEL(tim_port, 2));//chanel 从1开始
    }
    if (tim_port != MASTER) {
        HAL_HRTIM_WaveformCountStart(hhrtimx, TIM_PORT(tim_port));
    } else {
        HAL_HRTIM_WaveformCountStart(hhrtimx, HRTIM_TIMERID_MASTER);
    }
}

//target_duty输入百分比，100%为1
void set_duty(e_tim tim_port, float target_duty)
{
    hhrtim1.Instance->sTimerxRegs[tim_port].CMP1xR = (uint32_t)(hrtim_ary[tim_port].rate * target_duty);
}

//输入单位nm，最高支持370ns
void set_deadtime(e_tim tim_port, float deadtime_rising, float deadtime_falling)
{
    uint32_t deadtime_count_ris = (uint32_t)(8.0f * deadtime_rising * fHRTIM / 1000.0f);
    uint32_t deadtime_count_fal = (uint32_t)(8.0f * deadtime_falling * fHRTIM / 1000.0f);
    if (range(deadtime_count_ris,0,511) && range(deadtime_count_fal,0,511))
    {
        //tmd挨个改东西太多了，部分不太可能改的东西就写死了，不想弄了，下班！
        HRTIM_DeadTimeCfgTypeDef deadtime_cfg;
        deadtime_cfg.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8; //默认8倍频
        deadtime_cfg.RisingValue = deadtime_count_ris;
        deadtime_cfg.FallingValue = deadtime_count_fal;
        deadtime_cfg.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
        deadtime_cfg.RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
        deadtime_cfg.RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
        deadtime_cfg.FallingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
        deadtime_cfg.FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
        deadtime_cfg.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;
        if (HAL_HRTIM_DeadTimeConfig(&hhrtim1,tim_port,&deadtime_cfg) != HAL_OK)
        {
            Error_Handler();
        }
    }
    else Error_Handler();
}
