//
// Created by 18716 on 2022/9/29.
//

#ifndef LCD_TOUCH_SCREEN_H
#define LCD_TOUCH_SCREEN_H

typedef enum touch_screen_state {
    touch_screen_state_PRESS = 0,  // 按下
    touch_screen_state_RELEASE = 1 // 释放
}touch_screen_state;

#include "../interrupt.h"
#include "../s3c2440_soc.h"
#include "../uart.h"

void ADC_interrupt_handler();

void touch_screen_init();

void enter_down_wait_interrupt();

void enter_up_wait_interrupt();

touch_screen_state ts_check_state();

void ts_timer_callback();

void ts_get_xy_analog_value(int *x_res, int *y_res);
#endif //LCD_TOUCH_SCREEN_H
