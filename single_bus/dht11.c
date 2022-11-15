#include "dht11.h"

void dht11_init() {
    dht11_data_as_output();
    dht11_data_set(PIN_HIGH);
    // 等待一秒
    delay(1000);
}

void dht11_data_as_output() {
    GPGCON &= ~(0x3 << 12);
    GPGCON |= (0x1 << 12);
}

void dht11_data_as_input() {
    GPGCON &= ~(0x3 << 12);
}

void dht11_data_set(int pin) {
    if (pin == PIN_HIGH) {
        GPGDAT |= 1 << 6;
    } else if (pin == PIN_LOW) {
        GPGDAT &= ~(1 << 6);
    }
}

int dht11_data_read() {
    return (GPGDAT >> 6) & 0x1;
}

int dht11_read_temp_and_humidity(int *temp, int *humidity) {
    // 发送起始信号
    dht11_data_as_output();
    dht11_data_set(PIN_LOW);
    delay(23);
    // 释放数据线等待响应  由上拉电阻拉高
    dht11_data_as_input();
    // 等待低电平
    if (dht11_data_wait_sigle(PIN_LOW, 1000)) {
        puts("Error: Slave low ack timerout!\r\n");
        return - 1;
    }
    // 等待高电平
    if (dht11_data_wait_sigle(PIN_HIGH, 1000)) {
        puts("Error: Slave high ack timerout!\r\n");
        return -1;
    }

    // 开始接受数据
    unsigned char humidity_high = dht11_data_revice_byte();
    unsigned char humidity_low = dht11_data_revice_byte();
    unsigned char temp_high = dht11_data_revice_byte();
    unsigned char temp_low = dht11_data_revice_byte();
    unsigned char checknum = dht11_data_revice_byte();

    if (humidity_high + humidity_low + temp_high + temp_low == checknum) {
        if ((temp_low >> 7) & 0x1) {
            *temp = -temp_high;
        } else {
            *temp = temp_high;
        }
        *humidity = humidity_high;
        print_data("Current hum = ", humidity_high);
        print_data("Current temp = ", temp_high);
        return 0;
    }

    puts("Error: Check num error\r\n");
    return -1;
}


int dht11_data_wait_sigle(int pin, int timerout_us) {
    while (timerout_us--) {
        if (dht11_data_read() == pin) {
            return 0;
        }
        udelay(1);
    }
    return -1;
}

unsigned char dht11_data_revice_byte() {
    unsigned char receive_data = 0; 
    for (int i = 0; i < 8; i++) {
        // 等待低电平
        if (dht11_data_wait_sigle(PIN_LOW, 1000)) {
            puts("Error: Slave low data timerout!\r\n");
            return 0;
        }
        // 等待高电平
        if (dht11_data_wait_sigle(PIN_HIGH, 1000)) {
            puts("Error: Slave high data timerout!\r\n");
            return 0;
        }
        udelay(40); // 等待40 us 判断高低电平 低 0 高 1

        // 易错点
        receive_data <<= 1;
        receive_data |= dht11_data_read();
    }
    return receive_data;
}