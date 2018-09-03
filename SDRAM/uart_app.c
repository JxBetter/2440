#include "uart.h"

int sdram_test(void);

int main(void)
{
	unsigned char c;
	
	uart1_init();
	transmits("hi sdram!\n\r");
	
	if(sdram_test() == 0)
	{
		transmits("sdram test successful!!!!\n");
	}
	else
	{
		transmits("sdram test false\n");
	}
	
	while(1)
	{
		c = receive();
		if (c == '\r')
		{
			transmit('\n');
		}

		if (c == '\n')
		{
			transmit('\r');
		}

		transmit(c);
	}
	return 0;
}

int sdram_test(void)
{
	volatile unsigned char *p = (volatile unsigned char *)0x31000000;
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


/*
	unsigned char c;
	uart1_init();
	transmits('gujinxin\n');
	while(1)
	{
		c = receive();
		transmit(c);
	}
	return 0;

*/