#include "read.h"
#include "uart_in.h"
#include "uart_conv.h"
#include "string.h"
#include "uart_out.h"

// READ: soporta %s, %d, %f
void READ(const char *type, ...) {

    va_list arguments;
    va_start(arguments, type);

    char buffer[32];

    // Leer desde UART
    uart_gets_input(buffer, sizeof(buffer));

    if (*type == '%') {
        type++;

        if (*type == 's') {
            char *str = va_arg(arguments, char *);
            my_strncpy(str, buffer, 32);

        } else if (*type == 'd') {
            int *num = va_arg(arguments, int *);
            *num = uart_atoi(buffer);

        }  else {
            uart_puts("[ERR]");
        }
    }

    va_end(arguments);
}