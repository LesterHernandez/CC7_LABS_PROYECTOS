#include "uart_in.h"
#include "uart_hw.h"
#include "uart.h"

char uart_getc(void) {
    // Esperar hasta que haya datos (FIFO NO vacío)
    while (uart_read_reg(UART_FR) & UART_FR_RXFE);

    return (char)(uart_read_reg(UART_DR) & 0xFF);
}

void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;

    while (i < max_length - 1) {
        c = uart_getc();

        // Enter (maneja \r y \n)
        if (c == '\n' || c == '\r') {
            uart_putc('\n');   // opcional: eco de salto de línea
            break;
        }

        // Eco del carácter (opcional pero útil)
        uart_putc(c);

        buffer[i++] = c;
    }

    buffer[i] = '\0';
}