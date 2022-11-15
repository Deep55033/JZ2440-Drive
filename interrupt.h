#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "s3c2440_soc.h"
#include "uart.h"
#include "spi/spi_test.h"
#include "single_bus/dht11.h"

typedef void (*irp_callback_fun)(unsigned int irq);

// 中断控制器初始化
void interrupt_init();

void key_interrupt_init();

void interrupt_handler();

void key_handler(unsigned int offset);

void register_irq(unsigned int irq, irp_callback_fun p);

void disable_irq();

void enable_irq();

#endif // !_INTERRUPT_H_