#include "process.h"

// Definicion del arreglo de procesos
PCB procesos[2];

// Prototipos de procesos
extern void proceso1(void);
extern void proceso2(void);

// Direcciones de stack
#define P1_STACK_TOP 0x82110000
#define P2_STACK_TOP 0x82210000

// ================= INIT PROCESSES =================
void init_processes(void) {

    // -------- Proceso 1 --------
    unsigned int *sp1 = (unsigned int *)P1_STACK_TOP;

    *(--sp1) = (unsigned int)proceso1; // PC inicial

    // Inicializar r0-r12 en 0
    for (int i = 0; i < 13; i++) {
        *(--sp1) = 0;
    }

    *(--sp1) = 0x13; // SPSR (modo SVC)

    procesos[0].sp = (unsigned int)sp1;

    // -------- Proceso 2 --------
    unsigned int *sp2 = (unsigned int *)P2_STACK_TOP;

    *(--sp2) = (unsigned int)proceso2;

    for (int i = 0; i < 13; i++) {
        *(--sp2) = 0;
    }

    *(--sp2) = 0x13;

    procesos[1].sp = (unsigned int)sp2;
}