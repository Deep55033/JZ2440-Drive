#ifndef _DHT11_H_
#define _DHT11_H_

#include "../timer.h"
#include "../s3c2440_soc.h"
#include "../uart.h"

void dht11_init();

void dht11_data_as_output();

void dht11_data_as_input();

void dht11_data_set(int pin);

int dht11_data_read();

int dht11_read_temp_and_humidity(int *temp, int *humidity);

int dht11_data_wait_sigle(int pin, int timerout_us);

unsigned char dht11_data_revice_byte();

#endif // !_DHT11_H_