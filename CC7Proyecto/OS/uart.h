
#ifndef UART_H
#define UART_H


// ============================================================================
// Direcciones base de perifericos en BeagleBone Black
// ============================================================================

// Direccion base del modulo UART0
#define UART0_BASE     0x44E09000
#define UART_THR       (UART0_BASE + 0x00)  // Registro de transmision
#define UART_LSR       (UART0_BASE + 0x14)  // Registro de estado
#define UART_LSR_THRE  0x20                 // Bit: Transmit Holding Register Empty
#define UART_LSR_RXFE  0x10                 // Bit: Receive FIFO Empty
#define UART_LSR_DR 0x01
// Direccion base del DMTIMER2
#define DMTIMER2_BASE    0x48040000
#define TCLR             (DMTIMER2_BASE + 0x38)  // Registro de control del timer
#define TCRR             (DMTIMER2_BASE + 0x3C)  // Registro contador
#define TISR             (DMTIMER2_BASE + 0x28)  // Registro de estado de interrupcion
#define TIER             (DMTIMER2_BASE + 0x2C)  // Registro de habilitacion de interrupcion
#define TLDR             (DMTIMER2_BASE + 0x40)  // Registro de carga

// Direccion base del controlador de interrupciones INTCPS
#define INTCPS_BASE      0x48200000
#define INTC_MIR_CLEAR2  (INTCPS_BASE + 0xC8)   // Registro para desenmascarar interrupciones
#define INTC_CONTROL     (INTCPS_BASE + 0x48)   // Registro de control del INTC
#define INTC_ILR68       (INTCPS_BASE + 0x110)  // Registro de configuracion para IRQ 68

// Direccion base del modulo de control de reloj
#define CM_PER_BASE      0x44E00000
#define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE + 0x80)  // Control de reloj para Timer2

 

extern volatile unsigned int * const UART0;

void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_gets_input(char *buffer, int max_length);

int uart_atoi(const char *s);
void uart_itoa(int num, char *buffer);
float uart_atof(const char *s);  
void ftoa(float n, char *res, int afterpoint);



#endif // UART_H
