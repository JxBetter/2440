#include "uart.h"

int main(void)
{
	uart0_init();
	transmits("NANDFLASH\n\r");
	nandflash_init();
	//nand_read_id();
	nandflash_test();
	return 0;
}
