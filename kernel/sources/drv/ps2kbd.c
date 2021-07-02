#include "devices/kbd.h"
#include "cpu/irq.h"

char *kbd_buffer;
size_t kbd_buffer_size = 0;

esp_t kbd_ird_handler(esp_t esp)
{

    return esp;
}

void kbd_setup()
{
    irq_register(1, (irq_handler_t)&kbd_ird_handler);
}

void kbd_read(char *buffer, size_t size)
{
    kbd_buffer = buffer;
    kbd_buffer_size = size;
}
