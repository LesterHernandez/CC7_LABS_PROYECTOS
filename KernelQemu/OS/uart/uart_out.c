#include "uart_out.h"
#include "uart_hw.h"
#include "uart.h"

void uart_putc(char c) {
    // Esperar hasta que el FIFO NO esté lleno
    while (uart_read_reg(UART_FR) & UART_FR_TXFF);

    uart_write_reg(UART_DR, c);
}

void uart_puts(const char *s) {
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');   // necesario para que se vea bien en terminal
        }
        uart_putc(*s++);
    }
}