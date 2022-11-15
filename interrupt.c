#include "interrupt.h"

irp_callback_fun irp_callback_array[32];

void interrupt_init() {
    // 启用 EINT0 EINT2 EINT11 中断
    INTMSK &= ~((1 << 0) | (1 << 2) | (1 << 5));
}

void key_interrupt_init() {
    // 设置 GPF0 EINT0 GPF2 EINT2 为中断模式
    GPFCON |= ((0x2 << 0) | (0x2 << 4));
    // 设置 GPG3 EINT11 为中断模式
    GPGCON |= (0x2 << 6);

    // 设置 GPF0 EINT0 GPF2 EINT2 为双边沿触发
    EXTINT0 |= (0x7) | (0x7 << 8);
    // 设置 GPG3 EINT11 为双边沿触发
    EXTINT1 |= (0x7 << 12);

    // 使能 EINT11 中断
    EINTMASK &= ~(1 << 11);

    // EINTPEND  中断挂起寄存器
    // 注册中断
    register_irq(0, key_handler);
    register_irq(2, key_handler);
    register_irq(5, key_handler);
}

void interrupt_handler() {
    // 得到中断偏移值
    unsigned int offset = INTOFFSET;
    // 调用中断处理函数
    irp_callback_array[offset](offset);
    // 清除中断标志位
    SRCPND |= (1 << offset);
    INTPND |= (1 << offset);
}

void key_handler(unsigned int offset) {
    unsigned int val = EINTPEND;
    if (offset == 0) {
        // 按键 0 
        if (GPFDAT & 0x1) // high 
        {
            puts("key0 release\r\n");
            print_hex(TCNTO4);
        } else {
            puts("key0 enter\r\n");
        }
    } else if (offset == 2) {
        // 按键 2
        if ((GPFDAT >> 2) & 0x1) // high 
        {
            int temp, hum;
            puts("key2 release\r\n");
        } else {
            puts("key2 enter\r\n");
        }
    } else if (offset == 5) {
        if ((val >> 11) & 0x1) {
            if ((GPGDAT >> 3) & 0x1) // high 
            {
                
                puts("key11 release\r\n");
                // spi_test();
                
            } else {
                puts("key11 enter\r\n");
            }
        }
    }

    // 写一清除中断标志位
    EINTPEND = val;
}

void register_irq(unsigned int irq, irp_callback_fun p) {
    // 使能中断
    INTMSK &= ~((1 << irq));
    irp_callback_array[irq] = p;
}

void disable_irq() {
    asm(
            "mrs r0, cpsr\n\r"
            "orr r0, r0, #0x80\n\r"
            "msr cpsr, r0\n\r"
            );
}

void enable_irq() {
    asm(
            "mrs r0, cpsr\n\r"
            "bic r0, r0, #0x80\n\r"
            "msr cpsr, r0\n\r"
            );
}