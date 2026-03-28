#include "uart_hw.h"
#include "uart.h"


extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void uart_write_reg(unsigned int reg, unsigned int val) {
    PUT32(reg, val);
}

unsigned int uart_read_reg(unsigned int reg) {
    return GET32(reg);
}