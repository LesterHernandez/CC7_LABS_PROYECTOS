ENTRY(_start)

/* ================= MEMORIA ================= */
MEMORY
{
  OS_RAM (rwx) : ORIGIN = 0x60010000, LENGTH = 64K
  P1_RAM (rwx) : ORIGIN = 0x60020000, LENGTH = 64K
  P2_RAM (rwx) : ORIGIN = 0x60030000, LENGTH = 64K
}

/* ================= SECCIONES ================= */
SECTIONS
{
  /* ===== SISTEMA OPERATIVO ===== */
  . = ORIGIN(OS_RAM);

  .vectors : {
    . = ALIGN(32);
    KEEP(*(.vectors))
  } > OS_RAM

  .os_text : {
    . = ALIGN(4);
    *startup.o(.text*)
    *irq.o(.text*)
    *scheduler.o(.text*)
    *process.o(.text*)
    *kernel.o(.text*)
    *uart.o(.text*)
    *stdio.o(.text*)
    *string.o(.text*)
    *(.rodata*)
    . = ALIGN(4);
  } > OS_RAM

  .os_data : {
    . = ALIGN(4);
    *(.data*)
    . = ALIGN(4);
  } > OS_RAM

  .os_bss : {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    __bss_end__ = .;
    . = ALIGN(4);
  } > OS_RAM

  /* ===== STACKS ===== */
  . = ALIGN(8);
  _svc_stack_top = ORIGIN(OS_RAM) + LENGTH(OS_RAM);
  _irq_stack_top = _svc_stack_top - 0x1000;

  /* ===== PROCESO 1 ===== */
  . = ORIGIN(P1_RAM);

  .p1_text : {
    . = ALIGN(4);
    *proceso1.o(.text*)
  } > P1_RAM

  .p1_data : {
    . = ALIGN(4);
    *proceso1.o(.data*)
    *proceso1.o(.bss*)
  } > P1_RAM

  p1_stack_top = ORIGIN(P1_RAM) + LENGTH(P1_RAM);

  /* ===== PROCESO 2 ===== */
  . = ORIGIN(P2_RAM);

  .p2_text : {
    . = ALIGN(4);
    *proceso2.o(.text*)
  } > P2_RAM

  .p2_data : {
    . = ALIGN(4);
    *proceso2.o(.data*)
    *proceso2.o(.bss*)
  } > P2_RAM

  p2_stack_top = ORIGIN(P2_RAM) + LENGTH(P2_RAM);
}