PREFIX=arm-none-eabi-
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
OBJ=$(PREFIX)objcopy
OBJDUMP=$(PREFIX)objdump

CFLAG=-nostdlib -march=armv4

PROJECT=lcd

SOURCE_C=$(wildcard *.c)
SOURCE_S=$(wildcard *.s)

OBJECT_DIR=lcd i2c spi single_bus

OBJECT=start.o sdram.o init.o nand_flash.o uart.o main.o exception.o interrupt.o timer.o string.o
OBJECT+=$(patsubst %.c, %.o, $(foreach n, $(OBJECT_DIR), $(wildcard $(n)/*.c)))
OBJECT+=libgcc.a

DEP_FILES=$(patsubst %, .%.d, $(OBJECT))
DEP_FILES_REAL=$(wildcard $(DEP_FILES))

.PHONY: all
all: $(OBJECT)
	@# 启动文件必须要第一个链接
	$(LD) -T s3c2440.lds  $^ -o $(PROJECT).elf
	$(OBJ) -O binary -S $(PROJECT).elf $(PROJECT).bin
	$(OBJDUMP) -D $(PROJECT).elf > $(PROJECT).dis

ifneq ($(DEP_FILES_REAL), )
include  $(DEP_FILES_REAL)
endif

%.o : %.c
	$(CC) $(CFLAG) -c -o $@ $<

%.o : %.s
	$(CC) $(CFLAG) -c -o $@ $<

.PHONY: debug
debug:
	@echo $(OBJECT)

.PHONY: clean
clean:
	rm *.o *.elf *.bin *.dis lcd/*.o
