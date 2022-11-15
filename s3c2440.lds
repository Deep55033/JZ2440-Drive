SECTIONS
{
	. = 0x30000000;

	__start_text = .;

	. = ALIGN(4);
	.text      :
	{
	  start.o *(.text)
	}

	. = ALIGN(4);
	.rodata : { *(.rodata) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) *(.COMMON) }
	__bss_end = .;
}

