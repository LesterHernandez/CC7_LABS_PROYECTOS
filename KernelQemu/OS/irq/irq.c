#include "irq.h"
#include "scheduler.h"
#include "process.h"

// Acceso a memoria
extern void PUT32(unsigned int address, unsigned int value);
extern unsigned int GET32(unsigned int address);

// ================= TIMER INIT =================
void timer_init(void) {

    // -------- Configurar TIMER (SP804) --------
    PUT32(TIMER_CTRL, 0x0);           // Desactivar timer
    PUT32(TIMER_LOAD, 1000000);       // Valor de carga (ajusta velocidad)
    PUT32(TIMER_INTCLR, 1);           // Limpiar interrupción

    // Timer: periodic, enable interrupt, 32-bit, enable
    PUT32(TIMER_CTRL, (1 << 7) | (1 << 6) | (1 << 5) | (1 << 1));

    // -------- Configurar GIC --------
    
    // Habilitar IRQ del timer (IRQ 34)
    PUT32(GICD_ISENABLER1, (1 << (TIMER_IRQ - 32)));

    // Habilitar distribuidor
    PUT32(GICD_CTLR, 1);

    // Habilitar CPU interface
    PUT32(GICC_PMR, 0xFF);  // Permitir todas las prioridades
    PUT32(GICC_CTLR, 1);
}

// ================= IRQ HANDLER =================
__attribute__((naked)) void timer_irq_handler(void) {

    // 1. Guardar contexto
    asm volatile (
        "stmfd sp!, {r0-r12, lr}\n\t"
        "mrs r0, spsr\n\t"
        "stmfd sp!, {r0}\n\t"
    );

    // 2. Guardar SP del proceso actual
    asm volatile ("str sp, %0" : "=m" (procesos[current_process].sp));

    // 3. Scheduler
    schedule_next();

    // 4. Limpiar interrupción del TIMER
    PUT32(TIMER_INTCLR, 1);

    // 5. Notificar fin de IRQ al GIC
    unsigned int irq = GET32(GICC_IAR);
    PUT32(GICC_EOIR, irq);

    // 6. Restaurar SP del nuevo proceso
    asm volatile ("ldr sp, %0" : : "m" (procesos[current_process].sp));

    // 7. Restaurar contexto
    asm volatile (
        "ldmfd sp!, {r0}\n\t"
        "msr spsr_cxsf, r0\n\t"
        "ldmfd sp!, {r0-r12, pc}^\n\t"
    );
}