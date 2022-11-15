

void oled_write_command(unsigned char com) {
    GPGDAT &= ~(1 << 4);  // 发送命令
    spi_gpio_write_data(com);
    GPGDAT |= (1 << 4);
}

void oled_write_data(unsigned char data) {
    GPGDAT |= (1 << 4);  // 发送数据
    spi_gpio_write_data(data);
} 

void oled_init() {
    oled_write_command(0xAE);	//关闭OLED
	oled_write_command(0x00);//设置列低位地址
	oled_write_command(0x10);//设置列高位地址
	oled_write_command(0x40);	//设置起始行地址及映射RAM显示起始行 (0x00~0x3F)
	oled_write_command(0x81);	//对比度设置
	oled_write_command(0xCF); 	// Set SEG Output Current Brightness
	oled_write_command(0xA1);	//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_command(0xC8);	//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_command(0xA8);	//设置驱动路数(1 to 64)
	oled_write_command(0x3f);	//--1/64 duty
	oled_write_command(0xD3);	//-设置显示偏移(0x00~0x3F)
	oled_write_command(0x00);	//-not offset
	oled_write_command(0xd5);	//--set display clock divide ratio/oscillator frequency
	oled_write_command(0x80);	//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_command(0xD9);	//--set pre-charge period
	oled_write_command(0xF1);	//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_command(0xDA);	//--set com pins hardware configuration
	oled_write_command(0x12);
	oled_write_command(0xDB);	//--set vcomh
	oled_write_command(0x40);	//Set VCOM Deselect Level
	oled_write_command(0x20);	//设置页地址模式(0x00/0x01/0x02)
	oled_write_command(0x02);	//
	oled_write_command(0x8D);	//--set Charge Pump enable/disable
	oled_write_command(0x14);	//--set(0x10) disable
	oled_write_command(0xA4);	//显示开启(显示:A4;无显示:A5)
	oled_write_command(0xA7);	// 背景正反向显示 (0xa6:正显;a7:反显) 
	oled_write_command(0xAF);	//打开显示
}