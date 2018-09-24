#include "uart.h"
#include "lcd.h"

int main(void)
{
	uart0_init();
	transmits("LCD\n\r");
	lcd_test();
	return 0;
}
