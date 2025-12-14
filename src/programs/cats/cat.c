#include "cat.h"
#include "graphics/pomelo.h"
#include "sprites.h"

#include "drivers/kbd.h"
#include "drivers/screen.h"

#include "level.h"
#include "main.h"

// Cat licking itself
u32 licking_frames[] = {19, 20, 21, 22, 23, 21, 22, 23};
u32 licking_durations[] = {30, 20, 20, 20, 20, 15, 15, 15};
Animation anim_licking = {
	.frames = licking_frames,
	.durations = licking_durations,
	.length = 8,
	.loop = 1,
	.loop_start_frame = 5,
};

// Cat gong to sleep (flat)
u32 flat_sleep_frames[] = {24, 25, 26, 27, 27};
u32 flat_sleep_durations[] = {10, 10, 10, 10, 10};
Animation anim_flat_sleep = {
	.frames = flat_sleep_frames,
	.durations = flat_sleep_durations,
	.length = 5,
	.loop = 1,
	.loop_start_frame = 4
};

// Going to sleep (curled)
u32 curled_sleep_frames[] = {19, 28, 29};
u32 curled_sleep_durations[] = {100, 50, 60};
Animation anim_curled_sleep = {
	.frames = curled_sleep_frames,
	.durations = curled_sleep_durations,
	.length = 3,
	.loop = 1,
	.loop_start_frame = 1
};

// Cat sitting down
u32 sitting_frames[] = {16, 17, 18};
u32 sitting_durations[] = {20, 10, 10};
Animation anim_sitting = {
	.frames = sitting_frames,
	.durations = sitting_durations,
	.length = 3,
	.loop = 1,
	.loop_start_frame = 2
};

// Cat walking left (from right)
u32 walking_left_frames[] = {12, 13, 14, 15};
u32 walking_left_durations[] = {12, 12, 12, 12};
Animation anim_walking_left = {
	.frames = walking_left_frames,
	.durations = walking_left_durations,
	.length = 4,
	.loop = 1,
	.loop_start_frame = 0
};


// Cat walking right (from left)
u32 walking_right_frames[] = {4, 5, 6, 7};
u32 walking_right_durations[] = {12, 12, 12, 12};
Animation anim_walking_right = {
	.frames = walking_right_frames,
	.durations = walking_right_durations,
	.length = 4,
	.loop = 1,
	.loop_start_frame = 0
};

// Cat walking up
u32 walking_up_frames[] = {8, 9, 10, 11};
u32 walking_up_durations[] = {12, 12, 12, 12};
Animation anim_walking_up = {
	.frames = walking_up_frames,
	.durations = walking_up_durations,
	.length = 4,
	.loop = 1,
	.loop_start_frame = 0
};

// Cat walking down
u32 walking_down_frames[] = {0, 1, 2, 3};
u32 walking_down_durations[] = {12, 12, 12, 12};
Animation anim_walking_down = {
	.frames = walking_down_frames,
	.durations = walking_down_durations,
	.length = 4,
	.loop = 1,
	.loop_start_frame = 0
};


void cat_walk_update(Entity *e) {
}

void cat_walk_think(Entity *e) {
	Cat *cat = (Cat *)e;
	cat->base.next_think = TICKS + 10;

	float x = cat->base.x;
	float y = cat->base.y;
	float nx = cat->base.move.nx;
	float ny = cat->base.move.ny;
	float ix = cat->base.move.ix;
	float iy = cat->base.move.iy;

	if (is_pos_invalid(cur_level_ptr, nx + ix/2, ny + iy/2)) {
		cat->base.move.ix *= -1;
	}

	int chance = randint(1, 100);
	if (chance < 5) {
		cat->base.move.ix = cat->base.move.iy = 0;
		cat->base.move.wants_to_move = 0;
		set_anim(&cat->base.anim_state, &anim_sitting);
		entity_set_state(&cat->base, &cat_idle_update, &cat_idle_think);
	}

}


void cat_idle_update(Entity *e) {
	return;
}

void cat_idle_think(Entity *e) {
	e->next_think = TICKS + 30;

	int chance = randint(1, 100);
	if (chance < 30 && e->anim_state.anim == &anim_sitting && e->anim_state.looping_for > 5) {
		set_anim(&e->anim_state, &anim_licking);
	}

	if (chance < 10 && e->anim_state.anim == &anim_licking && e->anim_state.looping_for > 20) {
		set_anim(&e->anim_state, &anim_curled_sleep);
	}

    u8 entity_bumped = 0;
    for (int i = 0; i < active_entity_count; i++) {
        Entity* o = active_entities[i];
        if (o == e) continue;

        if (check_entity_collision(o, e, o->move.nx, o->move.ny, e->x, e->y)) {
            entity_bumped = 1;
        }
    }

    if (entity_bumped) {
   		set_intent_from_orientation(e, rand() % 4);
   		e->move.wants_to_move = 1;
   		e->next_think = TICKS + 10;
   		entity_set_state(e, &cat_walk_update, &cat_walk_think);
    }

}

void cat_manual_update(Entity *e) {
	return;
}

void cat_manual_think(Entity *e) {
	e->next_think = TICKS;
    e->move.ix = 0;
    e->move.iy = 0;

    if (kbd_is_key_down[KEY_W]) e->move.iy -= 1;
    if (kbd_is_key_down[KEY_S]) e->move.iy += 1;
    if (kbd_is_key_down[KEY_A]) e->move.ix -= 1;
    if (kbd_is_key_down[KEY_D]) e->move.ix += 1;

    if (kbd_state.shiftPressed) {
    	e->move.speed = 0.08f;
    } else {
    	e->move.speed = 0.05f;
    }

    if (e->move.iy || e->move.ix) e->move.wants_to_move = 1;

    if (e->anim_state.paused_for > 100) {
    	int chance = randint(1, 100);
    	if (e->orientation != DIR_UP) {
	     	if (chance < 50 && e->orientation == DIR_RIGHT) {
				set_anim(&e->anim_state, &anim_flat_sleep);	
	    	} else {
	    		set_anim(&e->anim_state, &anim_sitting);
	    	}   		
    	}
    }
    return;
}