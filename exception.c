#include "exception.h"

void undefined_exception(unsigned int cpsr, char *msg) {
    puts("cpsr = ");
    print_hex(cpsr);
    puts(msg);
}

void swi_exception(unsigned int *swi_in, char *msg) 
{
    print_hex((*swi_in) & (~0xff000000));
    puts(msg);
}