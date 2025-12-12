#include "kernel/util.h"
#include "kernel/pit.h"

#include "drivers/kbd.h"
#include "drivers/kbdmap.h"
#include "drivers/screen.h"

#include "graphics/pomelo.h"

#include "sprites.h"
#include "cat.h"
#include "main.h"

static u32 SEED  = 0xFACEDAB;

static u8 arr_cur_buffer[SCREEN_SIZE];
static u8 arr_next_buffer[SCREEN_SIZE];

static u8 *cur_buffer = arr_cur_buffer;
static u8 *next_buffer = arr_next_buffer;

static int KBD_SUB_ID;

static void SWAP() {
	u8 *tmp = cur_buffer;
	cur_buffer = next_buffer;
	next_buffer = tmp;

	pml_setbuffer(next_buffer, SCREEN_WIDTH, SCREEN_HEIGHT);

	memcpy(SCREEN_ADDR, cur_buffer, SCREEN_SIZE);
}

void PROGRAM_CAT_MAIN() {
	srand(SEED + get_ticks());

	KBD_SUB_ID = kbd_queue_subscribe(&kbd_queue);
	KeyEvent event;

	pml_setbuffer(next_buffer, SCREEN_WIDTH, SCREEN_HEIGHT);

	init_sprites();

	AnimState cat_anim_state;
	SpriteSheet* cat_sprites[4] = {&cat0_sprites, &cat1_sprites,
								   &cat2_sprites, &cat3_sprites};
	Cat cats[4];
	for (int i = 0; i < 4; i++) {
		cats[i].posx = 100;
		cats[i].posy = 40 + i * 32;
		cats[i].type = 0;
		cats[i].state = WALKING;
		cats[i].orientation = (i % 2 ? FACING_LEFT : FACING_RIGHT);
		cats[i].spritesheet = cat_sprites[i];

		cats[i].anim_state = cat_anim_state;
		set_anim(&cats[i].anim_state, 
			(cats[i].orientation == FACING_LEFT) ?
			&anim_walking_left :
			&anim_walking_right
			);
	}

	while(1) {
		// Update
		for (int i = 0; i < 4; i++) {
			cat_update(&cats[i]);
		}

		// RENDERING
		pml_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff);
		for (int x = 0; x < SCREEN_WIDTH; x += SCALE * 16) {
			for (int y = 0; y < SCREEN_HEIGHT; y += SCALE * 16) {
				pml_draw_sprite(&level0_sprites, 16, x, y, SCALE);
				// pml_draw_rect(x, y, 16*);
			}
		}

		// int x[] = {5, 8, 5, 4, 13, 3};
		// int y[] = {18, 0, 13, 1, 5, 6};
		// for (int i = 0; i < 6; i++ ) {
		// 	pml_draw_sprite(&level0_sprites, (i%2)+1, x[i]*16, y[i]*16, SCALE);
		// }

		for (int i = 0; i < 4; i++) {
			draw_cat(&cats[i], SCALE);	
		}
		

		// for (int i = 0; i < cats[i].anim_state.frame; i++) {
		// 	pml_draw_rect(4*i, 0, 2, 2, 0xff);
		// }

		// for (int i = 0; i < cats[i].anim_state.anim->length; i++) {
		// 	pml_draw_rect(4*i, 8, 2, 2, 0xff);
		// }

		// for (int i = 0; i < cats[i].state ; i++) {
		// 	pml_draw_rect(4*i, 16, 2, 2, 0xff);
		// }

		// for (int i = 0; i < cats[i].anim_state.looping_for ; i++) {
		// 	pml_draw_rect(4*i, 24, 2, 2, 0xff);
		// }

		sleep(10);
		SWAP();
	}
}
