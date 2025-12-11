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

	int CAT_X = SCREEN_WIDTH, CAT_Y = 100;
	int SCALE = 2;

	init_sprites();
	AnimState cat_anim_state;
	set_anim(&cat_anim_state, &anim_walking_left);

	Cat test_cat;
	test_cat.posx = CAT_X;
	test_cat.posy = CAT_Y;
	test_cat.type = 0;
	test_cat.state = WALKING;
	test_cat.orientation = FACING_LEFT;
	test_cat.anim_state = cat_anim_state;

	while(1) {
		// UPDATE

		if (test_cat.state == WALKING) {
			int speed = 1; // per tick
			if (test_cat.orientation == FACING_RIGHT) {
				test_cat.posx += speed;
			}
			else if (test_cat.orientation == FACING_LEFT) {
				test_cat.posx -= speed;
			}

			else if (test_cat.orientation == FACING_UP) {
				test_cat.posy -= speed;
			}

			else if (test_cat.orientation == FACING_DOWN) {
				test_cat.posy += speed;
			}
		}

		if (test_cat.state == WALKING && test_cat.posx <= SCREEN_WIDTH/2) {
			test_cat.orientation = FACING_DOWN;
			test_cat.state = SITTING_DOWN;
			set_anim(&test_cat.anim_state, &anim_sitting);
		}

		if (test_cat.state == SITTING_DOWN && test_cat.anim_state.looping == 1) {
			test_cat.state = IDLE;
		}

		int chance = randint(0, 1000);
		if (test_cat.state == IDLE && test_cat.anim_state.looping_for > 5) {
			set_anim(&test_cat.anim_state, &anim_licking);
			test_cat.state = LICKING;
		}

		if (chance < 10 && test_cat.state == LICKING && test_cat.anim_state.looping_for > 20) {
			set_anim(&test_cat.anim_state, &anim_curled_sleep);
			test_cat.state = CURLED;
		}
		update_anim(&test_cat.anim_state);


		// RENDERING
		pml_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff);
		for (int x = 0; x < SCREEN_WIDTH; x += SCALE * 16) {
			for (int y = 0; y < SCREEN_HEIGHT; y += SCALE * 16) {
				pml_draw_sprite(&tileset, 15, x, y, SCALE);
			}
		}

		int x[] = {5, 8, 5, 4, 13, 3};
		int y[] = {18, 0, 13, 1, 5, 6};
		for (int i = 0; i < 6; i++ ) {
			pml_draw_sprite(&tileset, (i%2)+1, x[i]*16, y[i]*16, SCALE);
		}
		draw_cat(&test_cat, 2);

		for (int i = 0; i < test_cat.anim_state.frame; i++) {
			pml_draw_rect(4*i, 0, 2, 2, 0xff);
		}

		for (int i = 0; i < test_cat.anim_state.anim->length; i++) {
			pml_draw_rect(4*i, 8, 2, 2, 0xff);
		}

		for (int i = 0; i < test_cat.state ; i++) {
			pml_draw_rect(4*i, 16, 2, 2, 0xff);
		}

		for (int i = 0; i < test_cat.anim_state.looping_for ; i++) {
			pml_draw_rect(4*i, 24, 2, 2, 0xff);
		}

		sleep(10);
		SWAP();
	}
}
