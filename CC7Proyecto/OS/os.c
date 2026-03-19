
#include "os.h"
#include "stdio.h"
#include "uart.h"

// ================= PROTOTIPOS =================
extern void enable_irq(void);
extern void start_first_process(void);
 

// ================= PCB =================
PCB p1, p2;
PCB *current;

// ================= PROCESOS =================
extern int proceso1(void);
extern int proceso2(void);

// ================= INIT PROCESSES =================
void init_processes() {

    // -------- P1 --------
    unsigned int *sp1 = (unsigned int *)P1_STACK_TOP;

    *(--sp1) = (unsigned int)proceso1; // LR

    for(int i = 12; i >= 0; i--) {
        *(--sp1) = 0;
    }

    p1.sp = (unsigned int)sp1;

    // -------- P2 --------
    unsigned int *sp2 = (unsigned int *)P2_STACK_TOP;

    *(--sp2) = (unsigned int)proceso2;

    for(int i = 12; i >= 0; i--) {
        *(--sp2) = 0;
    }

    p2.sp = (unsigned int)sp2;

    current = &p1;
}

// ================= SCHEDULER =================
PCB* next_process() {
    if (current == &p1)
        return &p2;
    else
        return &p1;
}

// ================= TIMER =================
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

    // Configurar periodo (~2s)
    PUT32(TLDR, 0xFE91CA00);
    PUT32(TCRR, 0xFE91CA00);

    // Habilitar interrupción
    PUT32(TIER, 0x2);

    // Iniciar timer
    PUT32(TCLR, 0x3);

    PRINT("Timer inicializado\r\n");
}

// ================= IRQ HANDLER (C) =================
void timer_irq_handler(void) {

    // Limpiar interrupción
    PUT32(TISR, 0x2);

    // Notificar INTC
    PUT32(INTC_CONTROL, 0x1);

    // Cambiar proceso
    current = next_process();
}

// ================= MAIN =================
int main(void) {

    PRINT("OS iniciado\r\n");

    init_processes();
    timer_init();
    enable_irq();
 
    start_first_process();

    while(1); // nunca debería regresar

    return 0;
}