.section .vectors, "ax"
.syntax unified
.code 32
.global _start

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


.section .text
.global reset_handler
reset_handler:

    /* ===== Configurar stack IRQ ===== */
    cps #0x12
    ldr sp, =_irq_stack_top

    /* ===== Configurar stack SVC ===== */
    cps #0x13
    ldr sp, =_svc_stack_top

    /* ===== Configurar VBAR ===== */
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0

    /* ===== Limpiar BSS ===== */
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0

clear_bss:
    cmp r0, r1
    strlt r2, [r0], #4
    blt clear_bss

    /* ===== Ir a main ===== */
    bl main

hang:
    b hang


/* ===== EXCEPCIONES ===== */
undefined_handler: b hang
swi_handler:       b hang
prefetch_handler:  b hang
data_handler:      b hang
fiq_handler:       b hang


/* ===== IRQ WRAPPER ===== */
.global irq_handler
.extern timer_irq_handler

irq_handler:
    sub lr, lr, #4
    b timer_irq_handler


/* ===== ARRANQUE PRIMER PROCESO ===== */
.global start_first_process
.extern procesos

start_first_process:
    ldr r0, =procesos
    ldr sp, [r0]

    ldmfd sp!, {r0}
    msr spsr_cxsf, r0

    ldmfd sp!, {r0-r12, pc}^


/* ===== FUNCIONES BAJO NIVEL ===== */
.global PUT32
PUT32:
    str r1, [r0]
    bx lr

.global GET32
GET32:
    ldr r0, [r0]
    bx lr


/* ===== HABILITAR IRQ ===== */
.global enable_irq
enable_irq:
    cpsie i
    bx lr
