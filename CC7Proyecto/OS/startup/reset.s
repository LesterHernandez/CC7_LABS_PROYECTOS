.section .text
.syntax unified
.code 32

.globl reset_handler
.extern main
.extern vector_table
.extern __bss_start__
.extern __bss_end__

reset_handler:
    ldr sp, =_stack_top

    // Configurar VBAR
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0

    // Limpiar BSS
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0

clear_bss:
    cmp r0, r1
    strlt r2, [r0], #4
    blt clear_bss

    // Ir a C
    bl main

hang:
    b hang
    