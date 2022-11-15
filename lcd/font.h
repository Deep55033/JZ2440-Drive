//
// Created by 18716 on 2022/9/29.
//

#ifndef LCD_FONT_H
#define LCD_FONT_H

#include "framebuffer.h"
#include "../uart.h"

void fb_draw_char(unsigned int x, unsigned int y, char c, unsigned int color);

void fb_draw_str(unsigned int x, unsigned int y, const char *str, unsigned int color);


#endif //LCD_FONT_H
