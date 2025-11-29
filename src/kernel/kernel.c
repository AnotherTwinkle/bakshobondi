#include "../drivers/screen.h"
#include "idt.h"
#include "isr.h"
#include "util.h"
#include "pit.h"

#define SEED 0xBADFACE
#define ON '*'
#define OFF ' '

char ibuf[25][80];

void main() {
	SCREEN_INIT();
	IDT_INIT();

	__asm__ volatile("sti");

	PIT_INIT(1000);

	srand(SEED + get_ticks());

	for (int r = 0; r < 25; r++) {
		for (int c = 0; c < 80; c++) {
			ibuf[r][c] = (rand() % 100 < 30) ? ON : OFF;
		}
	}

	flush(&ibuf);
	while(1) {
		sleep(500);
		conway_step();
		flush(&ibuf);
	}
}


int ia(int r, int c) {
	return r >= 0 && r < 25 && c >= 0 && c < 80 
	       && ibuf[r][c] == ON;
}

void conway_step() {
	char next[25][80];
	memcpy(next, ibuf, 25*80);

	for (int r = 0; r < 25; r++) {
	    for (int c = 0; c < 80; c++) {
	        int neigh = ia(r + 1, c - 1) + ia(r + 1, c) + ia(r + 1, c + 1)
	                  + ia(r, c - 1) + ia(r, c + 1)
	                  + ia(r - 1, c - 1) + ia(r - 1, c) + ia(r - 1, c + 1);

	        if (ibuf[r][c] == OFF) {
	            if (neigh == 3) {
	                next[r][c] = ON;
	            } else {
	                next[r][c] = OFF;
	            }
	        } else {
	            if (neigh < 2 || neigh > 3) {
	                next[r][c] = OFF;
	            } else {
	                next[r][c] = ON;
	            }
	        }
	    }
	}
	memcpy(ibuf, next,  25*80);
}