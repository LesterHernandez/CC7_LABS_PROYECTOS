#include "irq.h"
#include "scheduler.h"
#include "process.h"

// Funcion de bajo nivel
extern void PUT32(unsigned int address, unsigned int value);

// ================= TIMER INIT =================
void timer_init(void) {

    // Activar clock Timer2
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    // Habilitar IRQ 68
    PUT32(INTC_MIR_CLEAR2, 1 << 4);
    PUT32(INTC_ILR68, 0x0);

    // Detener timer
    PUT32(TCLR, 0x0);

    // Limpiar interrupciones
    PUT32(TISR, 0x7);

    // Configurar periodo
    PUT32(TLDR, 0xFE91CA00);
    PUT32(TCRR, 0xFE91CA00);

    // Habilitar interrupcion de overflow
    PUT32(TIER, 0x2);

    // Iniciar timer con auto-reload
    PUT32(TCLR, 0x3);
}

// ================= IRQ HANDLER =================
__attribute__((naked)) void timer_irq_handler(void) {

    // 1. Guardar contexto
    asm volatile (
        "stmfd sp!, {r0-r12, lr}\n\t"
        "mrs r0, spsr\n\t"
        "stmfd sp!, {r0}\n\t"
    );

    // 2. Guardar SP actual
    asm volatile ("str sp, %0" : "=m" (procesos[current_process].sp));

    // 3. Llamar al scheduler
    schedule_next();

    // 4. Limpiar interrupcion
    PUT32(TISR, 0x2);
    PUT32(INTC_CONTROL, 0x1);

    // 5. Restaurar contexto del nuevo proceso
    asm volatile ("ldr sp, %0" : : "m" (procesos[current_process].sp));

    asm volatile (
        "ldmfd sp!, {r0}\n\t"
        "msr spsr_cxsf, r0\n\t"
        "ldmfd sp!, {r0-r12, pc}^\n\t"
    );
}