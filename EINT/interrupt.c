#include "uart.h"
#include "s3c2440_soc.h"

char global_char = 'a';

int eint_process_function(void)
{
	transmits("eint_process_function");
	if(0 == INTOFFSET)
	{
		transmit(global_char);
		global_char ++;
		SRCPND = 1<<0;
		INTPND = 1<<0;
	}
	return 0;
}


int interrupt_init(void)
{
	INTMSK &= ~(1<<0);
	return 0;
}


int gpio_eint_init(void)
{
	GPFCON &= ~0x3;
	GPFCON |= 0x02;
	EXTINT0 &= ~0x7;
	EXTINT0 |= 0x3;
	return 0;
}