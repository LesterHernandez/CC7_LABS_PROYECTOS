.section .text
.syntax unified
.code 32

.global reset_handler
.extern main

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
    