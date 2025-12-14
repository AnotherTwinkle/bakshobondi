#include "kernel/util.h"
#include "main.h"

static u32 SEED  = 0xFACEDAB;

static u8 arr_cur_buffer[SCREEN_SIZE];
static u8 arr_next_buffer[SCREEN_SIZE];

static u8 *cur_buffer = arr_cur_buffer;
static u8 *next_buffer = arr_next_buffer;

static int KBD_SUB_ID;
int TICKS = 0;

KeyEvent kbd_event;
u8 kbd_result;

Level* cur_level_ptr = &level0;

Camera camera = {
	.posx = 0.0f,
	.posy = 0.0f,

	.dx = 0.0f,
	.dy = 0.0f,

	.zoom = 2,

	.is_following_entity = 0,
	.following_x = 0x0,
	.following_y = 0x0,

	.is_moving_to_point = 0,
	.target_x = 0.0f,
	.target_y = 0.0f,
	.move_dx = 0.0f,
	.move_dy = 0.0f
};

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

	pml_setbuffer(next_buffer, SCREEN_WIDTH, SCREEN_HEIGHT);

	init_sprites();

	SpriteSheet* cat_sprites[4] = {
	    &cat0_sprites,
	    &cat1_sprites,
	    &cat2_sprites,
	    &cat3_sprites
	};

	static Animation *cat_move_anims[4] = {
	    &anim_walking_right,
	    &anim_walking_up,
	    &anim_walking_left,
	    &anim_walking_down
	};

	AnimState cat_anim_state;

	EntityMovement cat_movement = {
		.ix = 0,
		.iy = 0,
		.speed = 0,
		.moving = 0,
		.wants_to_move = 0
	};

	for (int i = 0; i < 2; i++) {
	    Cat *cat = (Cat *)entity_alloc(sizeof(Cat));
	    if (!cat)
	        break;

	    cat->base.x = 16.0f;
	    cat->base.y = 12 + i;
	    cat->base.spritesheet = cat_sprites[i];

	    cat->base.update = cat_walk_update;
	    cat->base.think  = cat_walk_think;
	    cat->base.next_think = TICKS + 10;

		cat->base.anim_state = cat_anim_state;
		cat->base.orientation = (i ? DIR_LEFT : DIR_RIGHT);

		cat->base.flags |=  ENTITY_MOVABLE | ENTITY_ANIM_DRIVEN_BY_MOVE;

		cat->base.move = cat_movement;
    	cat->base.move.speed = 0.05f;
    	cat->base.move.ix = (i ? -1 : 1);
    	cat->base.move.iy = 0;
    	cat->base.move.wants_to_move = 1;
    	cat->base.move_anims = cat_move_anims;
	    set_anim(
	        &cat->base.anim_state,
	        (cat->base.orientation == DIR_LEFT)
	            ? &anim_walking_left
	            : &anim_walking_right
	    );

	    entity_add(&cat->base);
	}

	for (int i = 2; i < 3; i++) {
	    Cat *cat = (Cat *)entity_alloc(sizeof(Cat));
	    if (!cat)
	        break;

	    cat->base.y = 16.0f;
	    cat->base.x = 12 + i;
	    cat->base.orientation = DIR_DOWN;
	    cat->base.spritesheet = cat_sprites[i];

	    cat->base.update = cat_manual_update;
	    cat->base.think  = cat_manual_think;
	    cat->base.next_think = TICKS + 10;

	    cat->base.flags |=  ENTITY_MOVABLE | ENTITY_ANIM_DRIVEN_BY_MOVE;
    	cat->base.move.speed = 0.05f;
    	cat->base.move_anims = cat_move_anims;

		cat->base.anim_state = cat_anim_state;
	    set_anim(
	        &cat->base.anim_state,
	        &anim_sitting
	    );
	    set_frame(&cat->base.anim_state, 2);

	    entity_add(&cat->base);
		camera_follow_entity(&camera, &cat->base.x, &cat->base.y);
	}
	while(1) {
		// Keyboard
		kbd_result = kbd_dequeue(&kbd_queue, KBD_SUB_ID, &kbd_event);

		// Entity update and thinking
		for (int i = 0; i < active_entity_count; i++) {
			Entity *e = active_entities[i];

			if (e->think && TICKS >= e->next_think){
				e->think(e);
			}

			if (e->update) {
				e->update(e);
			}
		}

		for (int i = 0; i < active_entity_count; i++) {
			Entity *e = active_entities[i];
			movement_update(e);
			update_anim(&e->anim_state);
		}

		// Level Rendering
		pml_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff);
		for (int l = 0; l < cur_level_ptr->layer_count; l++) {
			if (cur_level_ptr->rendering_flags[l] & RF_HOVER) continue;

			for (int x = camera.posx; x < (int)(camera.posx +  (float)SCREEN_WIDTH/(16.0f*camera.zoom)) + 1 ; x++) { // Only render spsrites in our current viewport
				for (int y = camera.posy; y < (int)(camera.posy + (float)SCREEN_WIDTH/(16.0f*camera.zoom)) + 1 ; y++) {
					s16 sprite = cur_level_ptr->rendering_layers[l][x*cur_level_ptr->height_t + y];
					if (sprite < 0) continue;
					level_draw_sprite(cur_level_ptr->spritesheet, sprite, x, y, camera.zoom);
				}
			}
		}

		// Entity Rendering
		for (int i = 0; i < active_entity_count; i++) {
			entity_render(active_entities[i], camera.zoom);
		}

		// Level Rendering (Hover layers)
		for (int l = 0; l < cur_level_ptr->layer_count; l++) {
			if (!(cur_level_ptr->rendering_flags[l] & RF_HOVER)) continue;
			for (int x = camera.posx; x < (int)(camera.posx +  (float)SCREEN_WIDTH/(16.0f*camera.zoom)) + 1 ; x++) {
				for (int y = camera.posy; y < (int)(camera.posy + (float)SCREEN_WIDTH/(16.0f*camera.zoom)) + 1 ; y++) {
					s16 sprite = cur_level_ptr->rendering_layers[l][x*cur_level_ptr->height_t + y];
					if (sprite < 0) continue;
					level_draw_sprite(cur_level_ptr->spritesheet, sprite, x, y, camera.zoom);
				}
			}
		}

		// for (int i = 0; i < cats[0].anim_state.frame; i++) {
		// 	pml_draw_rect(4*i, 0, 2, 2, 0xff);
		// }

		// for (int i = 0; i < cats[0].anim_state.anim->length; i++) {
		// 	pml_draw_rect(4*i, 8, 2, 2, 0xff);
		// }

		// for (int i = 0; i < cats[0].state ; i++) {
		// 	pml_draw_rect(4*i, 16, 2, 2, 0xff);
		// }

		// for (int i = 0; i < cats[0].anim_state.looping_for ; i++) {
		// 	pml_draw_rect(4*i, 24, 2, 2, 0xff);
		// }

		for (int i = 0; i < (int)active_entities[0]->x; i++) {
			pml_draw_rect(4*i, 32, 2, 2, 0xff);
		}


		for (int i = 0; i < (int)active_entities[0]->y; i++) {
			pml_draw_rect(4*i, 40, 2, 2, 0xff);
		}

		if (TICKS % 2 == 0) {
			pml_draw_rect(4, 48, 1, 1, 0xff);
		}
		
		camera_update(&camera);
		sleep(10);
		TICKS++;
		SWAP();
	}
}
