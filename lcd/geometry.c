//
// Created by 18716 on 2022/9/28.
//

#include "geometry.h"

//void draw_circle(int x, int y, int r, int color)
//{
//    int a, b, num;
//    a = 0;
//    b = r;
//    while(2 * b * b >= r * r)          // 1/8圆即可
//    {
//        fb_draw_pixel(x + a, y - b,color); // 0~1
//        fb_draw_pixel(x - a, y - b,color); // 0~7
//        fb_draw_pixel(x - a, y + b,color); // 4~5
//        fb_draw_pixel(x + a, y + b,color); // 4~3
//
//        fb_draw_pixel(x + b, y + a,color); // 2~3
//        fb_draw_pixel(x + b, y - a,color); // 2~1
//        fb_draw_pixel(x - b, y - a,color); // 6~7
//        fb_draw_pixel(x - b, y + a,color); // 6~5
//
//        a++;
//        num = (a * a + b * b) - r*r;
//        if(num > 0)
//        {
//            b--;
//            a--;
//        }
//    }
//}

// 八对称性
inline void fb_draw_circle_8(int xc, int yc, int x, int y, unsigned long c) {
    // 参数 c 为颜色值
    fb_draw_pixel(xc + x, yc + y, c);
    fb_draw_pixel(xc - x, yc + y, c);
    fb_draw_pixel(xc + x, yc - y, c);
    fb_draw_pixel(xc - x, yc - y, c);
    fb_draw_pixel(xc + y, yc + x, c);
    fb_draw_pixel(xc - y, yc + x, c);
    fb_draw_pixel(xc + y, yc - x, c);
    fb_draw_pixel(xc - y, yc - x, c);
}

//Bresenham's circle algorithm
void fb_draw_circle(int xc, int yc, int r, int fill, unsigned long c) {
    // (xc, yc) 为圆心，r 为半径
    // fill 为是否填充
    // c 为颜色值

    // 如果圆在图片可见区域外，直接退出


    int x = 0, y = r, yi, d;
    d = 3 - 2 * r;

    if (fill) {
        // 如果填充（画实心圆）
        while (x <= y) {
            for (yi = x; yi <= y; yi++)
                fb_draw_circle_8(xc, yc, x, yi, c);

            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    } else {
        // 如果不填充（画空心圆）
        while (x <= y) {
            fb_draw_circle_8(xc, yc, x, y, c);

            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}


void fb_draw_line(int x1, int y1, int x2, int y2, int color) {
    int dx, dy, e;
    dx = x2 - x1;
    dy = y2 - y1;
    if (dx >= 0) {
        if (dy >= 0) // dy>=0
        {
            if (dx >= dy) // 1/8 octant
            {
                e = dy - dx / 2;
                while (x1 <= x2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        y1 += 1;
                        e -= dx;
                    }
                    x1 += 1;
                    e += dy;
                }
            } else        // 2/8 octant
            {
                e = dx - dy / 2;
                while (y1 <= y2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        x1 += 1;
                        e -= dy;
                    }
                    y1 += 1;
                    e += dx;
                }
            }
        } else           // dy<0
        {
            dy = -dy;   // dy=abs(dy)
            if (dx >= dy) // 8/8 octant
            {
                e = dy - dx / 2;
                while (x1 <= x2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        y1 -= 1;
                        e -= dx;
                    }
                    x1 += 1;
                    e += dy;
                }
            } else        // 7/8 octant
            {
                e = dx - dy / 2;
                while (y1 >= y2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        x1 += 1;
                        e -= dy;
                    }
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    } else //dx<0
    {
        dx = -dx;     //dx=abs(dx)
        if (dy >= 0) // dy>=0
        {
            if (dx >= dy) // 4/8 octant
            {
                e = dy - dx / 2;
                while (x1 >= x2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        y1 += 1;
                        e -= dx;
                    }
                    x1 -= 1;
                    e += dy;
                }
            } else        // 3/8 octant
            {
                e = dx - dy / 2;
                while (y1 <= y2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        x1 -= 1;
                        e -= dy;
                    }
                    y1 += 1;
                    e += dx;
                }
            }
        } else           // dy<0
        {
            dy = -dy;   // dy=abs(dy)
            if (dx >= dy) // 5/8 octant
            {
                e = dy - dx / 2;
                while (x1 >= x2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        y1 -= 1;
                        e -= dx;
                    }
                    x1 -= 1;
                    e += dy;
                }
            } else        // 6/8 octant
            {
                e = dx - dy / 2;
                while (y1 >= y2) {
                    fb_draw_pixel(x1, y1, color);
                    if (e > 0) {
                        x1 -= 1;
                        e -= dy;
                    }
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
}


void fb_draw_cross(unsigned int x, unsigned int y, unsigned int len, unsigned color) {
    fb_draw_line(x - len, y, x + len, y, color);
    fb_draw_line(x, y - len, x, y + len, color);
}