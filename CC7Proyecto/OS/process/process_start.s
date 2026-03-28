.section .text
.syntax unified
.code 32

.globl start_first_process
.extern procesos

start_first_process:
    ldr r0, =procesos
    ldr sp, [r0]

    // Restaurar SPSR (modo usuario, CPSR previo)
    ldmfd sp!, {r0}
    msr spsr_cxsf, r0

    // Restaurar contexto y saltar
    ldmfd sp!, {r0-r12, pc}^
    