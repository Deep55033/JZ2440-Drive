//
// Created by 18716 on 2022/9/27.
//

#ifndef LCD_LCD_CONTROL_H
#define LCD_LCD_CONTROL_H

#define LCD_CONTROL_NUMBER 10

#include "common/lcd_common.h"
#include "string.h"

int lcd_control_select(const char *name);

int lcd_control_register(p_lcd_control p_lcd_con);

int lcd_control_init(p_lcd_params p_lcd_par);

int lcd_control_enable();

int lcd_control_disable();


#endif //LCD_LCD_CONTROL_H
