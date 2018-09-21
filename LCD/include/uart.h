#ifndef _UART_
#define _UART_

int uart0_init(void);
int transmit(int c);
int receive(void);
int transmits(char * s);
int printHex(unsigned int val);

#endif
