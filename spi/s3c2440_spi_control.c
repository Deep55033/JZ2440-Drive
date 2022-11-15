#include "spi_control.h"



void s3c2440_spi_control_init() {
    GPGCON |= 0x3f << 10;
    // SPI 1
    // 设置频率 为 1mhz
    SPPRE1 = 32;
    // 配置 SPI 模块
    SPCON1 |= (1 << 4) | (1 << 3);
}

void s3c2440_spi_control_tranfer(p_spi_msg p_msg) {
    if (p_msg->flag == SPI_MSG_FLAG_WRITE) {
        while (p_msg->len) {
            while (!(SPSTA1 & 0x1)); // 等待发送和接受准备就绪
            SPTDAT1 = *p_msg->buf;
            p_msg->len--;
            p_msg->buf++;
        }
    } else if (p_msg->flag == SPI_MSG_FLAG_READ) {
        while (p_msg->len) {
            SPTDAT1 = 0xff;
            while (!(SPSTA1 & 0x1)); // 等待发送和接受准备就绪
            *p_msg->buf = SPTDAT1;
            p_msg->len--;
            p_msg->buf++;
        }
    }
}

spi_control s3c2440_spi_con = {
    .name = "s3c2440",
    .init = s3c2440_spi_control_init,
    .transfer = s3c2440_spi_control_tranfer
};

