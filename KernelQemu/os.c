#include "os.h"
#include "stdio.h"
#include "uart.h"

extern unsigned int p1_stack_top;
extern unsigned int p2_stack_top;

// ================= GLOBALES =================
PCB procesos[2];
volatile int current_process = 0; 

extern void PUT32(unsigned int address, unsigned int value); 
extern unsigned int GET32(unsigned int address);
extern void enable_irq(void);

extern void proceso1(void);
extern void proceso2(void);

// ================= DIRECCIONES DE HARDWARE =================
#define GIC_CPU_BASE    0x1E000100
#define GICC_CTLR       (GIC_CPU_BASE + 0x00)
#define GICC_PMR        (GIC_CPU_BASE + 0x04)
#define GICC_IAR        (GIC_CPU_BASE + 0x0C)
#define GICC_EOIR       (GIC_CPU_BASE + 0x10)

#define GIC_DIST_BASE   0x1E001000
#define GICD_CTLR       (GIC_DIST_BASE + 0x00)
#define GICD_ISENABLER0 (GIC_DIST_BASE + 0x100)

#define TIMER_BASE      0x1E000600
#define TIMER_LOAD      (TIMER_BASE + 0x00)
#define TIMER_CONTROL   (TIMER_BASE + 0x08)
#define TIMER_INTC      (TIMER_BASE + 0x0C)

// ==========================================================

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

void timer_init(void) {
    PUT32(GICD_CTLR, 1);           
    PUT32(GICC_PMR, 0xFFFF);       
    PUT32(GICC_CTLR, 1);           
    PUT32(GICD_ISENABLER0, (1 << 29));

    PUT32(TIMER_LOAD, 0x000FFFFF); 
    PUT32(TIMER_CONTROL, 0x7);
    PRINT("Timer de QEMU (GIC) inicializado\r\n");
}

// ================= LÓGICA DEL IRQ (C PURO) =================
// 'noinline' asegura que GCC respete esta función y no la fusione con el ensamblador
__attribute__((noinline)) unsigned int timer_irq_logic(unsigned int current_sp) {
    procesos[current_process].sp = current_sp;
    unsigned int irq_id = GET32(GICC_IAR);

    if ((irq_id & 0x3FF) == 29) {
        current_process = (current_process == 0) ? 1 : 0;
        PUT32(TIMER_INTC, 1); // Limpiar timer
    }

    PUT32(GICC_EOIR, irq_id);
    return procesos[current_process].sp;
}

// ================= CONTEXT SWITCH (ARM PROFESIONAL) =================
__attribute__((naked)) void timer_irq_handler(void) {
    // Usamos un solo bloque asm para evitar que GCC destruya la pila
    asm volatile (
        "sub lr, lr, #4\n\t"          // 1. Ajustar el PC de retorno
        "srsdb sp!, #0x13\n\t"        // 2. Guardar PC y SPSR en la pila PRIVADA del proceso (Modo SVC)
        "cpsid i, #0x13\n\t"          // 3. Cambiar a Modo SVC (Usar la pila del proceso)
        "stmfd sp!, {r0-r12, lr}\n\t" // 4. Guardar los registros restantes en la pila del proceso

        "mov r0, sp\n\t"              // 5. Pasar la pila privada actual a C
        "bl timer_irq_logic\n\t"      // 6. C calcula cuál es el siguiente proceso
        "mov sp, r0\n\t"              // 7. Cargar la pila privada del NUEVO proceso

        "ldmfd sp!, {r0-r12, lr}\n\t" // 8. Restaurar registros del nuevo proceso
        "rfeia sp!\n\t"               // 9. Magia ARM: Restaurar PC y Estado, y reanudar ejecución
    );
}

// ================= INICIO DEL OS (REEMPLAZA A STARTUP.S) =================
__attribute__((naked)) void start_os(void) {
    asm volatile (
        "ldr r0, =procesos\n\t"
        "ldr sp, [r0]\n\t"            // Cargar la pila del Proceso 0
        "ldmfd sp!, {r0-r12, lr}\n\t" // Sacar registros
        "rfeia sp!\n\t"               // Iniciar la ejecución
    );
}

// ================= MAIN =================
int main(void) {
    PRINT("\n===================================\r\n");
    PRINT(" OS Iniciado (Arquitectura Aislada)\r\n");
    PRINT("===================================\r\n");

    init_processes();
    timer_init();
    
    enable_irq(); 
 
    start_os(); // Usamos nuestra nueva función a prueba de balas

    while(1); 
    return 0;
}