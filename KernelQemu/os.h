#ifndef OS_H
#define OS_H

// ================= STACK =================
#define STACK_SIZE 1024

// Direcciones ajustadas para QEMU
#define P1_STACK_TOP 0x6001FFFF
#define P2_STACK_TOP 0x6002FFFF

// ================= PCB =================
typedef struct {
    unsigned int sp;
} PCB;

// Usaremos un arreglo global en lugar de p1 y p2 
extern PCB procesos[2];
extern volatile int current_process;

// ================= FUNCIONES =================
void init_processes(void);
void timer_init(void);
void timer_irq_handler(void);

#endif