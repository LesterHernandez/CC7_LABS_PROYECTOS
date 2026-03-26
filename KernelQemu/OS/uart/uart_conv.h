#ifndef UART_CONV_H
#define UART_CONV_H

// Convierte string a entero
int uart_atoi(const char *s);

// Convierte entero a string
void uart_itoa(int num, char *buffer);

#endif
