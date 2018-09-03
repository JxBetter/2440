#ifndef _UART_
#define _UART_

int uart1_init(void);
int transmit(int c);
int receive(void);
int transmits(char * s);

#endif