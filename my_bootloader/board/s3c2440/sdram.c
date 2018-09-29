#include "s3c2440_soc.h"
#include "nand.h"
#include "uart.h"

void sdram_init(void)
{
	BWSCON = 0x22000000;

	BANKCON6 = 0x18001;
	BANKCON7 = 0x18001;

	REFRESH  = 0x8404f5;

	BANKSIZE = 0xb1;

	MRSRB6   = 0x20;
	MRSRB7   = 0x20;
}


int boot_from_nor(void)
{
	uart0_init();
	volatile unsigned char *zero = (volatile unsigned char *)0;
	unsigned char cache = *zero;

	*zero = 0x12;
	if (*zero == 0x12)
	{
		*zero = cache;
		transmits("in nanflash\n\r");
		return 0;
	}
	else
	{
		transmits("in norflash\n\r");
		return 1;
	}
}



int copy_to_sdram(void)
{
	int len;
	extern int __code_start, __bss_start;
	volatile unsigned char *src = (volatile unsigned char *)0;
	volatile unsigned char *dest = (volatile unsigned char *)&__code_start;
	volatile unsigned char *end = (volatile unsigned char *)&__bss_start;
	//len = (int)&bss_start - (int)&_code_start;
	len = end - dest;
	
	if (boot_from_nor())
	{
		while(dest < end)
		{
			*dest++ = *src++;
		}
		transmits("copy norflash to sdram success\n\r");
	}
	else
	{
		nandflash_init();
		read_nandflash((unsigned int)src, (unsigned char *)dest, len);
		transmits("copy nandflash to sdram success\n\r");
	}

	return 0;
}

int clean_bss(void)
{
	extern int __bss_start, __bss_end;
	volatile unsigned char *start = (volatile unsigned char *)&__bss_start;
	volatile unsigned char *end = (volatile unsigned char *)&__bss_end;
	
	while(start < end)
	{
		*start++ = 0;
	}
	return 0;
}


int sdram_test(void)
{
	volatile unsigned char *p = (volatile unsigned char *)0x30000000;
	int i;

	// write sdram
	for (i = 0; i < 1000; i++)
		p[i] = 0x55;

	// read sdram
	for (i = 0; i < 1000; i++)
		if (p[i] != 0x55)
			return -1;

	return 0;
}

