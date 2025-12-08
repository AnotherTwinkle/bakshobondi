#include "idt.h"
#include "isr.h"
#include "util.h"
#include "pit.h"
#include "drivers/kbd.h"
#include "drivers/screen.h"

#include "graphics/pomelo.h"

#include "programs/gameoflife/gol.h"
#include "programs/kbdtest/kbdtest.h"

#define SEED 0xBADFACE

static u8 ibuff[SCREEN_SIZE];
static u8 next[SCREEN_SIZE];

static u8 get_ibuff_pixel(u32 x, u32 y) {
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
		return EMPTY_PIXEL;
	}

	return ibuff[y * SCREEN_WIDTH + x];
}

void cntm(int x, int y) {
	if (get_ibuff_pixel(x, y) != 0x00 && (rand() % 100 < 20)) {
		pml_setpixel(x, y, 0xff);
	}
}

void main() {
	SCREEN_INIT();
	IDT_INIT();

	__asm__ volatile("sti");

	PIT_INIT(1000);
	KBD_INIT();

	srand(SEED + get_ticks());

	pml_setbuffer((u8*)ibuff, SCREEN_WIDTH, SCREEN_HEIGHT);
	pml_draw_rect(20, 30, 100, 75, 0x87);
	pml_draw_rect(50, 50, 100, 75, 0x87);
	pml_draw_rect(60, 70, 200, 25, 0x87);
	pml_draw_rect(200, 90, 75, 50, 0x87);

	pml_draw_rect(50, 50, 4, 4, 0xff);
	pml_draw_rect(250, 100, 4, 4, 0xff);

	memcpy((u8*)next, (u8*)ibuff, SCREEN_SIZE);
	pml_setbuffer((u8*)next, SCREEN_WIDTH, SCREEN_HEIGHT);

	while(1) {
		memcpy((u8*)SCREEN_ADDR, (u8*)ibuff, SCREEN_SIZE);
		// while(1);
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			for (int y = 0; y < SCREEN_HEIGHT; y++) {
				if (get_ibuff_pixel(x, y) != 0xff) continue;
				cntm(x + 1, y);
				cntm(x - 1, y);
				cntm(x, y + 1);
				cntm(x, y - 1);
			}
		}
		memcpy((u8*)ibuff, (u8*)next, SCREEN_SIZE);
		sleep(20);
	}

}


