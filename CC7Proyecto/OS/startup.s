.section .text
.syntax unified
.code 32
.globl _start

// ================= VECTOR TABLE =================
.align 5
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
_start:
reset_handler:
    ldr sp, =_stack_top      @ Inicializar stack

    @ Configurar vector table (VBAR)
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0

    @ Llamar a main del OS
    bl main

hang:
    b hang

// ================= EXCEPCIONES =================
undefined_handler: b hang
swi_handler:       b hang
prefetch_handler:  b hang
data_handler:      b hang
fiq_handler:       b hang

// ================= IRQ HANDLER =================
irq_handler:
    sub lr, lr, #4              @ Ajustar retorno
    stmfd sp!, {r0-r12, lr}     @ Guardar contexto

    bl timer_irq_handler        @ C handler (aquí irá scheduler)

    ldmfd sp!, {r0-r12, lr}     @ Restaurar contexto
    subs pc, lr, #0             @ Return from IRQ

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
    bic r0, r0, #0x80   @ limpiar bit I
    msr cpsr_c, r0
    bx lr

// ================= STACK =================
.section .bss
.align 4
_stack_bottom:
    .skip 0x2000   @ 8KB stack
_stack_top: