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

// Usaremos un arreglo global en lugar de p1 y p2 
extern PCB procesos[2];
extern int current_process;

// ================= FUNCIONES =================
void init_processes(void);
void timer_init(void);
void timer_irq_handler(void);

#endif