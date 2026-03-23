#ifndef PROCESS_H
#define PROCESS_H

#include "pcb.h"

// Arreglo global de procesos
extern PCB procesos[2];

// Inicializa los procesos (stacks y contexto inicial)
void init_processes(void);

#endif
