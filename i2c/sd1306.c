#include "i2c_control.h"

#define SD1306_ADDR 0x3c 

void sd1306_init() {
    unsigned char data = 0xAF;
    i2c_msg msg = {
        .addr = SD1306_ADDR,
        .buf = &data,
        .state = I2C_MSG_STATE_UNDONE,
        .flag = I2C_FLAG_MSG_WRITE,
        .len = 1,
        .tran_count = 0,
    };

    int state = i2c_tranfer(&msg, 1);
    if (state == I2C_MSG_STATE_ERROR_NOACK) {
        puts("Not found device\r\n");
    } else {
        puts("Found device\r\n");
    }
}