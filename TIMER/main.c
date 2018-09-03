#include "uart.h"

char global_c = 'A';
char global_c2 = 'a';
unsigned int global_c3 = 0;

int main(void)
{
	unsigned char c;
	
	uart1_init();
	printHex(global_c);
	printHex(global_c2);
	printHex(global_c3);
	transmits("gujinxin!\n\r");
	transmit(global_c);
	transmit(global_c2);
	global_c ++;
	global_c2 ++;
	transmit(global_c);
	transmit(global_c2);
	
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