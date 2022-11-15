#include "uart.h"
#include "interrupt.h"
#include "nand_flash.h"
#include "lcd/lcd_test.h"
#include "i2c/i2c_control.h"
#include "spi/spi_control.h"
#include "single_bus/dht11.h"

void print1()
{
	puts("before\r\n");
}

void print2()
{
	puts("after\r\n");
}


int main(void) {
	// 中断初始化
	//interrupt_init();
	key_interrupt_init();
	// 定时器初始化
    timer4_init();
    // lcd_init();
    // touch_screen_init();
    // lcd_test();
    // i2c_init();
    // sd1306_init();
    // bh1750_init();
    // bh1750_reset();
    // bh1750_set_mode();
    spi_control_init();
    dht11_init();
    
    int temp, hum;



    while (1) {
        // char c = getchar();
        // if (c == '\r') putchar('\n');
        // else if (c == '\n') putchar('\r');
        // putchar(c);
        dht11_read_temp_and_humidity(&temp, &hum);
        delay(2000);
    }
}