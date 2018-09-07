#include "uart.h"

int main(void)
{
	uart0_init();
	transmits("NORFLASH\n\r");
	norflash_test();
	return 0;
}
