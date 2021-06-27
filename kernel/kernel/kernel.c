#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	printf("CarrotOS booted!\nThis is a newline test!");
}
