#include "uart_out.h"
#include "uart_hw.h"
#include "uart.h"

void uart_putc(char c) {
    while (!(uart_read_reg(UART_LSR) & UART_LSR_THRE));
    uart_write_reg(UART_THR, c);
}

void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}