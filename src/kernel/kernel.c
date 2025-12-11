#include "idt.h"
#include "isr.h"
#include "util.h"
#include "pit.h"
#include "drivers/kbd.h"
#include "drivers/kbdmap.h"
#include "drivers/screen.h"

#include "graphics/pomelo.h"

#include "programs/gameoflife/gol.h"
#include "programs/kbdtest/kbdtest.h"
#include "programs/cats/main.h"

#define SEED 0xBADFACE

void main() {
	SCREEN_INIT();
	IDT_INIT();

	__asm__ volatile("sti");

	PIT_INIT(1000);
	KBD_INIT();

	srand(SEED + get_ticks());

	PROGRAM_CAT_MAIN();
}