.section .text
.syntax unified
.code 32

.global irq_handler
.extern timer_irq_handler

irq_handler:
    sub lr, lr, #4
    b timer_irq_handler
    