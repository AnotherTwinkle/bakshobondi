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

    if (color == EMPTY_PIXEL) {
    	return;
    }

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

void pml_draw_rect_ca(u32 x, u32 y, u32 w, u32 h, u8 color) {
	u32 adj_x = x - w/2;
	u32 adj_y = y - h/2;

	pml_draw_rect(adj_x, adj_y, w, h, color);
}

void pml_draw_sprite(SpriteSheet *sheet, u32 idx, u32 x, u32 y, u32 scale) {
	u32 width = sheet->width;
	u32 height = sheet->height;
	u32 unit_width = sheet->unit_width;
	u32 unit_height = sheet->unit_height;

	u32 sprite_x = (idx % (width/unit_width)) * unit_width;
	u32 sprite_y = (idx / (width/unit_width)) * unit_height;

	int sx = sprite_x;
	for (int xx = x; xx < (x + unit_width * scale); xx += scale, sx++) {
		for (int yy = y, sy = sprite_y; yy < (y + unit_height * scale); yy += scale, sy++) {
			u8 color = sheet->data[sy * width + sx];
			pml_draw_rect(xx, yy, scale, scale, color);

			/*
			if (xx == (x + (unit_width-1) * scale) || yy == (y + (unit_height-1) * scale) ||
				xx == x || yy == y) pml_draw_rect(xx, yy, scale, scale, BLACK);
			*/
		}
	}
}

void pml_draw_sprite_ca(SpriteSheet *sheet, u32 idx, u32 x, u32 y, u32 scale) {
    u32 unit_width = sheet->unit_width;
    u32 unit_height = sheet->unit_height;

    // Adjust x and y to the top-left corner for centering
    u32 adj_x = x - (unit_width * scale) / 2;
    u32 adj_y = y - (unit_height * scale) / 2;

    pml_draw_sprite(sheet, idx, adj_x, adj_y, scale);
}
