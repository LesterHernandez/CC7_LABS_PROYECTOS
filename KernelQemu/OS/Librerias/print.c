#include "print.h"
#include "uart_out.h"
#include "uart_conv.h"

// PRINT: soporta %s, %d, %c, %f
void PRINT(const char *type, ...) {

    va_list arguments;
    va_start(arguments, type);

    char buffer[32];

    while (*type) {
        if (*type == '%') {
            type++;

            if (*type == 's') {
                char *str = va_arg(arguments, char *);
                uart_puts(str);

            } else if (*type == 'd') {
                int num = va_arg(arguments, int);
                uart_itoa(num, buffer);
                uart_puts(buffer);

            } else if (*type == 'c') {
                char c = (char)va_arg(arguments, int);
                uart_putc(c);

            } else {
                uart_puts("[ERR]");
            }

        } else {
            uart_putc(*type);
        }

        type++;
    }

    va_end(arguments);
}