//
// Created by 18716 on 2022/9/27.
//

#include "lcd.h"

lcd_params jz2240_lcd_par = {
        .name = "jz2240_lcd",
        .height = 272,
        .width = 480,
        .lcd_pin_polarity = {
                .clk = PIN_POLARITY_NORMAL,
                .data_enable = PIN_POLARITY_NORMAL,
                .hsync = PIN_POLARITY_INVERT,
                .vsync = PIN_POLARITY_INVERT,
                .data = PIN_POLARITY_NORMAL
        },
        .lcd_time_sequence = {
                .thb = 2,
                .thp = 41,
                .thf = 2,
                .tvb = 2,
                .tvf = 2,
                .tvp = 10
        },
        .frame_buf_base = FRAME_BUFFER_BASE,
        .bpp = 16,
};
