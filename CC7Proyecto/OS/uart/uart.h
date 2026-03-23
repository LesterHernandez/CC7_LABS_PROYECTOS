#ifndef UART_H
#define UART_H

// ============================================================================
// Direcciones base de perifericos (BeagleBone Black)
// ============================================================================

// UART0
#define UART0_BASE        0x44E09000

// Registros UART (offsets desde la base)
#define UART_THR          (UART0_BASE + 0x00)  // Transmit Holding Register
#define UART_RHR          (UART0_BASE + 0x00)  // Receive Holding Register
#define UART_LSR          (UART0_BASE + 0x14)  // Line Status Register

// Bits del LSR
#define UART_LSR_THRE     0x20   // Transmit Holding Register Empty
#define UART_LSR_DR       0x01   // Data Ready

#endif
