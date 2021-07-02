#include "devices/timer.h"
#include "cpu/cpu.h"
#include "kernel/logging.h"
#include "cpu/irq.h"

uint ticks = 0;

esp_t timer_handler(esp_t esp, context_t *context)
{
    UNUSED(context);
    ticks++;
    return esp;
}

/* --- Public Functions ----------------------------------------------------- */

void timer_setup()
{
    timer_set_frequency(1000);
    irq_register(0, (irq_handler_t)&timer_handler);
}

void timer_set_frequency(int hz)
{
    u32 divisor = 1193180 / hz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);

    info("Timer frequency is %dhz.", hz);
}

u32 timer_get_ticks()
{
    return ticks;
}
