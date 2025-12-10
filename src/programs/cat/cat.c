#include "kernel/util.h"
#include "kernel/pit.h"

#include "drivers/kbd.h"
#include "drivers/kbdmap.h"
#include "drivers/screen.h"

#include "graphics/pomelo.h"

#include "cat_sprites.h"
#include "cat.h"

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

	int CAT_X = 160, CAT_Y = 100;
	double SLEEP_FACTOR =  2;
	int x = 0;

	while(1) {
		u8 result = kbd_dequeue(&kbd_queue, KBD_SUB_ID, &event);

		sleep(400*SLEEP_FACTOR);
		pml_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff);
		pml_draw_sprite_ca(&cat_sprites, 28 + x, CAT_X, CAT_Y, 4);
		x ^= 1;

		SWAP();
	}
}

// void TEST_SPRITESHEET() {
// 	pml_setbuffer((u8*)next, SCREEN_WIDTH, SCREEN_HEIGHT);
// 	pml_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff);

// 	int x = 0;
// 	while(1) {
// 		pml_draw_rect_ca(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 		// pml_draw_sprite_ca(&cat_sprites, 28 + x, CAT_X, CAT_Y, 4);
// 		pml_draw_sprite_ca(&cat_sprites, 28 + x, CAT_X, CAT_Y, 4);

// 		memcpy((u8*)SCREEN_ADDR, (u8*)next, SCREEN_SIZE);
// 		sleep(400*SLEEP_FACTOR);
// 		x ^= 1;
// 	}

// 	pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 	pml_draw_sprite(&cat_sprites, 18, CAT_X, CAT_Y, 4);
// 	sleep(600 * SLEEP_FACTOR);
// 	while(1) {

// 		pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 		pml_draw_sprite(&cat_sprites, 19, CAT_X, CAT_Y, 4);
// 		sleep(300 * SLEEP_FACTOR);

// 		pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 		pml_draw_sprite(&cat_sprites, 20, CAT_X, CAT_Y, 4);
// 		sleep(200 * SLEEP_FACTOR);

// 		pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 		pml_draw_sprite(&cat_sprites, 21, CAT_X, CAT_Y, 4);
// 		sleep(200 * SLEEP_FACTOR);

// 		pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 		pml_draw_sprite(&cat_sprites, 22, CAT_X, CAT_Y, 4);
// 		sleep(200 * SLEEP_FACTOR);

// 		pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 		pml_draw_sprite(&cat_sprites, 23, CAT_X, CAT_Y, 4);
// 		sleep(200 * SLEEP_FACTOR);

// 		for (int index = 0; index < 6; index++) {
// 			pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 			pml_draw_sprite(&cat_sprites, 21, CAT_X, CAT_Y, 4);
// 			sleep(150 * SLEEP_FACTOR);

// 			pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 			pml_draw_sprite(&cat_sprites, 22, CAT_X, CAT_Y, 4);
// 			sleep(150 * SLEEP_FACTOR);

// 			pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 			pml_draw_sprite(&cat_sprites, 23, CAT_X, CAT_Y, 4);
// 			sleep(150 * SLEEP_FACTOR);
// 		}
// 		pml_draw_rect(CAT_X, CAT_Y, 32*4, 32*4, 0xff);
// 		pml_draw_sprite(&cat_sprites, 18, CAT_X, CAT_Y, 4);
// 		sleep(500 * SLEEP_FACTOR);

// 	}

// }

// pml_setbuffer((u8*)ibuff, SCREEN_WIDTH, SCREEN_HEIGHT);
// screen_fill(0xff);

// for (int x = 0; x < 8; x++) {
//     for (int y = 0; y < 8; y++) {
//         u8 pixel = tileset.data[y*32 + x]; // first 8x8 block
//         pml_draw_rect(x*8, y*8, 8, 8, pixel);
//     }
// }

// memcpy((u8*)SCREEN_ADDR, (u8*)ibuff, SCREEN_SIZE);
