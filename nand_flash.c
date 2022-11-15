//
// Created by 18716 on 2022/9/25.
//

#include "nand_flash.h"

void nand_init() {
    NFCONF &= ~(0x3 << 12);
    NFCONF |= (0x1 << 8);
    // 关闭软件锁
    NFCONT &= ~(0x1 << 12);
    NAND_DISABLE_SELECT();
    // 使能 nand flash 控制器
    NFCONT |= 0x1;
}


void nand_wait_ready() {
    while (!(NFSTAT & 0x1));
}


void nand_send_command(unsigned char com) {
    NFCCMD = com;
    for (int i = 0; i < 10; i++);
}

void nand_send_address(unsigned char address) {
    NFADDR = address;
    for (int i = 0; i < 10; i++);
}

unsigned char nand_read_data_byte() {
    return NFDATA;
}

void nand_read_data(unsigned int address, unsigned char *buf, unsigned int len) {
    unsigned int i = 0;

    unsigned int page = address / 2048;
    unsigned int col = address % 2048;


    NAND_ENABLE_SELECT();

    while (i < len) {
        /* 发出00h命令 */
        nand_send_command(0);

        /* 发出地址 */
        /* col addr */
        nand_send_address(col & 0xff);
        nand_send_address((col >> 8) & 0xff);

        /* row/page addr */
        nand_send_address(page & 0xff);
        nand_send_address((page >> 8) & 0xff);
        nand_send_address((page >> 16) & 0xff);

        /* 发出30h命令 */
        nand_send_command(0x30);

        /* 等待就绪 */
        nand_wait_ready();

        /* 读数据 */
        for (; (col < 2048) && (i < len); col++) {
            buf[i++] = nand_read_data_byte();
        }
        if (i == len)
            break;

        col = 0;
        page++;
    }

    NAND_DISABLE_SELECT();

}

void nand_display_data(unsigned int address) {
    NAND_ENABLE_SELECT();

    unsigned int page = address / 2048;
    unsigned int col = address & (2048 - 1);
    int read_count = 0;
    unsigned char buf[16] = {0};
    unsigned char buf_count = 0;

    while (read_count < 2048) {
        nand_send_command(0x00);

        nand_send_address(col & 0xff);
        nand_send_address((col >> 8) & 0xff);

        nand_send_address(page & 0xff);
        nand_send_address((page >> 8) & 0xff);
        nand_send_address((page >> 16) & 0xff);

        nand_send_command(0x30);

        nand_wait_ready();

        for (; (col < 2048) && (read_count < 2048); col++) {
            unsigned char read_data = nand_read_data_byte();
            buf[buf_count++] = read_data;
            read_count++;
            print_hex_byte(read_data);
            putchar(' ');
            if (!(buf_count % 16)) {
                puts("   ");
                for (int i = 0; i < 16; i++) {
                    // 可见字符
                    if (32 <= buf[i] && buf[i] <= 126) {
                        putchar(buf[i]);
                    } else {
                        // 不可见字符 用 . 表示
                        putchar('.');
                    }
                }
                puts("\r\n");
                // 重置计数
                buf_count = 0;
            }
        }
    }

    NAND_DISABLE_SELECT();
}

void nand_scan_info() {
    NAND_ENABLE_SELECT();

    // 发送 0x90 命令
    nand_send_command(0x90);
    // 发送地址 0
    nand_send_address(0x0);

    // 读取数据
    unsigned int maker_code = nand_read_data_byte();
    unsigned int device_id = nand_read_data_byte();
    unsigned int three_data = nand_read_data_byte();
    unsigned int four_data = nand_read_data_byte();
    unsigned int five_data = nand_read_data_byte();

    NAND_DISABLE_SELECT();

    print_data("Maker code = ", maker_code);
    print_data("device_id = ", device_id);
    print_data("Internal Chip Number = ", 1 << (three_data & 0x3));
    print_data_unit("Page size = ", 1 << (four_data & 0x3), " kb");
    print_data_unit("Block size = ", 64 << ((four_data >> 4) & 0x3), " kb");
    print_data("Plane number = ", 1 << ((five_data >> 2) & 0x3));
    print_data_unit("Plane size = ", 64 << ((five_data >> 4) & 0x7), " mb");
}

void nand_erase_block(unsigned int address, unsigned int len) {
    if (address % (128 * 1024)) {
        puts("Put address error\r\n");
        return;
    }

    NAND_ENABLE_SELECT();

    while (len--) {
        unsigned int page = address / 2048;

        nand_send_command(0x60);
        nand_send_address(page & 0xff);
        nand_send_address((page >> 8) & 0xff);
        nand_send_address((page >> 16) & 0xff);
        nand_send_command(0xd0);
        nand_wait_ready();
        // 查询擦除状态
        nand_send_command(0x70);

        if (!(nand_read_data_byte() & 0x1)) {
            puts("Erase block success!\r\n");
        } else {
            puts("Erase block error!\r\n");
        }

        address += 128 * 1024;
    }

    NAND_DISABLE_SELECT();
}

void nand_send_data_byte(char data) {
    NFDATA = data;
}

void nand_page_program(unsigned int address, char *buf, unsigned int len) {
    unsigned int page = address / 2048;
    unsigned int col = address % 2048;
    unsigned int count = 0;

    NAND_ENABLE_SELECT();

    while (count < len) {
        nand_send_command(0x80);

        nand_send_address(col & 0xff);
        nand_send_address((col >> 8) & 0xff);

        nand_send_address(page & 0xff);
        nand_send_address((page >> 8) & 0xff);
        nand_send_address((page >> 16) & 0xff);

        for (; (col < 2048) && (count < len); col++) {
            nand_send_data_byte(buf[count++]);
        }

        nand_send_command(0x10);
        nand_wait_ready();
        // 查询写入状态
        nand_send_command(0x70);
        if (!(nand_read_data_byte() & 0x1)) {
            puts("Write success!\r\n");
        } else {
            puts("Write error!\r\n");
        }

        col = 0;
        page++;
    }

    NAND_DISABLE_SELECT();
}

void nand_display_data_test() {
    puts("put read address: ");
    unsigned int address = get_uint();
    print_hex(address);
    puts("\r\n");
    nand_display_data(0x20000);
}


void nand_erase_block_test() {
    nand_erase_block(0x20000, 1);
}

void nand_page_program_test() {
    nand_page_program(0x20000, "Example NAND Flash", 18);
}


void nand_test() {
    char quit_flag = 0;
    while (!quit_flag) {
        puts("[s] Select NAND flash info\r\n");
        puts("[r] Read NAND flash \r\n");
        puts("[e] Erase block NAND flash \r\n");
        puts("[p] Page program NAND flash \r\n");
        puts("[q] Quit \r\n");

        switch (getchar()) {
            case 's':
            case 'S':
                nand_scan_info();
                break;
            case 'r':
            case 'R':
                nand_display_data_test();
                break;
            case 'e':
            case 'E':
                nand_erase_block_test();
                break;
            case 'p':
            case 'P':
                nand_page_program_test();
                break;
            case 'q':
            case 'Q':
                quit_flag = 1;
                break;

        }
    }
}

