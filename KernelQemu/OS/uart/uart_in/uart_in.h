#ifndef UART_IN_H
#define UART_IN_H

// Lee un caracter desde UART
char uart_getc(void);

// Lee una cadena desde UART (con limite de longitud)
void uart_gets_input(char *buffer, int max_length);

#endif
