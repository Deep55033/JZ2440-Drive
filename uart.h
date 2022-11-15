#ifndef __UART_H_
#define __UART_H_

#include "s3c2440_soc.h"
#include "timer.h"

void uart0_init();

int putchar(int c);


int getchar();

int puts(const char *str);

void print_hex(unsigned int val);

void print_hex_byte(unsigned char c);

void print_data(const char *str, unsigned int data);

void print_data_unit(const char *str, unsigned int data, const char *unit);

void timer_uart_callback();

#endif