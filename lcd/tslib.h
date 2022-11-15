#ifndef _TSLIB_H_
#define _TSLIB_H_

#include "geometry.h"
#include "font.h"
#include "touch_screen.h"

void ts_calibration();


void ts_calibration_point(unsigned int x, unsigned int y, int *x_analog_res, int *y_analog_res);

int ts_is_xy_swap(int x1, int y1, int x2, int y2);

void ts_xy_swap(int *x, int *y) ;

void ts_read(int *lcd_x, int *lcd_y);
#endif