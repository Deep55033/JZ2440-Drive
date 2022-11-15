#include "uart.h"

timer_interrupt tim_int = {
    .name = "uart",
    .tim_cal = timer_uart_callback
};


void uart0_init() {
    // 初始化引脚 为 串口
    GPHCON &= ~(0xf << 4);
    GPHCON |= (0xa << 4);
    // 默认上拉
    GPHUP &= (0x3 << 2);

    // 数据格式 8n1
    ULCON0 |= 0x3;

    // 波特率配置
    // UBRDIVn = (int)( UART clock / ( buad rate x 16) ) –1  UART clock = 66 MHZ
    UBRDIV0 = 35;

    // 使能发送端和接收端
    UCON0 |= 0x5;

    // 注册定时器中断
    // timer_register(&tim_int);
}

int putchar(int c) {
    // 等待上次数据发送完成
    while (!((UTRSTAT0 >> 2) & 0x1));
    UTXH0 = (unsigned char) c;
    return c;
}

int getchar() {
    // 等待数据
    while (!(UTRSTAT0 & 0x1));
    return URXH0 & 0xff;
}

int puts(const char *str) {
    while (*str) {
        putchar(*str);
        str++;
    }
    return 0;
}

void print_hex(unsigned int val) {
    unsigned char buf[8] = {0};
    unsigned char temp[9] = {0};

    for (int i = 0; i < 8; i++) {
        buf[i] = val & 0xf;
        val >>= 4;
    }
    for (int i = 7; i >= 0; i--) {

        if (0 <= buf[7 - i] && buf[7 - i] <= 9) {
            temp[i] = '0' + buf[7 - i];
        } else if (0xa <= buf[7 - i] && buf[7 - i] <= 0xf) {
            temp[i] = 'A' + (buf[7 - i] - 0xa);
        }
    }
    puts("0x");
    puts(temp);
}

void print_hex_byte(unsigned char c) {
    unsigned char buf[3] = {0};

    unsigned char head = c & 0xf;
    unsigned char tail = (c >> 4) & 0xf;

    if (0 <= head && head <= 9) {
        buf[1] = head + '0';
    } else {
        buf[1] = head + 'A' - 0xa;
    }

    if (0 <= tail && tail <= 9) {
        buf[0] = tail + '0';
    } else {
        buf[0] = tail + 'A' - 0xa;
    }
    puts(buf);
}

void print_data(const char *str, unsigned int data) {
    puts(str);
    print_hex(data);
    puts("\r\n");
}

void print_data_unit(const char *str, unsigned int data, const char *unit) {
    puts(str);
    print_hex(data);
    puts(unit);
    puts("\r\n");
}

// 10 ms 触发一次
void timer_uart_callback() {
    static unsigned int timer_callback_count = 0;
    timer_callback_count++;
    // 一秒触发一次
    if (timer_callback_count == 100) {
        timer_callback_count = 0;
        puts("Timer calllback 1 s\r\n");
    }
}
