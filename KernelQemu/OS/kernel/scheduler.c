#include "scheduler.h"

// Proceso actual (0 o 1)
int current_process = 0;

// ================= SCHEDULER =================
// Algoritmo Round Robin simple
void schedule_next(void) {
current_process = (current_process == 0) ? 1 : 0;
}
