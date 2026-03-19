
#include "stdio.h"
#include "uart.h"

// ================== PROTOTIPOS ==================
extern void enable_irq(void);

// ================== TIMER ==================

void timer_init(void) {

    // 1. Activar clock del Timer2
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    // 2. Configurar INTC (interrupciones)
    PUT32(INTC_MIR_CLEAR2, 1 << 4);   // habilitar IRQ 68 (Timer2)
    PUT32(INTC_ILR68, 0x0);           // prioridad 0

    // 3. Detener timer
    PUT32(TCLR, 0x0);

    // 4. Limpiar interrupciones pendientes
    PUT32(TISR, 0x7);

    // 5. Configurar periodo (~2 segundos)
    PUT32(TLDR, 0xFE91CA00);
    PUT32(TCRR, 0xFE91CA00);

    // 6. Habilitar interrupción por overflow
    PUT32(TIER, 0x2);

    // 7. Iniciar timer (auto-reload)
    PUT32(TCLR, 0x3);

    PRINT("Timer inicializado\r\n");
}

// ================== HANDLER ==================

void timer_irq_handler(void) {

    // 1. Limpiar interrupción del timer
    PUT32(TISR, 0x2);

    // 2. Notificar al INTC
    PUT32(INTC_CONTROL, 0x1);

    // 3. DEBUG (luego lo quitarás)
    PRINT("Tick\r\n");

    // Aquí irá:
    // - scheduler
    // - context switch
}

// ================== MAIN (KERNEL) ==================

int main(void) {

    PRINT("OS iniciado\r\n");

    // Inicializar timer
    timer_init();

    // Habilitar interrupciones globales
    enable_irq();

    PRINT("Interrupciones habilitadas\r\n");

    //  IMPORTANTE:
    // El OS no hace nada más
    while (1) {
        // Idle loop
    }

    return 0;
}