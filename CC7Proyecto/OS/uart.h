
#ifndef UART_H
#define UART_H


#define UART0_BASE   0x44E09000

#define UART_THR     0x00  // Transmit Holding Register
#define UART_RHR     0x00  // Receive Holding Register
#define UART_LSR     0x14  // Line Status Register
#define UART_LSR_THRE 0x20 // Transmitter Holding Register Empty
#define UART_LSR_DR   0x01 // Data Ready

extern volatile unsigned int * const UART0;

void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_gets_input(char *buffer, int max_length);

int uart_atoi(const char *s);
void uart_itoa(int num, char *buffer);
float uart_atof(const char *s);  
void ftoa(float n, char *res, int afterpoint);



#endif // UART_H
