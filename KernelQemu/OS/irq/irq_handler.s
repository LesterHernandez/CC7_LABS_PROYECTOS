.syntax unified
.code 32

// ================= EXTERNOS =================
.extern timer_irq_handler

// ================= IRQ HANDLER =================
.globl irq_handler
irq_handler:
sub lr, lr, #4          @ Ajustar LR para retorno correcto
b timer_irq_handler     @ Saltar al handler en C
