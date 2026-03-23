#ifndef SCHEDULER_H
#define SCHEDULER_H

// Indice del proceso actual
extern int current_process;

// Selecciona el siguiente proceso (Round Robin)
void schedule_next(void);

#endif
