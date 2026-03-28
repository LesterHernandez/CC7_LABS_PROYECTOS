.section .vectors, "ax"
.syntax unified
.code 32

.global _start
.global vector_table

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
    