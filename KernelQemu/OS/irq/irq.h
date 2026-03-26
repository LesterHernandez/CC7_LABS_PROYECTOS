#ifndef IRQ_H
#define IRQ_H

// ================= TIMER (SP804) =================
#define TIMER0_BASE   0x10011000

#define TIMER_LOAD    (TIMER0_BASE + 0x00)
#define TIMER_VALUE   (TIMER0_BASE + 0x04)
#define TIMER_CTRL    (TIMER0_BASE + 0x08)
#define TIMER_INTCLR  (TIMER0_BASE + 0x0C)
#define TIMER_MIS     (TIMER0_BASE + 0x14)

// ================= GIC =================
#define GIC_DIST_BASE   0x1E001000
#define GIC_CPU_BASE    0x1E000100

#define GICD_CTLR       (GIC_DIST_BASE + 0x000)
#define GICD_ISENABLER1 (GIC_DIST_BASE + 0x104)

#define GICC_CTLR       (GIC_CPU_BASE + 0x000)
#define GICC_PMR        (GIC_CPU_BASE + 0x004)
#define GICC_IAR        (GIC_CPU_BASE + 0x00C)
#define GICC_EOIR       (GIC_CPU_BASE + 0x010)

// IRQ number del timer en QEMU
#define TIMER_IRQ 34

// ================= FUNCIONES =================
void timer_init(void);
void timer_irq_handler(void);
void enable_irq(void);

#endif