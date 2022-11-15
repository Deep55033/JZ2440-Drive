//
// Created by 18716 on 2022/9/28.
//

#include "lcd_test.h"

void lcd_test() {
    p_lcd_params lcd_par = lcd_get_params();
    // 清屏为黑色
    fb_clear(0x0);

//    fb_draw_line(0, 0, lcd_par->width - 1, lcd_par->height - 1, 0xF800);
//    fb_draw_line(0, 0, 0, lcd_par->height - 1, 0xF800);
//    fb_draw_line(0, 0, lcd_par->width - 1, 3, 0xF800);
//    fb_draw_line(0, lcd_par->height - 1, lcd_par->width - 1, lcd_par->height - 1, 0xF800);
//    fb_draw_line(lcd_par->width - 1, 0, lcd_par->width - 1, lcd_par->height - 1, 0xF800);
//    fb_draw_line(lcd_par->width - 1, 0, 0, lcd_par->height - 1, 0xF800);
//
//    fb_draw_circle(lcd_par->width / 2, lcd_par->height / 2, lcd_par->height / 3, 1, 0x1f);

    fb_draw_str(20, 20, "This is test!\r\nBroad is jz2440.", 0xffff);
//    fb_draw_char(20, 20, 'A', 0xffff);

    ts_calibration();

    int lcd_x = 0, lcd_y = 0;

    while (1) {
        ts_read(&lcd_x, &lcd_y);
        puts("Read point\r\n");
        if (lcd_x < 0 || lcd_y < 0) continue;
        fb_draw_pixel(lcd_x, lcd_y, RED);
        puts("Drawing\r\n");
    }
}  