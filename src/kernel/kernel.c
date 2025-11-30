#include "drivers/screen.h"
#include "idt.h"
#include "isr.h"
#include "util.h"
#include "pit.h"
#include "drivers/kbd.h"

#include "programs/gameoflife/gol.h"

#define SEED 0xBADFACE
#define ON '*'
#define OFF ' '

void main() {
	SCREEN_INIT();
	IDT_INIT();

	__asm__ volatile("sti");

	PIT_INIT(1000);
	KBD_INIT();

	PROGRAM_GOL_MAIN();

}


