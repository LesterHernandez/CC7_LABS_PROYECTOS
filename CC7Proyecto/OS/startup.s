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
    ldr sp, =_stack_top      @ Stack del OS

    @ Configurar VBAR (Vector Base Address Register)
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0

    @ Ir al kernel
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
.extern current

// ================= IRQ HANDLER =================
irq_handler:
    sub lr, lr, #4

    @ Guardar contexto actual en stack
    stmfd sp!, {r0-r12, lr}

    @ Guardar SP en PCB actual
    ldr r0, =current
    ldr r1, [r0]
    str sp, [r1]

    @ Llamar handler en C (scheduler)
    bl timer_irq_handler

    @ Cargar nuevo proceso
    ldr r0, =current
    ldr r1, [r0]
    ldr sp, [r1]

    @ Restaurar contexto del nuevo proceso
    ldmfd sp!, {r0-r12, lr}

    @ Retornar de IRQ
    subs pc, lr, #4

// ================= ARRANQUE PRIMER PROCESO =================
.globl start_first_process
start_first_process:
    ldr r0, =current
    ldr r1, [r0]
    ldr sp, [r1]

    ldmfd sp!, {r0-r12, lr}
    mov pc, lr

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
    bic r0, r0, #0x80   @ limpiar bit I (habilitar IRQ)
    msr cpsr_c, r0
    bx lr

// ================= STACK DEL OS =================
.section .bss
.align 4
_stack_bottom:
    .skip 0x2000   @ 8KB stack
_stack_top: