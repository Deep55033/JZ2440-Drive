#include "i2c_control.h"

static p_i2c_control p_i2c_control_array[I2C_CONTROL_NUMBER];
static p_i2c_control p_i2c_control_selected; 

int i2c_control_select(const char *name) {
    for (int i = 0; i < I2C_CONTROL_NUMBER; i ++) {
        if (p_i2c_control_array[i] && !strcmp(p_i2c_control_array[i]->name, name)) {
            p_i2c_control_selected = p_i2c_control_array[i];
            return 0;
        }
    }
    return -1;
}

int i2c_control_register(p_i2c_control p_i2c_con) {
    for (int i = 0; i < I2C_CONTROL_NUMBER; i ++) {
        if (!p_i2c_control_array[i]) {
            p_i2c_control_array[i] = p_i2c_con;
            return 0;
        }
    }
    return -1;
}

void i2c_init(void) {
    s3c2440_i2c_contol_add();
    if (i2c_control_select("s3c2440") == -1) {
        puts("I2c control select error\r\n");
    }
    p_i2c_control_selected->init();
}

int i2c_tranfer(p_i2c_msg p_msg, int num) {
    return p_i2c_control_selected->tranfer(p_msg, num);
}