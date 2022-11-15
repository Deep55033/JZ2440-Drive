//
// Created by 18716 on 2022/9/27.
//

#include "lcd_control.h"
#include "../s3c2440_soc.h"

void jz2440_lcd_control_init(p_lcd_params lcd_par);

void jz2440_lcd_control_enable(void);

void jz2440_lcd_control_disable(void);

lcd_control jz2440_lcd_con = {
        .name = "jz2440",
        .init = jz2440_lcd_control_init,
        .enable = jz2440_lcd_control_enable,
        .disable = jz2440_lcd_control_disable,
};

void jz2440_lcd_pin_init(void) {
    // 引脚初始化
    GPCCON |= 0xAAAAAAAA;
    GPDCON |= 0xAAAAAAAA;
    // lcd 背光引脚
    GPBCON |= 0x1;
}

void jz2440_lcd_control_init(p_lcd_params lcd_par) {
    jz2440_lcd_pin_init();
    // 设置 时钟 和 tft显示模式
    LCDCON1 |= (0x7 << 8) | (0x3 << 5);

    // 设置像素模式
    if (lcd_par->bpp == 16) {
        LCDCON1 |= (0xc << 1);
    } else if (lcd_par->bpp == 8) {
        LCDCON1 |= (0xb << 1);
    } else if (lcd_par->bpp == 24) {
        LCDCON1 |= (0xd << 1);
    } else return;

    // LCDCON1 [0] 用于开启lcd control

    // 初始化垂直信号时序
    LCDCON2 |= (((lcd_par->lcd_time_sequence.tvb - 1) << 24)) |
               ((lcd_par->height - 1) << 14) |
               ((lcd_par->lcd_time_sequence.tvf - 1) << 6) |
               ((lcd_par->lcd_time_sequence.tvp - 1));

    // 水平信号初始化
    LCDCON3 |= ((lcd_par->lcd_time_sequence.thb - 1) << 19) |
               ((lcd_par->width - 1) << 8) |
               (lcd_par->lcd_time_sequence.thf - 1);
    LCDCON4 |= lcd_par->lcd_time_sequence.thp - 1;

    if (lcd_par->bpp == 16) {
        // RGB 565 format
        LCDCON5 |= (1 << 11);
    }

    // 配置各个引脚极性
    LCDCON5 |= (lcd_par->lcd_pin_polarity.clk << 10) |
               (lcd_par->lcd_pin_polarity.hsync << 9) |
               (lcd_par->lcd_pin_polarity.vsync << 8) |
               (lcd_par->lcd_pin_polarity.data << 7) |
               (lcd_par->lcd_pin_polarity.data_enable << 6) |
               0x1;

    // farmer buffer 配置
    LCDSADDR1 = (lcd_par->frame_buf_base & ~(1 << 31)) >> 1;
    LCDSADDR2 = ((lcd_par->frame_buf_base + (lcd_par->width * lcd_par->height * lcd_par->bpp / 8)) >> 1) & 0x1FFFFF;
}

void jz2440_lcd_control_enable(void) {
    // 使能 LCD 控制器
    LCDCON1 |= 0x1;
    // 开启 LCD 电源
    GPBDAT |= 0x1;  // 输出高电平
}


void jz2440_lcd_control_disable(void) {
    // 使能 LCD 控制器
    LCDCON1 &= ~0x1;
    // 开启 LCD 电源
    GPBDAT &= ~0x1;
}