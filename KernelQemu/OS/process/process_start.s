.section .text
.syntax unified
.code 32

.global start_first_process
.extern procesos

start_first_process:
    ldr r0, =procesos
    ldr sp, [r0]

    ldmfd sp!, {r0-r12, lr}
    rfeia sp!
    