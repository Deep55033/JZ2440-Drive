#include "i2c_control.h"


void s3c2440_i2c_interrupt_handler(unsigned int irq);

void s3c2440_i2c_contol_init();

int s3c2440_i2c_contol_tranfer(p_i2c_msg p_msg, int num);

void s3c2440_i2c_contol_add(void);

static i2c_control s3c2440_i2c_contol = {
    .name = "s3c2440",
    .init = s3c2440_i2c_contol_init,
    .tranfer = s3c2440_i2c_contol_tranfer
};

static volatile p_i2c_msg cur_msg; 

void s3c2440_i2c_interrupt_handler(unsigned int irq) {
    cur_msg->tran_count ++;

    // 地址
    if (cur_msg->tran_count == 1 && (IICSTAT & 0x1)) { // 没有接受到 ACK
        puts("No ACK\r\n");
        cur_msg->state = I2C_MSG_STATE_ERROR_NOACK; 
        return;
    }

    if (cur_msg->flag == I2C_FLAG_MSG_WRITE) {
        if ((cur_msg->tran_count - 1) == cur_msg->len) {
            // 结束
            IICSTAT = 0xd0;
            // 清除中断标志位
            IICCON &= ~(1 << 4);
            cur_msg->state = I2C_MSG_STATE_DONE;
            for (int i = 0; i < 1000; i++);
        } else {
            IICDS = cur_msg->buf[cur_msg->tran_count - 1];
            // 清除中断标志位
            IICCON &= ~(1 << 4);
        }
    } else if (cur_msg->flag == I2C_FLAG_MSG_READ) {
        if (cur_msg->tran_count == 1) {  // 第一次发送地址
            IICCON |= (1 << 7);
            // 清除中断标志位
            IICCON &= ~(1 << 4);
        }
        else if ((cur_msg->tran_count - 1) > cur_msg->len) {
            // 结束
            IICSTAT = 0x90;
            // 清除中断标志位
            IICCON &= ~(1 << 4);
            cur_msg->state = I2C_MSG_STATE_DONE;
            for (int i = 0; i < 1000; i++);
        } else {
            cur_msg->buf[cur_msg->tran_count - 2] = IICDS & 0xFF;
            // 如果是最后一个数据 不需要回应ACK
            if (cur_msg->tran_count == cur_msg->len) {
               IICCON &= ~(1 << 7);
            } else {
                IICCON |= (1 << 7);
            }
            // 清除中断标志位
            IICCON &= ~(1 << 4);
        }
    }
}

void s3c2440_i2c_contol_init() { 
    // 引脚复用为 i2c
    GPECON |= (0x2 << 30) | (0x2 << 28);

    IICCON = (1 << 7) | (1 << 6) | (1 << 5);
    IICCON &= ~0xf;
    // 注册中断
    register_irq(27, s3c2440_i2c_interrupt_handler);
}


int s3c2440_i2c_contol_tranfer(p_i2c_msg p_msg, int num) {
    cur_msg = p_msg;
    for (int i = 0; i < num; i++) {
        if (cur_msg->flag == I2C_FLAG_MSG_WRITE) {  // 发送
            // 写入从机地址 
            IICSTAT |= 1 << 4;
            IICDS = cur_msg->addr << 1;
            IICSTAT = 0xF0;
        } else if (cur_msg->flag == I2C_FLAG_MSG_READ) { // 接受
            // 写入从机地址
            IICSTAT |= 1 << 4;
            IICDS = cur_msg->addr << 1;
            IICSTAT = 0xB0;
        }
        // 等待数据发送完成且没有错误发生
        while (cur_msg->state == I2C_MSG_STATE_UNDONE);
        
    }
    return cur_msg->state;
}

void s3c2440_i2c_contol_add() {
    i2c_control_register(&s3c2440_i2c_contol);
}