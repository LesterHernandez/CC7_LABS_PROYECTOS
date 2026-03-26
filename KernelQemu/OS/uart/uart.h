#ifndef UART_H
#define UART_H

// ============================================================================
// UART para QEMU vexpress-a9 (PL011)
// ============================================================================

// Base de la UART0 en QEMU
#define UART0_BASE        0x10009000

// Registros (offsets)
#define UART_DR           (UART0_BASE + 0x00)  // Data Register
#define UART_FR           (UART0_BASE + 0x18)  // Flag Register

// Bits del Flag Register (FR)
#define UART_FR_TXFF      (1 << 5)  // Transmit FIFO Full
#define UART_FR_RXFE      (1 << 4)  // Receive FIFO Empty

#endif