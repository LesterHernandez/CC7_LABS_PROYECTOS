ENTRY(_start)

/* ========================================================
   QEMU vexpress-a9 MEMORY MAP
   La RAM empieza en 0x60000000
======================================================== */
MEMORY
{
  OS_RAM (rwx) : ORIGIN = 0x60000000, LENGTH = 64K
  P1_RAM (rwx) : ORIGIN = 0x60010000, LENGTH = 64K
  P2_RAM (rwx) : ORIGIN = 0x60020000, LENGTH = 64K
}

SECTIONS
{
  /* ================= SECCIÓN DEL SISTEMA OPERATIVO ================= */
  .os_text : {
    KEEP(*(.vectors))      /* La tabla de interrupciones DEBE ir aquí al inicio */
    *os.o(.text*)          /* Todo el código C y ASM del OS */
    *startup.o(.text*)
    *stdio.o(.text*)
    *string.o(.text*)
    *uart.o(.text*)
    *(.rodata*)            /* Textos y constantes (como los PRINT) */
  } > OS_RAM

  .os_data : { *(.data*) } > OS_RAM
  
  .os_bss : {
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    __bss_end__ = .;
  } > OS_RAM

  /* ================= SECCIÓN DEL PROCESO 1 ================= */
  .p1_text : {
    *proceso1.o(.text*)
    *proceso1.o(.data*)
    *proceso1.o(.bss*)
  } > P1_RAM

  /* ================= SECCIÓN DEL PROCESO 2 ================= */
  .p2_text : {
    *proceso2.o(.text*)
    *proceso2.o(.data*)
    *proceso2.o(.bss*)
  } > P2_RAM
}

/* Calculamos automáticamente dónde empieza la pila (Stack) de cada uno */
_stack_top    = ORIGIN(OS_RAM) + LENGTH(OS_RAM);
p1_stack_top  = ORIGIN(P1_RAM) + LENGTH(P1_RAM);
p2_stack_top  = ORIGIN(P2_RAM) + LENGTH(P2_RAM);