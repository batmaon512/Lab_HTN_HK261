/*
 * software_timer.c
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#include "software_timer.h"

#define TIMER_CYCLE_2 1


//software timer variable
uint16_t flag_timer2 = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_MUL = 0;

static uint16_t scan_timer = 0;
static uint16_t scan_target_ticks = 0;

/**
  * @brief  Init timer interrupt
  * @param  None
  * @retval None
  */
void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}


/**
  * @brief  Set duration of software timer interrupt
  * @param  duration Duration of software timer interrupt
  * @retval None
  */
//void setTimer2(uint16_t duration){
//	timer2_MUL = duration/TIMER_CYCLE_2;
//	timer2_counter = timer2_MUL;
//	flag_timer2 = 0;
//}

void setTimer2(uint16_t duration_ms){
    timer2_MUL = duration_ms * 2;
    timer2_counter = timer2_MUL;
    flag_timer2 = 0;
}

/**
  * @brief  Cài đặt tần số quét LED khung hình (Bài 3)
  * @param  freq: 1, 25, 100 (Hz). Đặt 0 để dùng chế độ quét chuẩn 1ms (250Hz)
  */
void setLedScanFreq(uint16_t freq){
    scan_timer = 0;
    if (freq == 1) {
        scan_target_ticks = 500;
    }
    else if (freq == 25) {
        scan_target_ticks = 20;
    }
    else if (freq == 100) {
        scan_target_ticks = 5;
    }
    else {
        scan_target_ticks = 2;
    }
}

/**
* @brief  Hàm phục vụ ngắt phần cứng TIM2 (gọi mỗi 0.5ms)
**/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM2){
        // 1. Quản lý Software Timer 50ms cho vòng while(1)
        if(timer2_counter > 0){
            timer2_counter--;
            if(timer2_counter == 0) {
                flag_timer2 = 1;
                timer2_counter = timer2_MUL;
            }
        }

        // 2. Tự động quét LED định kỳ theo đúng nhịp cấu hình
        scan_timer++;
        if (scan_timer >= scan_target_ticks) {
            scan_timer = 0;
            led7_Scan();
        }
    }
}
