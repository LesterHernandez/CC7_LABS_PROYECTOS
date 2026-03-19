#ifndef OS_H
#define OS_H

// ================= STACK =================
#define STACK_SIZE 1024

#define P1_STACK_TOP 0x82110000
#define P2_STACK_TOP 0x82210000
// ================= PCB =================
typedef struct {
    unsigned int sp;
} PCB;

extern PCB p1, p2;
extern PCB *current;

// ================= FUNCIONES =================
void init_processes(void);
PCB* next_process(void);

void timer_init(void);
void timer_irq_handler(void);

#endif