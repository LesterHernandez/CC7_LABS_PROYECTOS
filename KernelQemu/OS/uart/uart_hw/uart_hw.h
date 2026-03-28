#ifndef UART_HW_H
#define UART_HW_H

void uart_write_reg(unsigned int reg, unsigned int val);
unsigned int uart_read_reg(unsigned int reg);

#endif