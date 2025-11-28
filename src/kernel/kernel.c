#include "../drivers/screen.h"
#include "idt.h"
#include "isr.h"
#include "util.h"

char *hexstr(int num);
void main() {
	SCREEN_INIT();
	idt_init();
//	char* r = hexstr(10);
	asm volatile ("int $0x0");

	asm volatile ("int $0x1");

	asm volatile ("int $0x2");
	asm volatile ("int $0x3");
}
