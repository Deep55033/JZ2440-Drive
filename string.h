//
// Created by 18716 on 2022/9/26.
//

#ifndef NAND_FLASH_STRING_H
#define NAND_FLASH_STRING_H

#define TO_UPPER(c) (c &= 0xdf)

#include "uart.h"

unsigned int strlen(const char * s);

unsigned int get_uint();

int strcmp(const char * cs,const char * ct);

#endif //NAND_FLASH_STRING_H
