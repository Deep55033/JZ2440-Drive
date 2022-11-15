//
// Created by 18716 on 2022/9/28.
//

#ifndef LCD_GEOMETRY_H
#define LCD_GEOMETRY_H

#include "framebuffer.h"

void fb_draw_circle(int xc, int yc, int r, int fill, unsigned long c);

void fb_draw_line(int x1, int y1, int x2, int y2, int color);

void fb_draw_circle_8(int xc, int yc, int x, int y, unsigned long c);

void fb_draw_cross(unsigned int x, unsigned int y, unsigned int len, unsigned color);

#endif //LCD_GEOMETRY_H
