//
// Created by 18716 on 2022/9/27.
//

#include "lcd.h"

static p_lcd_params pri_lcd_params_array[LCD_ARRAY_NUMBER];
static p_lcd_params pri_select_lcd_params;

int lcd_register(p_lcd_params param) {
    for (int i = 0; i < LCD_ARRAY_NUMBER; i++) {
        if (!pri_lcd_params_array[i]) {
            pri_lcd_params_array[i] = param;
            return 0;
        }
    }
    return -1;
}


int lcd_select(const char *name) {
    for (int i = 0; i < LCD_ARRAY_NUMBER; i++) {
        if (pri_lcd_params_array[i] && !strcmp(name, pri_lcd_params_array[i]->name)) {
            pri_select_lcd_params = pri_lcd_params_array[i];
            return 0;
        }
    }
    return -1;
}

extern lcd_control jz2440_lcd_con;
extern lcd_params jz2240_lcd_par;

int lcd_init() {
    // 注册 jz2440 lcd 控制器
    lcd_control_register(&jz2440_lcd_con);
    if (lcd_control_select("jz2440") == -1) {
        puts("Lcd control select error\r\n");
    }
    lcd_register(&jz2240_lcd_par);
    if (lcd_select("jz2240_lcd") == -1) {
        puts("Lcd select error\r\n");
    }
    lcd_control_init(pri_select_lcd_params);
    // 使能 lcd
    lcd_control_enable();
    return 0;
}

inline p_lcd_params lcd_get_params() {
    return pri_select_lcd_params;
}

