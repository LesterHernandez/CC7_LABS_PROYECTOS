#include "uart_in.h"
#include "uart_hw.h"
#include "uart.h"

char uart_getc(void) {
    while (!(uart_read_reg(UART_LSR) & UART_LSR_DR));
    return (char)(uart_read_reg(UART_RHR) & 0xFF);
}

void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;

    while (i < max_length - 1) {
        c = uart_getc();

        if (c == '\n' || c == '\r') {
            break;
        }

        buffer[i++] = c;
    }

    buffer[i] = '\0';
}