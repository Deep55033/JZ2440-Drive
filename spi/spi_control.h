#ifndef _SPI_CONTROL_H_
#define _SPI_CONTROL_H_

#include "../string.h"
#include "../s3c2440_soc.h"

#define SPI_MSG_FLAG_READ 0
#define SPI_MSG_FLAG_WRITE 1 

#define SPI_CONTROL_NUMBER 10

typedef struct spi_msg {
    int flag;
    unsigned char *buf;
    unsigned int len;
}spi_msg, *p_spi_msg;

typedef struct spi_control {
    const char *name;
    void (*init)();
    void (*transfer)(p_spi_msg p_msg);
}spi_control, *p_spi_control;


extern spi_control spi_gpio_analog_control;
extern spi_control s3c2440_spi_con;

int spi_control_register(p_spi_control p_spi_con);

int spi_control_unregister(const char *name);

int spi_control_select(const char *name);

void spi_control_transfer(p_spi_msg p_msg);

void spi_control_init();

#endif // !_SPI_CONTROL_H_