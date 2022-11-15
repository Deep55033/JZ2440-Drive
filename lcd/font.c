//
// Created by 18716 on 2022/9/29.
//

#include "font.h"

extern const unsigned char fontdata_8x16[];

void fb_draw_char(unsigned int x, unsigned int y, char c, unsigned int color) {
    fb_limit_xy(&x, &y);
    unsigned char *c_p = (unsigned char *) &fontdata_8x16[c * 16];
    unsigned int x_offset = 0;
    unsigned int y_offset = 0;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            // 这个点需要着色
            if (*c_p & (1 << (7 - j))) fb_draw_pixel(x + x_offset, y + y_offset, color);
            else fb_draw_pixel(x + x_offset, y + y_offset, 0x0);
            x_offset++;
        }
        x_offset = 0;
        y_offset++;
        c_p++;
    }
}

void fb_draw_str(unsigned int x, unsigned int y, const char *str, unsigned int color) {
    p_lcd_params lcd_par = lcd_get_params();
    unsigned int cache_x = x;

    while (*str) {
        if (*str == '\r') x = cache_x;
        else if (*str == '\n') y += 16;
        else {
            fb_draw_char(x, y, *str, color);
            x += 8;
            if (lcd_par->width <= x) {
                x = cache_x;
                y += 16;
            }
        }
        str++;
    }
}