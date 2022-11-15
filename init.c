//
// Created by 18716 on 2022/9/25.
//

#include "init.h"

char is_boot_from_nor_flash() {
    volatile unsigned int *p = (volatile unsigned int *) 0;
    *p = 0xFF;
    return *p == 0xFF ? 0 : 1;
}

void copy2sdram(void) {
    extern int __start_text, __bss_start;

    volatile unsigned int *dest = (volatile unsigned int *) &__start_text;
    volatile unsigned int *end = (volatile unsigned int *) &__bss_start;
    volatile unsigned int *src = (volatile unsigned int *) 0;

    // 判断是不是从nor flash中启动
    if (is_boot_from_nor_flash()) {

        while (dest < end) {
            *dest++ = *src++;
        }
    } else {

        // 初始化 nand flash
        nand_init();
        nand_read_data(0x0, (unsigned char *) dest, (int) &__bss_start - (int) &__start_text);
    }
}


void clean_bss(void) {

    extern int __bss_end, __bss_start;

    volatile unsigned int *start = (volatile unsigned int *) &__bss_start;
    volatile unsigned int *end = (volatile unsigned int *) &__bss_end;



    while (start <= end) {
        *start++ = 0;
    }
}

void system_init() {
    sdram_init();
    copy2sdram();
    clean_bss();
}