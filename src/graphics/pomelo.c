/*
PomeloGL, Graphics Library for Bakshobondi
Started on : 08/12/2025
*/

#include "pomelo.h"
#include "drivers/screen.h"

u8 *CUR_BUFFER;
u16 BUFFER_WIDTH;
u16 BUFFER_HEIGHT;

void pml_setbuffer(u8 *buff, u16 buff_width, u16 buff_height) {
	CUR_BUFFER = buff;
	BUFFER_WIDTH = buff_width;
	BUFFER_HEIGHT = buff_height;
}


void pml_setpixel(u32 x, u32 y, u8 color) {
    if (x >= BUFFER_WIDTH || y >= BUFFER_HEIGHT)
        return;

    CUR_BUFFER[y * BUFFER_WIDTH + x] = color;
}

u8 pml_getpixel(u32 x, u32 y) {
	if (x > BUFFER_WIDTH || y > BUFFER_HEIGHT) {
		return 0x0;
	}

	return CUR_BUFFER[y * BUFFER_WIDTH + x];
}

void pml_draw_rect(u32 x, u32 y, u32 w, u32 h, u8 color) {
	for (int xx = x; xx < (x + w); xx++) {
		for (int yy = y; yy < (y + h); yy++) {
			pml_setpixel(xx, yy, color);
		}
	}
}
