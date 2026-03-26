#include "kernel.h"
#include "process.h"
#include "irq.h"
#include "print.h"

// Funcion ASM para arrancar el primer proceso
extern void start_first_process(void);

int main(void) {

    PRINT("Kernel iniciado\r\n");

    // Inicializar procesos
    init_processes();

    // Inicializar interrupciones (timer)
    timer_init();
    enable_irq();

    // Iniciar ejecucion del primer proceso
    start_first_process();

    while (1); // Nunca deberia regresar aqui

    return 0;
}