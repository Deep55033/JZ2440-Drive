//
// Created by 18716 on 2022/9/25.
//

#ifndef NAND_FLASH_NAND_FLASH_H
#define NAND_FLASH_NAND_FLASH_H

#include "s3c2440_soc.h"
#include "uart.h"
#include "interrupt.h"
#include "string.h"

// 选中 nand flash
#define NAND_ENABLE_SELECT()  (NFCONT &= ~(0x1 << 1))
// 取消选中
#define NAND_DISABLE_SELECT()   (NFCONT |= (0x1 << 1))


void nand_init();

void nand_wait_ready();

void nand_send_command(unsigned char com);

void nand_send_address(unsigned char address);

unsigned char nand_read_data_byte();

void nand_read_data(unsigned int address, unsigned char *buf, unsigned int len);

void nand_display_data(unsigned int address);

void nand_scan_info();

void nand_erase_block(unsigned int address, unsigned int len);

void nand_display_data_test();

void nand_page_program(unsigned int address, char *buf, unsigned int len);

void nand_send_data_byte(char data);

void nand_erase_block_test();

void nand_page_program_test();

void nand_test();


#endif //NAND_FLASH_NAND_FLASH_H
