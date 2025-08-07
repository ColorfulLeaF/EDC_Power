#include "SVPWM.h"


/**
  * @brief  SVPWM调制器
  * @param  hsvpwm：SVPWM句柄指针
  * @param  Uq：q轴电压
  * @param  Ud：d轴电压
  * @param  angle：相位角（弧度）
  * @param  VDC：直流母线电压
  * @retval None
  */
void SVPWM(SVPWM_HandleTypeDef *hsvpwm, float32_t Uq, float32_t Ud, float32_t angle, float32_t VDC) {
    float32_t Ualpha, Ubeta, sin_val, cos_val;
    sin_val = arm_sin_f32(angle), cos_val = arm_cos_f32(angle);
    arm_inv_park_f32(Ud, Uq, &Ualpha, &Ubeta, sin_val, cos_val);

    //扇区计算
    float32_t u_1 = Ubeta;
    float32_t u_2 = SQRT3 / 2.0f * Ualpha - Ubeta / 2.0f;
    float32_t u_3 = SQRT3 / (-2.0f) * Ualpha - Ubeta / 2.0f;
    uint8_t a, b, c;
    if (u_1 > 0.0) a = 1; else a = 0;
    if (u_2 > 0.0) b = 1; else b = 0;
    if (u_3 > 0.0) c = 1; else c = 0;



    float32_t t_1, t_2;
    switch (4 * c + 2 * b + a) {
        case 3:
            t_1 = SQRT3 / VDC * u_2;
            t_2 = SQRT3 / VDC * u_1;
            break;
        case 1:
            t_1 = -SQRT3 / VDC * u_2;
            t_2 = -SQRT3 / VDC * u_3;
            break;
        case 5:
            t_1 = SQRT3 / VDC * u_1;
            t_2 = SQRT3 / VDC * u_3;
            break;
        case 4:
            t_1 = -SQRT3 / VDC * u_1;
            t_2 = -SQRT3 / VDC * u_2;
            break;
        case 6:
            t_1 = SQRT3 / VDC * u_3;
            t_2 = SQRT3 / VDC * u_2;
            break;
        case 2:
            t_1 = -SQRT3 / VDC * u_3;
            t_2 = -SQRT3 / VDC * u_1;
            break;
    }
    //过调制保护
    if (t_1 + t_2 >= 1) {
        float32_t t = t_1 + t_2;
        t_1 = t_1 / t;
        t_2 = t_2 / t;
    }



    // 分配占空比到三个定时器
    float32_t t_com0 = (1 - t_1 - t_2) / 2;
    float32_t t_comy = t_com0 + t_2;
    float32_t t_comx = t_comy + t_1;

    switch (4 * c + 2 * b + a) {
        case 3:
            hsvpwm->duty_a = t_comx;
            hsvpwm->duty_b = t_comy;
            hsvpwm->duty_c = t_com0;
            break;
        case 1:
            hsvpwm->duty_a = t_comy;
            hsvpwm->duty_b = t_comx;
            hsvpwm->duty_c = t_com0;
            break;
        case 5:
            hsvpwm->duty_a = t_com0;
            hsvpwm->duty_b = t_comx;
            hsvpwm->duty_c = t_comy;
            break;
        case 4:
            hsvpwm->duty_a = t_com0;
            hsvpwm->duty_b = t_comy;
            hsvpwm->duty_c = t_comx;
            break;
        case 6:
            hsvpwm->duty_a = t_comy;
            hsvpwm->duty_b = t_com0;
            hsvpwm->duty_c = t_comx;
            break;
        case 2:
            hsvpwm->duty_a = t_comx;
            hsvpwm->duty_b = t_com0;
            hsvpwm->duty_c = t_comy;
            break;
        default:
            hsvpwm->duty_a = 0;
            hsvpwm->duty_b = 0;
            hsvpwm->duty_c = 0;
            break;
    }
}

/**
  * @brief  三次谐波注入
  * @param  hsvpwm：SVPWM句柄指针
  * @param  a：
  * @param  b：
  * @param  c：标幺量的abc
  * @retval None
  */
void SVPWM_ThirdHarmonicInjection(SVPWM_HandleTypeDef *hsvpwm, float32_t a, float32_t b, float32_t c) {
    a += 1;
    b += 1;
    c += 1;
    a /= SQRT3;
    b /= SQRT3;
    c /= SQRT3;

    float32_t max = ((a >= b ? a : b) >= c) ? (a >= b ? a : b) : c;
    float32_t min = ((a <= b ? a : b) <= c) ? (a <= b ? a : b) : c;
    float32_t third_harmonic = (1 - max - min) * 0.5f;

    a += third_harmonic;
    b += third_harmonic;
    c += third_harmonic;

    hsvpwm->duty_a = a;
    hsvpwm->duty_b = b;
    hsvpwm->duty_c = c;
}
