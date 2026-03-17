#include "stdio.h"
#include "uart.h"
#include "string.h"
#include <stdarg.h> // Required to handle variable arguments

// PRINT: basic print with support for %s, %d, %f
void PRINT(const char *type, ...) {
    va_list arguments;          // Store the variable arguments
    va_start(arguments, type);  // Initialize the variable arguments

    char buffer[32];  // Buffer for handling conversions to strings
    while (*type) {
        if (*type == '%') {  // If a format specifier is found  
            type++;          // Move to the next character

            if (*type == 's') {  // String
                char *str = va_arg(arguments, char *);
                uart_puts(str);

            } else if (*type == 'd') {  // Integer
                int num = va_arg(arguments, int);   
                uart_itoa(num, buffer);  // Convert integer to string
                uart_puts(buffer);
            } else if (*type == 'c') {   // chars
                char c = (char)va_arg(arguments, int);
                uart_putc(c);

            } 
            else if(*type == 'f'){
                double num = va_arg(arguments, double);  // Get the float
                ftoa(num, buffer, 6);  // Convert float to string using ftoa
                uart_puts(buffer);  // Print the float as string
            }

            else {
                uart_puts("[Unknown format specifier]");  // Invalid specifier
            }

        } else {
            uart_putc(*type);  // Print the character as is
        }
        type++;
    }

    va_end(arguments);  // End handling variable arguments
}

// READ: simple scanf-like function for %s and %d
// READ: simple scanf-like function for %s, %d y %f
void READ(const char *type, ...) {
    va_list arguments;
    va_start(arguments, type);

    char buffer[32];
    uart_gets_input(buffer, sizeof(buffer));

    if (*type == '%') {
        type++;

        if (*type == 's') {
            char *str = va_arg(arguments, char *);
            my_strncpy(str, buffer, sizeof(buffer));

        } else if (*type == 'd') {
            int *num = va_arg(arguments, int *);
            *num = uart_atoi(buffer);

        } else if (*type == 'f') {
            float *num = va_arg(arguments, float *);
            *num = uart_atof(buffer);

        } else {
            uart_puts("[Unknown format specifier]");
        }
    }

    va_end(arguments);
}
