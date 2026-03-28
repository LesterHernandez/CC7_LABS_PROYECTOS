.section .text
.syntax unified
.code 32

.globl undefined_handler
.globl swi_handler
.globl prefetch_handler
.globl data_handler
.globl fiq_handler

undefined_handler: b hang
swi_handler:       b hang
prefetch_handler:  b hang
data_handler:      b hang
fiq_handler:       b hang

hang:
    b hang
    