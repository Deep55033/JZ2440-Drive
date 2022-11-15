#include "i2c_control.h"

#define BH1750_ADDR  0x23
#define BH1750_COM_CON_MEASURE_H_START 0x10 


int bh1750_init();

void bh1750_timer_callback();

unsigned int bh1750_measure_test();

void bh1750_reset();

void bh1750_set_mode();


int bh1750_init() {
    unsigned char data = 0x1;
    i2c_msg msg = {
        .addr = BH1750_ADDR,
        .buf = &data,
        .state = I2C_MSG_STATE_UNDONE,
        .flag = I2C_FLAG_MSG_WRITE,
        .len = 1,
        .tran_count = 0,
    };
    // power on 
    int state = i2c_tranfer(&msg, 1);
    return state;
}

void bh1750_reset() {
    unsigned char data = 0x7;
    i2c_msg msg = {
        .addr = BH1750_ADDR,
        .buf = &data,
        .state = I2C_MSG_STATE_UNDONE,
        .flag = I2C_FLAG_MSG_WRITE,
        .len = 1,
        .tran_count = 0,
    };
    // resert
    int state = i2c_tranfer(&msg, 1);

}

void bh1750_set_mode() {
    unsigned char data = BH1750_COM_CON_MEASURE_H_START;
    i2c_msg start_measure_msg = {
        .addr = BH1750_ADDR,
        .buf = &data,
        .state = I2C_MSG_STATE_UNDONE,
        .flag = I2C_FLAG_MSG_WRITE,
        .len = 1,
        .tran_count = 0,
    };
    i2c_tranfer(&start_measure_msg, 1);
}

unsigned int bh1750_measure_test() {
    
    unsigned char read_buf[2] = {0};

    i2c_msg read_measure_res_msg = {
        .addr = BH1750_ADDR,
        .buf = read_buf,
        .state = I2C_MSG_STATE_UNDONE,
        .flag = I2C_FLAG_MSG_READ,
        .len = 2,
        .tran_count = 0,
    };

    int state = i2c_tranfer(&read_measure_res_msg, 1);
    unsigned int result_light = (read_buf[0] << 8) | read_buf[1];
    if (state == I2C_MSG_STATE_DONE) {
        print_data_unit("Current light is ", result_light, " lx");
    } else {
        puts("Read measure result error\r\n");
    }
}
