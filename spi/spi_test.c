#include "spi_test.h"

void spi_test() {
    unsigned char buf[5] = {0x55, 0x02, 0x38, 0x01, 0xbb};
    spi_msg msg = {
        .flag = SPI_MSG_FLAG_WRITE,
        .len = 5,
        .buf = buf
    };

    spi_control_transfer(&msg);
}