#ifndef UART_H
#define UART_H

// ============================================================================
// Direcciones base para QEMU (vexpress-a9)
// UART: PL011
// ============================================================================

#define UART0_BASE     0x10009000
#define UART_DR        (UART0_BASE + 0x00) // Data Register (TX y RX)
#define UART_FR        (UART0_BASE + 0x18) // Flag Register

#define UART_FR_TXFF   0x20 // Bit 5: Transmit FIFO Full
#define UART_FR_RXFE   0x10 // Bit 4: Receive FIFO Empty

// Dejamos las de Timer temporalmente (las cambiaremos en la fase 2)
#define DMTIMER2_BASE    0x48040000
#define TCLR             (DMTIMER2_BASE + 0x38)
#define TCRR             (DMTIMER2_BASE + 0x3C)
#define TISR             (DMTIMER2_BASE + 0x28)
#define TIER             (DMTIMER2_BASE + 0x2C)
#define TLDR             (DMTIMER2_BASE + 0x40)

#define INTCPS_BASE      0x48200000
#define INTC_MIR_CLEAR2  (INTCPS_BASE + 0xC8)
#define INTC_CONTROL     (INTCPS_BASE + 0x48)
#define INTC_ILR68       (INTCPS_BASE + 0x110)

#define CM_PER_BASE      0x44E00000
#define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE + 0x80)

// ================= PROTOTIPOS =================
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_gets_input(char *buffer, int max_length);

int uart_atoi(const char *s);
void uart_itoa(int num, char *buffer);
float uart_atof(const char *s);  
void ftoa(float n, char *res, int afterpoint);

#endif // UART_H