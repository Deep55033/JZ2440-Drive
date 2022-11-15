#ifndef _TIMER_H_
#define _TIMER_H_

#include "s3c2440_soc.h"
#include "interrupt.h"
#include "string.h"

#define UINT_MAX (~0U)
#define TIMER_CALLBACK_NUMBER 10

typedef void (*timer_callback) (void); 

typedef struct timer_interrupt{
    const char *name;
    timer_callback tim_cal;
} timer_interrupt, *p_timer_interrupt;

void timer4_init();

void timer4_callback(unsigned int irq);

int timer_register(p_timer_interrupt timer_int);

int timer_unregister(const char *name);

void udelay(unsigned int us);

void delay(unsigned int ms);

void delay_test();

#endif // !_TIMER_H_


