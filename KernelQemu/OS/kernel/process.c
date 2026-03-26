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
void init_processes(void) {

    // -------- Proceso 1 --------
    unsigned int *sp1 = (unsigned int *)&p1_stack_top;

    // Primero PC
    *(--sp1) = (unsigned int)proceso1;

    // Luego r12 → r0 (en orden inverso)
    *(--sp1) = 0; // r12
    *(--sp1) = 0; // r11
    *(--sp1) = 0; // r10
    *(--sp1) = 0; // r9
    *(--sp1) = 0; // r8
    *(--sp1) = 0; // r7
    *(--sp1) = 0; // r6
    *(--sp1) = 0; // r5
    *(--sp1) = 0; // r4
    *(--sp1) = 0; // r3
    *(--sp1) = 0; // r2
    *(--sp1) = 0; // r1
    *(--sp1) = 0; // r0

    // Finalmente SPSR
    *(--sp1) = 0x13;

    procesos[0].sp = (unsigned int)sp1;


    // -------- Proceso 2 --------
    unsigned int *sp2 = (unsigned int *)&p2_stack_top;

    *(--sp2) = (unsigned int)proceso2;

    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;
    *(--sp2) = 0;

    *(--sp2) = 0x13;

    procesos[1].sp = (unsigned int)sp2;
}