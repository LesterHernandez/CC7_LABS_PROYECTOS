#ifndef IRQ_H
#define IRQ_H

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
// Inicializacion del timer (genera interrupciones)
void timer_init(void);

// Handler de interrupcion del timer
void timer_irq_handler(void);

// Habilitar IRQ globales (ASM)
void enable_irq(void);

#endif
