#include "sdram.h"

void sdram_init()
{	
	BWSCON = 0x22000000;

	BANKCON6 = 0x18001;
	BANKCON7 = 0x18001;

	REFRESH  = 0x8404f5;

	BANKSIZE = 0xb1;

	MRSRB6   = 0x20;
	MRSRB7   = 0x20;
}

int sdram_test()
{
	unsigned int *p = (unsigned int *) 0x30000000;
	for (int i = 0; i < 1000; i++) 
	{
		*p = 0x55;
		p ++;
	}
	p = (unsigned int *) 0x30000000;
	for (int i = 0; i < 1000; i++)
	{
		if (*p != 0x55)
		{	
			puts("sdram error\r\n");
			return 0;
		}
		p ++;
	}
	puts("sdram success\r\n");
	return 1;
}