#ifndef _I2C_CONTROL_H_
#define _I2C_CONTROL_H_

#define I2C_FLAG_MSG_WRITE 0
#define I2C_FLAG_MSG_READ 1

#define I2C_MSG_STATE_ERROR_NOACK -1
#define I2C_MSG_STATE_UNDONE 0
#define I2C_MSG_STATE_DONE 1

#define I2C_CONTROL_NUMBER 10 
 


#include "../string.h"
#include "../interrupt.h"
#include "../s3c2440_soc.h"
#include "../uart.h"
#include "../timer.h"

typedef struct i2c_msg{
    unsigned char addr;
    int flag;
    char *buf;
    unsigned int len;
    unsigned int tran_count;
    int state;
}i2c_msg, *p_i2c_msg;

typedef struct i2c_control {
    const char *name;
    void (*init)();
    int (*tranfer)(p_i2c_msg p_msg, int num);
}i2c_control, *p_i2c_control;

void i2c_init(void);

int i2c_control_select(const char *name);

int i2c_control_register(p_i2c_control p_i2c_con);

int i2c_tranfer(p_i2c_msg p_msg, int num);

#endif // !_I2C_CONTROL_H_