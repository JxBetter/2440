#include "uart.h"
#include "s3c2440_soc.h"

char global_char = 'a';

int eint_process_function(void)
{
	transmits("External interrupt!!\r\n");
	return 0;
}


int timer_irq_function(void)
{
	transmits("Timer interrupt!!\r\n");
	return 0;
}


int interrupt_init(void)
{
	INTMSK &= ~((1<<0));
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


int interrupt_function(void)
{
	unsigned int flag = INTOFFSET;
	if (0 == flag)
	{
		eint_process_function();
	}
	else if (10 == flag)
	{
		timer_irq_function();
	}
	SRCPND = 1<<flag;
	INTPND = 1<<flag;
	return 0;
}

