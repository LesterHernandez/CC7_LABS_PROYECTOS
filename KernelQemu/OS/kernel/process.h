#ifndef PROCESS_H
#define PROCESS_H

#include "pcb.h"

// ================= PROCESOS =================

// Arreglo global de procesos
extern PCB procesos[2];

// ================= STACKS DESDE LINKER =================

// Estas variables vienen del linker (memmap.ld)
extern unsigned int p1_stack_top;
extern unsigned int p2_stack_top;

// ================= FUNCIONES =================

// Inicializa los procesos (stacks y contexto inicial)
void init_processes(void);

#endif