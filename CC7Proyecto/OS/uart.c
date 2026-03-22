#include "uart.h"


extern void PUT32(unsigned int address, unsigned int value);
extern unsigned int GET32(unsigned int address);

void uart_putc(char c) {
    // Espera a que el THR esté vacío leyendo directamente de la memoria
    while (!(GET32(UART_LSR) & UART_LSR_THRE));
    
    // Escribe el caracter de forma segura
    PUT32(UART_THR, c);
}

char uart_getc(void) {
    // Espera a que haya datos
    while (!(GET32(UART_LSR) & UART_LSR_DR));
    
    // Lee el caracter
    return (char)(GET32(UART_RHR) & 0xFF);
}

// Función para enviar una cadena por UART
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Función para recibir una línea de entrada por UART
void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    while (i < max_length - 1) { // Deja espacio para el terminador nulo
        c = uart_getc();
        if (c == '\n' || c == '\r') {
            uart_putc('\n'); // Eco del salto de línea
            break;
        }
        uart_putc(c); // Eco del carácter
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Termina la cadena con null
}

// Función simple para convertir una cadena a entero
int uart_atoi(const char *s) {
    int num = 0;
    int sign = 1;
    int i = 0;

    // Maneja signo opcional
    if (s[i] == '-') {
        sign = -1;
        i++;
    }

    for (; s[i] >= '0' && s[i] <= '9'; i++) {
        num = num * 10 + (s[i] - '0');
    }

    return sign * num;
}

// Función para convertir un entero a cadena
void uart_itoa(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0 && i < 14) { // Reserva espacio para el signo y el terminador nulo
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Invierte la cadena
    int start = 0, end = i - 1;
    char temp;
    while (start < end) {
        temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

float uart_atof(const char *s) {
    float result = 0.0f;
    float factor = 1.0f;
    int sign = 1;

    if (*s == '-') {
        sign = -1;
        s++;
    }

    while (*s && *s != '.') {
        result = result * 10.0f + (*s - '0');
        s++;
    }

    if (*s == '.') {
        s++;
        while (*s) {
            factor *= 0.1f;
            result += (*s - '0') * factor;
            s++;
        }
    }

    return sign * result;
}

void ftoa(float n, char *res, int afterpoint) {
    int ipart = (int)n;
    float fpart = n - (float)ipart;

    uart_itoa(ipart, res);

    if (afterpoint > 0) {
        int i = 0;
        while (res[i] != '\0') i++;
        res[i++] = '.';

        for (int j = 0; j < afterpoint; j++) {
            fpart *= 10;
            int digit = (int)fpart;
            res[i++] = digit + '0';
            fpart -= digit;
        }
        res[i] = '\0';
    }
}
