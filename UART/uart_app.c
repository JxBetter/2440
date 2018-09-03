#include "uart.h"

int main(void)
{
	unsigned char c;
	
	uart1_init();
	transmits("gujinxin!\n\r");
	
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