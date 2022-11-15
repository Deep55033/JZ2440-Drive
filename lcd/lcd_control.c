//
// Created by 18716 on 2022/9/27.
//

#include "lcd_control.h"

static p_lcd_control lcd_control_array[LCD_CONTROL_NUMBER];
static p_lcd_control selected_lcd_control;


int lcd_control_select(const char *name) {
    for (int i = 0;i < LCD_CONTROL_NUMBER; i++) {
        if (lcd_control_array[i] && !(strcmp(name, lcd_control_array[i]->name))) {
            selected_lcd_control = lcd_control_array[i];
            return i;
        }
    }
    return -1;
}

int lcd_control_register(p_lcd_control p_lcd_con) {
    for (int i = 0; i < LCD_CONTROL_NUMBER; i++) {
        if (!lcd_control_array[i]) {
            lcd_control_array[i] = p_lcd_con;
            return 0;
        }
    }
    return -1;
}

int lcd_control_init(p_lcd_params p_lcd_par) {
    if (!selected_lcd_control) return -1;
    selected_lcd_control->init(p_lcd_par);
    return 0;
}

int lcd_control_enable() {
    if (!selected_lcd_control) return -1;
    selected_lcd_control->enable();
    return 0;
}

int lcd_control_disable() {
    if (!selected_lcd_control) return -1;
    selected_lcd_control->disable();
    return 0;
}
