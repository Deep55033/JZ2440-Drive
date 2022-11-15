#include "timer.h"

p_timer_interrupt timer_interrupt_array[TIMER_CALLBACK_NUMBER];

static volatile unsigned int tick_count = 0;

// 每0.5秒一个中断
void timer4_init() {
    // PCLK 66.66 mhz
    // 预分频到 2 MHZ
    TCFG0 |= (33 << 8);

    // 最终 1 mhz   1 us 一个时钟

    // 5 ms 触发一次中断   
    TCNTB4 = 5000;

    // 手动更新定时器
    TCON |= (1 << 21);
    
    // 设置自动重加载 
    TCON |= (1 << 22);

    // 清除手动更新
    TCON &= ~(1 << 21);
    // 开启定时器
    TCON |= (1 << 20);

    // 注册定时器中断 在每次重载时发生中断
    register_irq(14, timer4_callback);
}

void timer4_callback(unsigned int irq)
{
    tick_count++;
    for (int i = 0; i < TIMER_CALLBACK_NUMBER; i++)  {
        if (timer_interrupt_array[i]) { // 已经注册
            timer_interrupt_array[i]->tim_cal();
        }
    }

}

int timer_register(p_timer_interrupt timer_int) {
    for (int i = 0; i < TIMER_CALLBACK_NUMBER; i++) {
        if (!timer_interrupt_array[i]) { // 如果没有被注册
            timer_interrupt_array[i] = timer_int;
            return 0;
        }
    }
    return -1;
}

int timer_unregister(const char *name) {
    for (int i = 0; i < TIMER_CALLBACK_NUMBER; i++) {
        if (timer_interrupt_array[i] && strcmp(timer_interrupt_array[i]->name, name)) {
            timer_interrupt_array[i] = (void *) 0;
            return 0;
        }
    }
    return -1;
}


void udelay(unsigned int us) {
    int pre = TCNTO4;
    int cur = 0;
    int count  = us;
    int delta = 0;;
    while (count > 0) {
        cur = TCNTO4;
        if (cur > pre) { // 发生重载
            delta = pre + (5000 - cur);
        } else {
            delta =  pre - cur;
        }
        count = count - delta; 
        pre = cur;
    }
}



void delay(unsigned int ms) {
    udelay(ms * 1000);
}


void delay_test() {
    GPGCON &= (0x3 << 12);
    GPGCON |= (0x1 << 12);
    while (1) {
        udelay(10);
        GPGDAT |= 1 << 6;
        udelay(10);
        GPGDAT &= ~(1 << 6);
    }
}