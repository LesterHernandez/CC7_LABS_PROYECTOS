.section .vectors, "ax"
.syntax unified
.code 32
.globl _start

// ================= VECTOR TABLE =================
.align 5
_start:
vector_table:
    b reset_handler
    b undefined_handler
    b swi_handler
    b prefetch_handler
    b data_handler
    b .
    b irq_handler
    b fiq_handler

// ================= RESET =================
.section .text
reset_handler:
    ldr sp, =_stack_top

    // Configurar VBAR (Vector Base Address Register)
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0

    // Limpiar sección BSS
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0

clear_bss:
    cmp r0, r1
    strlt r2, [r0], #4
    blt clear_bss

    // Ir al kernel en C
    bl main

hang:
    b hang

// ================= EXCEPCIONES =================
undefined_handler: b hang
swi_handler:       b hang
prefetch_handler:  b hang
data_handler:      b hang
fiq_handler:       b hang

// ================= VARIABLES EXTERNAS =================
.extern procesos
.extern timer_irq_handler

// ================= IRQ HANDLER =================
irq_handler:
    sub lr, lr, #4
    b timer_irq_handler

// ================= ARRANQUE PRIMER PROCESO =================
.globl start_first_process
start_first_process:
    ldr r0, =procesos
    ldr sp, [r0]

    // Restaurar contexto inicial
    ldmfd sp!, {r0}
    msr spsr_cxsf, r0
    ldmfd sp!, {r0-r12, pc}^

// ================= FUNCIONES BAJO NIVEL =================
.globl PUT32
PUT32:
    str r1, [r0]
    bx lr

.globl GET32
GET32:
    ldr r0, [r0]
    bx lr

// ================= HABILITAR IRQ =================
.globl enable_irq
enable_irq:
    mrs r0, cpsr
    bic r0, r0, #0x80
    msr cpsr_c, r0
    bx lr

// ================= STACK DEL OS =================
.section .bss
.align 4
_stack_bottom:
    .skip 0x2000
_stack_top:
