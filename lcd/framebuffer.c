//
// Created by 18716 on 2022/9/28.
//

#include "framebuffer.h"


inline void fb_draw_pixel(unsigned int x, unsigned int y, unsigned int color) {
    p_lcd_params lcd_params = lcd_get_params();
    if (!lcd_params) return;
    fb_limit_xy(&x, &y);

    unsigned short *p =
            (unsigned short *) (lcd_params->frame_buf_base + (lcd_params->width * lcd_params->bpp * y / 8) +
                                (lcd_params->bpp * x / 8));
    *p = color & 0xffff;
}


void fb_limit_xy(unsigned int *px, unsigned int *py) {
    p_lcd_params lcd_params = lcd_get_params();
    if (lcd_params->width <= *px) *px = lcd_params->width - 1;
    if (lcd_params->height <= *py) *py = lcd_params->height - 1;
}

void fb_clear(unsigned int color) {
    p_lcd_params lcd_par = lcd_get_params();
    if (lcd_par->bpp == 16) {
        unsigned short *p = (unsigned short *) lcd_par->frame_buf_base;
        for (int i = 0; i < lcd_par->width; i++)
            for (int j = 0; j < lcd_par->height; j++)
                *p++ = color;
    }
}

