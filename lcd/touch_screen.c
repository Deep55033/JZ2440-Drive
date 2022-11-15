//
// Created by 18716 on 2022/9/29.
//

#include "touch_screen.h"

static int ts_timer_callback_flag = 0; 

static touch_screen_state touch_sta = touch_screen_state_RELEASE;
static int x_analog = 0, y_analog = 0;
static char is_xy_vaild = 0; // 数据是否有效

static timer_interrupt ts_tim_int = {
    .name = "ts",
    .tim_cal = ts_timer_callback
};

void touch_screen_init() {
    ADCCON &= ~(0xFF << 6);
    ADCCON |= 65 << 6;
    ADCCON |= (1 << 14);
    ADCCON &= ~(1 << 2);

    // 初始化中断
    register_irq(31, ADC_interrupt_handler);
    INTSUBMSK &= ~((1 << 10) | (1 << 9));

    // 按下屏幕后等待一段时间再进行ADC转换  等待电平稳定
    ADCDLY = 6000;

    // 注册定时器中断
    timer_register(&ts_tim_int);

    enter_down_wait_interrupt();
}

// 进入按下等待中断模式
void enter_down_wait_interrupt() {
    ADCTSC = 0xd3;
}

// 进入松开等待中断模式
void enter_up_wait_interrupt() {
    ADCTSC = 0x1D3;
}

// 进入自动测量模式
void enter_auto_measure() {
    ADCTSC = 1 << 2;
}

void ADC_interrupt_handler() {
    if ((SUBSRCPND >> 10) & 0x1) { // ADC 中断
        x_analog = (ADCDAT0 & 0x3FF);
        y_analog = (ADCDAT1 & 0x3FF);
        is_xy_vaild = 1;
        enter_up_wait_interrupt();
    } else if ((SUBSRCPND >> 9) & 0x1) { // 屏幕 中断
        if ((ADCDAT0 >> 15) & 0x1) { // up
            ts_timer_callback_flag = 0;
            touch_sta = touch_screen_state_RELEASE;
            enter_down_wait_interrupt();
            puts("ADC release\r\n");
        } else { // down
            puts("ADC down\r\n");
            ts_timer_callback_flag = 1;
            touch_sta = touch_screen_state_PRESS;
        }
    }

    // 清除中断
    SUBSRCPND |= (1 << 10) | (1 << 9);
}

void ts_timer_callback() {
    if (ts_timer_callback_flag) {
        if (!(ADCDAT0 >> 15 & 0x1)) { // 判断当前是否还是按下状态
            // 进入自动测量模式  测量点击位置
            enter_auto_measure();
            // 启动测量
            ADCCON |= 0x1;
            touch_sta = touch_screen_state_PRESS;
        } else { // 松开状态 
            puts("Timer release\r\n");
            ts_timer_callback_flag = 0;
            touch_sta = touch_screen_state_RELEASE;
            enter_down_wait_interrupt();
        }
        
    }
}

// 检查当前触摸屏状态
touch_screen_state ts_check_state() {
    return touch_sta;
}

void ts_get_xy_analog_value(int *x_res, int *y_res) {
    if (ts_check_state() != touch_screen_state_PRESS) {
        *x_res = -1, *y_res = -1;
        return;
    }
    // 处于按下状态且数据有效
    if (is_xy_vaild) {
        *x_res = x_analog;
        *y_res = y_analog;
        is_xy_vaild = 0;
    } else {
        *x_res = -2;
        *y_res = -2;
    }
}