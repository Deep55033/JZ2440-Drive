//
// Created by 18716 on 2022/9/26.
//

#include "string.h"

unsigned int strlen(const char *s) {
    const char *sc;
    for (sc = s; *sc != '\0'; ++sc)
        /* nothing */;
    return sc - s;
}


unsigned int get_uint() {
    char buf[10] = {0};
    char cur_read_count = 0;
    unsigned int res = 0;

    while (1) {
        char input_char = getchar();
        putchar(input_char);
        if (input_char == '\r' || input_char == '\n') break;
        buf[cur_read_count++] = input_char;
    }

    for (int i = 0; i < cur_read_count; i++) {
        char item = buf[cur_read_count];
        if ('0' <= item && item <= '9') {
            res |= (item - '0') << (cur_read_count * 4);
        } else if (('A' <= item && item <= 'F') || ('a' <= item && item <= 'f')) {
            TO_UPPER(item);
            res |= ((item - 'A' + 0xa) << (cur_read_count * 4));
        }
    }

    return res;
}


int strcmp(const char * cs,const char * ct)
{
    register signed char __res;

    while (1) {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
    }

    return __res;
}