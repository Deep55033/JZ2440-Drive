#include "spi_control.h"

void spi_gpio_init() {
    // SPICLK GPG7
    // SPIMOSI GPG6
    // SPIMISO GPG5   input 
    // CS GPG2
    // DC GPG4
    GPGCON &= ~((0x3 << 14) | (0x3 << 12) | (0x3 << 10) | (0x3 << 4) | (0x3 << 8));
    GPGCON |= (0x1 << 14) | (0x1 << 12) | (0x1 << 4) | (0x1 << 8);

    // 片选引脚默认输出高电平
    GPGDAT |= 1 << 2;
}

void spi_gpio_set_clk_line(int pin) {
    if (pin == PIN_HIGH) {
        GPGDAT |= (1 << 7);
    } else if (pin == PIN_LOW) {
        GPGDAT &= ~(1 << 7);
    }
}

void spi_gpio_set_data_line(int pin) {
    if (pin == PIN_HIGH) {
        GPGDAT |= (1 << 6);
    } else if (pin == PIN_LOW) {
        GPGDAT &= ~(1 << 6);
    }
}

 int spi_gpio_read_data_line() {
    return (GPGDAT >> 5) & 0x1;
}

void spi_gpio_write_data(unsigned char data) {
    // 写入数据  高位先发
    for (int i = 0; i < 8; i++) {
        // 拉低时钟
        spi_gpio_set_clk_line(PIN_LOW);
        // 发送
        spi_gpio_set_data_line((data >> (7 - i)) & 0x1);
        // 拉高时钟 从机在上升沿读取数据
        spi_gpio_set_clk_line(PIN_HIGH);
    }
}


unsigned char spi_gpio_read_data() {

    unsigned char val = 0;

    for (int i = 0; i < 8; i++) {
        spi_gpio_set_clk_line(PIN_LOW);

        val |= spi_gpio_read_data_line() << (7 - i) ;      

        spi_gpio_set_clk_line(PIN_HIGH);
    }
}


void spi_gpio_transfer(p_spi_msg p_msg) {
    if (p_msg->flag == SPI_MSG_FLAG_WRITE) {  // 发送数据
        while (p_msg->len) {
            spi_gpio_write_data(*p_msg->buf);
            p_msg->len--;
            p_msg->buf++;
        }
    } else if (p_msg->flag == SPI_MSG_FLAG_READ) { // 读取数据
        while (p_msg->len) {
            *p_msg->buf = spi_gpio_read_data();
            p_msg->len--;
            p_msg->buf++;
        }
    }
}

spi_control spi_gpio_analog_control = {
    .name = "spi_gpio",
    .init = spi_gpio_init,
    .transfer = spi_gpio_transfer
};
