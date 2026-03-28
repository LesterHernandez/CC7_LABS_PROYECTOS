.section .text
.syntax unified
.code 32

.global undefined_handler
.global swi_handler
.global prefetch_handler
.global data_handler
.global fiq_handler

undefined_handler: b hang
swi_handler:       b hang
prefetch_handler:  b hang
data_handler:      b hang
fiq_handler:       b hang

hang:
    b hang
    