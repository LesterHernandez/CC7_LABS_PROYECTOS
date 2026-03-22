#include "os.h"
#include "stdio.h"
#include "uart.h"

// Referencias a los stacks calculados en memmap.ld
extern unsigned int p1_stack_top;
extern unsigned int p2_stack_top;


// Arreglo para rastrear tus 2 procesos
PCB procesos[2];
int current_process = 0; // 0 para proceso1, 1 para proceso2

extern void PUT32(unsigned int address, unsigned int value); 

// ================= PROTOTIPOS =================
extern void enable_irq(void);
extern void start_first_process(void);
 
// ================= PROCESOS =================
// Cambiado a void para que coincida con la definición en proceso1.c y proceso2.c
extern void proceso1(void);
extern void proceso2(void);

// ================= INIT PROCESSES =================
void init_processes() {
    // -------- P1 --------
    unsigned int *sp1 = (unsigned int *)P1_STACK_TOP;

    *(--sp1) = (unsigned int)proceso1; // PC (retorno a proceso1) 

    // Inicializar r0 a r12 en 0 (13 registros)
    for(int i = 0; i < 13; i++) {
        *(--sp1) = 0;
    }

    *(--sp1) = 0x13; // SPSR: Modo Supervisor (SVC) con interrupciones habilitadas

    procesos[0].sp = (unsigned int)sp1; // Guardar SP en el PCB 

    // -------- P2 --------
    unsigned int *sp2 = (unsigned int *)P2_STACK_TOP;

    *(--sp2) = (unsigned int)proceso2; // PC (retorno a proceso2) 

    // Inicializar r0 a r12 en 0
    for(int i = 0; i < 13; i++) {
        *(--sp2) = 0;
    }

    *(--sp2) = 0x13; // SPSR inicial

    procesos[1].sp = (unsigned int)sp2; // Guardar SP en el PCB 
}

// ================= TIMER =================
void timer_init(void) {
    // Activar clock Timer2
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    // Habilitar IRQ 68 [cite: 86]
    PUT32(INTC_MIR_CLEAR2, 1 << 4);
    PUT32(INTC_ILR68, 0x0);

    // Detener timer
    PUT32(TCLR, 0x0);

    // Limpiar interrupciones
    PUT32(TISR, 0x7);

    // Configurar periodo (~1s según PDF o 2s según tu lógica) [cite: 33]
    PUT32(TLDR, 0xFE91CA00);
    PUT32(TCRR, 0xFE91CA00);

    // Habilitar interrupción de overflow [cite: 85]
    PUT32(TIER, 0x2);

    // Iniciar timer con auto-reload [cite: 85]
    PUT32(TCLR, 0x3);

    PRINT("Timer inicializado\r\n");
}

// ================= IRQ HANDLER (C) =================
__attribute__((naked)) void timer_irq_handler(void) {
    // 1GUARDAR CONTEXTO DEL PROCESO ACTUAL
    asm volatile (
        "stmfd sp!, {r0-r12, lr}\n\t" 
        "mrs r0, spsr\n\t"            
        "stmfd sp!, {r0}\n\t"         
    );

    // 2Guardar el Stack Pointer (SP) actual
    asm volatile ("str sp, %0" : "=m" (procesos[current_process].sp));

    //  Cambiar de proceso (Round-Robin) [cite: 58]
    current_process = (current_process == 0) ? 1 : 0;

    // 4 Limpiar interrupción del timer y del INTC [cite: 98, 99]
    PUT32(TISR, 0x2);         
    PUT32(INTC_CONTROL, 0x1); 

    // 5 RESTAURAR CONTEXTO DEL NUEVO PROCESO [cite: 100]
    asm volatile ("ldr sp, %0" : : "m" (procesos[current_process].sp));
    
    asm volatile (
        "ldmfd sp!, {r0}\n\t"         
        "msr spsr_cxsf, r0\n\t"       
        "ldmfd sp!, {r0-r12, pc}^\n\t" // 
    );
}

// ================= MAIN =================
int main(void) {
    PRINT("OS iniciado\r\n");

    init_processes();
    timer_init();
    enable_irq();
 
    start_first_process();

    while(1); // Nunca debería llegar aquí
    return 0;
}

//mezclar el lab 3 con el lab 4, el lab 3 es el proceso1 y el proceso2, el lab 4 es el timer y el cambio de contexto, entonces en este codigo se esta haciendo todo eso, se esta inicializando los procesos, se esta inicializando el timer, se esta manejando la interrupcion del timer para hacer el cambio de contexto entre los procesos.
//pcb solo tiene que sp, agregar la direccion para poder regresar al proceso, el timer se esta configurando para generar una interrupcion cada cierto tiempo, y en la interrupcion se esta guardando el contexto del proceso actual, cambiando al siguiente proceso, limpiando la interrupcion y restaurando el contexto del nuevo proceso.
