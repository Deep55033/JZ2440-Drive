#include "spi_control.h"

static p_spi_control g_p_spi_con[SPI_CONTROL_NUMBER];
static p_spi_control g_p_spi_con_selected;

int spi_control_register(p_spi_control p_spi_con) {
    for (int i = 0; i < SPI_CONTROL_NUMBER; i++) {
        if (!g_p_spi_con[i]) { // 如果没有被注册
            g_p_spi_con[i] = p_spi_con;
            return 0;
        }
    }
    return -1;
}

int spi_control_unregister(const char *name) {
    for (int i = 0; i < SPI_CONTROL_NUMBER; i ++) {
        if (g_p_spi_con[i] && strcmp(g_p_spi_con[i]->name, name)) {
            g_p_spi_con[i] = (void *) 0;
            return 0;
        }
    }
    return -1;
}

int spi_control_select(const char *name) {
    for (int i = 0; i < SPI_CONTROL_NUMBER; i++) {
        if (g_p_spi_con[i] && strcmp(g_p_spi_con[i]->name, name) == 0) {
            g_p_spi_con_selected = g_p_spi_con[i];
            return 0;
        }
    }
    return -1;
}


void spi_control_transfer(p_spi_msg p_msg) {
    // 发送数据
    g_p_spi_con_selected->transfer(p_msg);
}


void spi_control_init() {
    // 注册控制器
    spi_control_register(&spi_gpio_analog_control);
    // 选择控制器
    spi_control_select("spi_gpio");
    // 初始化
    g_p_spi_con_selected->init();
}
