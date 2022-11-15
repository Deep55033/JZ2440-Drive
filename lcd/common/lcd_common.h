//
// Created by 18716 on 2022/9/28.
//

#ifndef LCD_LCD_COMMON_H
#define LCD_LCD_COMMON_H

typedef enum pin_polarity_state {
    PIN_POLARITY_NORMAL = 0,
    PIN_POLARITY_INVERT = 1
} pin_polarity_state;

// 屏幕时序参数
typedef struct lcd_params_for_time_sequence_str {
    unsigned int thp;        // HSYNC 脉冲宽度
    unsigned int thb;        // 左黑边宽度
    unsigned int thf;        // 右黑边宽度

    unsigned int tvp;        // VSYNC 宽度
    unsigned int tvb;        // 上黑边宽度
    unsigned int tvf;        // 下黑边宽度

} pri_lcd_params_for_time_sequence, *pri_p_lcd_params_for_time_sequence;

// 屏幕引脚极性
typedef struct lcd_params_for_pin_polarity {
    pin_polarity_state data_enable;
    pin_polarity_state hsync;
    pin_polarity_state vsync;
    pin_polarity_state clk;
    pin_polarity_state data;
} pri_lcd_params_for_pin_polarity, *pri_p_lcd_params_for_pin_polarity;


typedef struct lcd_params {
    // 屏幕名字
    const char *name;
    pri_lcd_params_for_time_sequence lcd_time_sequence;
    pri_lcd_params_for_pin_polarity lcd_pin_polarity;
    unsigned int width;      // 屏幕宽
    unsigned int height;      // 屏幕宽
    int bpp;
    // frame buffer 基地址
    unsigned int frame_buf_base;
} lcd_params, *p_lcd_params;

typedef struct lcd_control {
    const char *name;

    void (*init)(p_lcd_params lcd_par);

    void (*disable)(void);

    void (*enable)(void);
} lcd_control, *p_lcd_control;

#endif //LCD_LCD_COMMON_H
