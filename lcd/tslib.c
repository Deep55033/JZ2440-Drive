
#include "tslib.h"

static double g_kx, g_ky;
static double g_kx_c, g_ky_c, g_lcd_xc, g_lcd_yc;
static char  is_swap_xy = 0;

void ts_calibration() {
    
    int a_x, a_y, b_x, b_y, c_x, c_y, d_x, d_y, e_x, e_y;

    p_lcd_params lcd_par = lcd_get_params();
    
    // 清空屏幕
    fb_clear(BLACK);
    fb_draw_str(70, 70, "Touch screen calibration!", WHITE);
    // a
    ts_calibration_point(50, 50, &a_x, &a_y);
    // b
    ts_calibration_point(lcd_par->width - 50, 50, &b_x, &b_y);
    // c
    ts_calibration_point(lcd_par->width - 50, lcd_par->height - 50, &c_x, &c_y);
    // d
    ts_calibration_point(50, lcd_par->height - 50, &d_x, &d_y);
    // e
    ts_calibration_point(lcd_par->width / 2, lcd_par->height / 2, &e_x, &e_y);

    if ((is_swap_xy = ts_is_xy_swap(a_x, a_y, b_x, b_y))) {
        ts_xy_swap(&a_x, &a_y);
        ts_xy_swap(&b_x, &b_y);
        ts_xy_swap(&c_x, &c_y);
        ts_xy_swap(&d_x, &d_y);
        ts_xy_swap(&e_x, &e_y);
    }

    double ts_s1 = b_x - a_x;
    double ts_s2 = c_x - d_x;
    double lcd_s = lcd_par->width - 100;

    double ts_d1 = d_y - a_y;
    double ts_d2 = c_y - b_y;
    double lcd_d = lcd_par->height - 100;

    g_kx = (2 * lcd_s) / (ts_s1 + ts_s2); 
    g_ky = (2 * lcd_d) / (ts_d1 + ts_d2);

    g_kx_c = e_x;
    g_ky_c = e_y;

    g_lcd_xc = lcd_par->width / 2;
    g_lcd_yc = lcd_par->height / 2;

    fb_draw_str(70, 70,"Touch screen calibration success!", WHITE);
}

int ts_is_xy_swap(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx > dy) return 0; 
    else return 1;
}

void ts_xy_swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void ts_calibration_point(unsigned int x, unsigned int y, int *x_analog_res, int *y_analog_res) {
    int measure_count = 0;
    int temp_x = 0, temp_y = 0, x_analog = 0, y_analog = 0;

    fb_draw_cross(x, y, 10, WHITE);
    // 等待屏幕被按下
    while (ts_check_state() != touch_screen_state_PRESS);
    // 在松开之前一直读取数据 求均值
    while (ts_check_state() == touch_screen_state_PRESS) {
        ts_get_xy_analog_value(&x_analog, &y_analog);
        // 说明已经松开
        if (x_analog == -1 || y_analog == -1) break;
        // 说明ADC还未转换完成
        else if (x_analog == -2 || y_analog == -2) continue;
        temp_x += x_analog;
        temp_y += y_analog;
        measure_count ++;
    }

    // 求出均值
    *x_analog_res = temp_x / measure_count;
    *y_analog_res = temp_y / measure_count;

    print_data("x_analog_res = ", *x_analog_res);
    print_data("y_analog_res = ", *y_analog_res);
}

void ts_read(int *lcd_x, int *lcd_y) {

    int x_analog = 0, y_analog = 0, temp_x = 0, temp_y = 0;

    p_lcd_params lcd_par =  lcd_get_params();

    // 等待屏幕被按下
    while (ts_check_state() != touch_screen_state_PRESS) {
       
    }

    ts_get_xy_analog_value(&x_analog, &y_analog);

    // 错误数据
    if (x_analog < 0 || y_analog < 0) {
        *lcd_x = -1;
        *lcd_y = -1;
        return;
    }

    if (is_swap_xy) {
        ts_xy_swap(&x_analog, &y_analog);
    }

    // 计算 lcd 坐标
    temp_x = g_kx * (x_analog - g_kx_c) + g_lcd_xc;
    temp_y = g_ky * (y_analog - g_ky_c) + g_lcd_yc;

    if (temp_x < 0 || temp_x >=  lcd_par->width || temp_y < 0 || temp_y >= lcd_par->height) {
        *lcd_x = -1;
        *lcd_y = -1;
        return;
    }

    *lcd_x = temp_x;
    *lcd_y = temp_y;
}