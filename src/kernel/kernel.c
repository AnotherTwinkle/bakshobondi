#include "../drivers/screen.h"
#include "idt.h"
#include "isr.h"
#include "util.h"
#include "pit.h"

char *hexstr(int num);
void main() {
	SCREEN_INIT();
	IDT_INIT();
	PIT_INIT(1000);
	KBD_INIT();

	asm volatile ("sti");
}
