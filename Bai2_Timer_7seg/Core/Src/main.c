/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifndef EX
#define EX 5
#endif
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#if (EX == 1)
uint16_t count_led_debug = 0;
uint16_t count_led_y0 = 0;
uint16_t count_led_y1 = 0;

#elif (EX == 2)
typedef enum {STATE_RED_GREEN,
	STATE_GREEN_YELLOW,
	STATE_YELLOW_RED}
TrafficState;

TrafficState traffic_state = STATE_RED_GREEN;

uint16_t traffic_timer = 0;

#elif (EX == 3)
#define SCAN_FREQ_HZ 1

#elif (EX == 4)
uint8_t hours = 11;
uint8_t mins = 59;
uint8_t secs = 50;
uint16_t count_sec = 0;
uint16_t count_colon = 0;
uint8_t colon_state = 0;

#elif (EX == 5)
int shift_digits[4] = {1, 2, 3, 4};
uint16_t count_shift = 0;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void system_init();
//void test_LedDebug();
//void test_LedY0();
//void test_LedY1();
//void test_7seg();

void system_init(void);

#if (EX == 1)
	void ex1_run(void);
#elif (EX == 2)
	void ex2_run(void);
#elif (EX == 3)
	void ex3_run(void);
#elif (EX == 4)
	void ex4_run(void);
#elif (EX == 5)
	void ex5_run(void);
#endif
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  system_init();

#if (EX == 3)
    setLedScanFreq(SCAN_FREQ_HZ); // Đặt tần số 1Hz, 25Hz hoặc 100Hz
#else
    setLedScanFreq(0);            // Mặc định quét mượt 1ms (250Hz)
#endif

#if (EX != 4)
	led7_SetColon(1);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  while(!flag_timer2);
		  flag_timer2 = 0;

	  #if (EX == 1)
		  ex1_run();
	  #elif (EX == 2)
		  ex2_run();
	  #elif (EX == 3)
		  ex3_run();
	  #elif (EX == 4)
		  ex4_run();
	  #elif (EX == 5)
		  ex5_run();
	  #endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void system_init(void)
{
  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
  timer_init();
  led7_init();
  setTimer2(50);
}

#if (EX == 1)
void ex1_run(void){
  count_led_debug = (count_led_debug + 1) % 40;
  if (count_led_debug == 0) {
    HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
  }

  count_led_y0 = (count_led_y0 + 1) % 120;
  if (count_led_y0 < 40) {
    HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1);
  } else {
    HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
  }

  count_led_y1 = (count_led_y1 + 1) % 120;
  if (count_led_y1 < 100) {
    HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 1);
  } else {
    HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
  }
}
#endif

#if (EX == 2)
void ex2_run(void){
  traffic_timer++;
  switch (traffic_state)
  {
    case STATE_RED_GREEN:
      HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1);
      HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
      HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
      if (traffic_timer >= 100) {
        traffic_timer = 0;
        traffic_state = STATE_GREEN_YELLOW;
      }
      break;

    case STATE_GREEN_YELLOW:
    	HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	    HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 1);
	    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
      if (traffic_timer >= 60) {
        traffic_timer = 0;
        traffic_state = STATE_YELLOW_RED;
      }
      break;

    case STATE_YELLOW_RED:
    	HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
		HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
		HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 1);
      if (traffic_timer >= 20) {
        traffic_timer = 0;
        traffic_state = STATE_RED_GREEN;
      }
      break;

    default:
    	traffic_timer = 0;
    	traffic_state = STATE_RED_GREEN;
    	break;
  }
}
#endif


#if (EX == 3)
void ex3_run(void){
    led7_SetDigit(1, 0, 0);
    led7_SetDigit(2, 1, 0);
    led7_SetDigit(3, 2, 0);
    led7_SetDigit(4, 3, 0);
}
#endif

#if (EX == 4)
void ex4_run(void){
  count_colon = (count_colon + 1) % 5;
  if (count_colon == 0) {
    colon_state = !colon_state;
    led7_SetColon(colon_state);
  }

  count_sec++;
  if (count_sec >= 20) {
    count_sec = 0;
    secs++;
    if (secs >= 60) {
      secs = 0;
      mins++;
      if (mins >= 60) {
        mins = 0;
        hours = (hours + 1) % 24;
      }
    }
  }

  led7_SetDigit(hours / 10, 0, 0);
  led7_SetDigit(hours % 10, 1, 0);
  led7_SetDigit(mins / 10, 2, 0);
  led7_SetDigit(mins % 10, 3, 0);
}
#endif

#if (EX == 5)
void ex5_run(void){
  count_shift++;
  if (count_shift >= 20) {
    count_shift = 0;

    int temp = shift_digits[3];
    shift_digits[3] = shift_digits[2];
    shift_digits[2] = shift_digits[1];
    shift_digits[1] = shift_digits[0];
    shift_digits[0] = temp;
  }

  for (uint8_t i = 0; i < 4; i++) {
    led7_SetDigit(shift_digits[i], i, 0);
  }
}
#endif
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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
