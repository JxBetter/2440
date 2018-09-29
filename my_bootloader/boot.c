#include "uart.h"

int main(void)
{
	uart0_init();
	transmits("see me\n\r");
	return 0;
}