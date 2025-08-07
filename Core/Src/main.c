/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "hrtim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_hrtim.h"
#include "bsp_dual_adc.h"
#include "SVPWM.h"
#include "generator.h"
#include "bsp_uart.h"
#include "PR.h"
#include "SPWM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float32_t freq = 50;
float32_t Vrms = 32.0f;
float32_t forward_back = 1.0f / 60 * SQRT3;
float32_t Kp = 0.01f, Kr = 50.0f;
float32_t Uab, Ubc, Uca, Ia, Ib, Ic, Ua, Uc;
float32_t angle = 0;//系统相位
SVPWM_HandleTypeDef hsvpwm1;
PR_HandleTypeDef hpra, hprc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    //采样
    Uab = ((float32_t)(adc_val[0] & 0xFFFF) / 4095.0f * 3.0f - 1.5f) / 0.02941176471f;
    Ubc = ((float32_t)(adc_val[0] >> 16) / 4095.0f * 3.0f - 1.5f) / 0.02941176471f;
    Uca = -Uab - Ubc;
    Ia = ((float32_t)(adc_val[1] & 0xFFFF) / 4095.0f * 3.0f - 1.5f) / 0.28f;
    Ic = ((float32_t)(adc_val[1] >> 16) / 4095.0f * 3.0f - 1.5f) / 0.28f;
    Ib = -Ia - Ic;
    Ua = (2.0f * Uab + Ubc) / 3.0f;
    Uc = -(2.0f * Ubc + Uab) / 3.0f;

    //生成参考波
    angle = FrequencyGenerator(freq * 2.0f * PI, 5e-5f);
    float32_t P = (Uab * Ia + Ubc * Ib + Uca * Ic) / SQRT3;
    float32_t phase_a;
    float32_t phase_c;
    if(P > 100.0f) {
        phase_a = (Vrms + 0.1f) * SQRT2 / SQRT3 * CosineGenerator(angle);
        phase_c = (Vrms + 0.1f) * SQRT2 / SQRT3 * CosineGenerator(angle + 2.0f * PI / 3.0f);
    } else {
        phase_a = Vrms * SQRT2 / SQRT3 * CosineGenerator(angle);
        phase_c = Vrms * SQRT2 / SQRT3 * CosineGenerator(angle + 2.0f * PI / 3.0f);
    }

    //PR电压外环与P电流内环
    PRController_Update(&hpra, Ua, phase_a);
    PRController_Update(&hprc, Uc, phase_c);
    float32_t A, B, C;
    A = (hpra.ctrl - Ia) * 0.06f + phase_a * forward_back;
    C = (hprc.ctrl - Ic) * 0.06f + phase_c * forward_back;

    //调制发波
    B = 0 - A - C;
    SVPWM_ThirdHarmonicInjection(&hsvpwm1, A, B, C);
    set_duty(TIM_A, hsvpwm1.duty_a);
    set_duty(TIM_B, hsvpwm1.duty_b);
    set_duty(TIM_C, hsvpwm1.duty_c);

    //再次使能ADC
    BSP_ADC_DualSampleStart();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin == KEY1_Pin) {
        if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) {
            freq++;
            PRController_Init(&hpra, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
            PRController_Init(&hprc, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
        }
    }
    if(GPIO_Pin == KEY4_Pin) {
        if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET) {
            freq--;
            PRController_Init(&hpra, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
            PRController_Init(&hprc, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
        }
    }
    if(GPIO_Pin == KEY2_Pin) {
        if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET) {
            freq += 10;
            PRController_Init(&hpra, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
            PRController_Init(&hprc, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
        }
    }
    if(GPIO_Pin == KEY5_Pin) {
        if(HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == GPIO_PIN_RESET) {
            freq -= 10;
            PRController_Init(&hpra, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
            PRController_Init(&hprc, Kp, Kr, (float32_t) freq * 2.0f * PI, 80);
        }
    }
    if(GPIO_Pin == KEY3_Pin) {
        if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET) {
            Vrms += 0.1f;
        }
    }
    if(GPIO_Pin == KEY6_Pin) {
        if(HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == GPIO_PIN_RESET) {
            Vrms -= 0.1f;
        }
    }
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
    PRController_Init(&hpra, Kp, Kr, (float32_t)freq * 2.0f * PI, 80);
    PRController_Init(&hprc, Kp, Kr, (float32_t)freq * 2.0f * PI, 80);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_HRTIM1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_UART4_Init();
  MX_DAC1_Init();
  /* USER CODE BEGIN 2 */
    BSP_ADC_DualSampleInit();//使能ADC采样
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2047);
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2047);
    HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac1,DAC_CHANNEL_2);
    set_deadtime(TIM_A, 0, 0);//设置死区
    init_hrtim(&hhrtim1, TIM_A, 20000);//设置定时器A的PWM频率（建议为MASTER频率的整数倍）
    init_hrtim(&hhrtim1, TIM_B, 20000);//设置定时器B的PWM频率（建议为MASTER频率的整数倍）
    init_hrtim(&hhrtim1, TIM_C, 20000);//设置定时器C的PWM频率（建议为MASTER频率的整数倍）
    init_hrtim(&hhrtim1, MASTER, 20000);//设置定时器MASTER的频率（用于多通道PWM的同步，以及ADC的采样触发）
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//      BSP_UART_TransmitFloat(&huart4, 3, Uab, Ia, Ua);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
