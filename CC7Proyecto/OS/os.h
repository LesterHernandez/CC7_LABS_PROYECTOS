#ifndef OS_H
#define OS_H
#define UART0_BASE   0x44E09000   // Dirección base del UART0 en AM335x

// Offsets de registros (16550 compatible)
#define UART_THR     0x00  // Transmit Holding Register (escritura)
#define UART_RHR     0x00  // Receive Holding Register (lectura)
#define UART_LSR     0x14  // Line Status Register
#define UART_LSR_THRE 0x20 // THR Empty
#define UART_LSR_DR   0x01 // Data Ready
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_gets_input(char *buffer, int max_length);

int uart_atoi(const char *s);
void uart_itoa(int num, char *buffer);
float uart_atof(const char *s);  
void ftoa(float n, char *res, int afterpoint);



#endif // UART_H