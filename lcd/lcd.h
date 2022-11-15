//
// Created by 18716 on 2022/9/27.
//

#ifndef LCD_LCD_H
#define LCD_LCD_H

#define LCD_ARRAY_NUMBER 10
#define FRAME_BUFFER_BASE 0x32000000

#include "../uart.h"
#include "string.h"
#include "common/lcd_common.h"
#include "lcd_control.h"


int lcd_register(p_lcd_params param);

int lcd_select(const char *name);

int lcd_init();

p_lcd_params lcd_get_params();

#endif //LCD_LCD_H
