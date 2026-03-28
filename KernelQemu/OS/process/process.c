#include "process.h"

// Definicion del arreglo de procesos
PCB procesos[2];

// Prototipos de procesos
extern void proceso1(void);
extern void proceso2(void);

// Stacks definidos en el linker
extern unsigned int p1_stack_top;
extern unsigned int p2_stack_top;


// ================= INIT PROCESSES =================
void init_processes() {
    // -------- P1 (Letras) --------
    unsigned int *sp1 = (unsigned int *)&p1_stack_top;
    *(--sp1) = 0x13;                   // SPSR PRIMERO (Queda en la parte alta)
    *(--sp1) = (unsigned int)proceso1; // PC DESPUÉS (Queda en la parte baja apuntada por SP)
    *(--sp1) = 0;                      // LR_svc (Link Register del proceso)
    for(int i = 0; i < 13; i++) { *(--sp1) = 0; } // Registros r12 a r0
    procesos[0].sp = (unsigned int)sp1; 

    // -------- P2 (Números) --------
    unsigned int *sp2 = (unsigned int *)&p2_stack_top;
    *(--sp2) = 0x13; 
    *(--sp2) = (unsigned int)proceso2; 
    *(--sp2) = 0; 
    for(int i = 0; i < 13; i++) { *(--sp2) = 0; }
    procesos[1].sp = (unsigned int)sp2; 
}