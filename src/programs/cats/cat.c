#include "cat.h"
#include "graphics/pomelo.h"
#include "sprites.h"

#include "drivers/screen.h"
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
	.loop = 0,
	.loop_start_frame = -1
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



void set_anim(AnimState *s, Animation *a) {
	s->anim = a;
	s->frame = 0;
	s->looping = 0;
	s->looping_for = 0;
	s->ticks_left =  a->durations[0];
}

void update_anim(AnimState *s) {
	if (!s->anim) return;

	if (s->ticks_left > 0) {
		s->ticks_left--;
		return;
	}

	s->frame++;

	if (s->frame >= s->anim->length) {
		if (s->anim->loop) {
			s->frame = s->anim->loop_start_frame;
			s->looping = 1; 
		} else {
			s->frame = s->anim->length - 1;
		}
	}

	if (s->looping) s->looping_for++;

	s->ticks_left = s->anim->durations[s->frame];
}

void cat_update(Cat* cat) {
	if (cat->state == WALKING) {
		int speed = 1; // per tick
		if (cat->orientation == FACING_RIGHT) {
			cat->posx += speed;
		}
		else if (cat->orientation == FACING_LEFT) {
			cat->posx -= speed;
		}

		else if (cat->orientation == FACING_UP) {
			cat->posy -= speed;
		}

		else if (cat->orientation == FACING_DOWN) {
			cat->posy += speed;
		}


		if (cat->posx + 16*SCALE > SCREEN_WIDTH) {
			cat->orientation = FACING_LEFT;
			set_anim(&cat->anim_state, &anim_walking_left);
		}

		if (cat->posx - 16*SCALE < 0) {
			cat->orientation = FACING_RIGHT;
			set_anim(&cat->anim_state, &anim_walking_right);
		}
	}

	if (cat->state == WALKING && randint(1, 10000) < 10) {
		cat->orientation = FACING_DOWN;
		cat->state = SITTING_DOWN;
		set_anim(&cat->anim_state, &anim_sitting);
	}

	if (cat->state == SITTING_DOWN && cat->anim_state.looping == 1) {
		cat->state = IDLE;
	}

	int chance = randint(0, 1000);
	if (chance < 30 && cat->state == IDLE && cat->anim_state.looping_for > 5) {
		set_anim(&cat->anim_state, &anim_licking);
		cat->state = LICKING;
	}

	if (chance < 10 && cat->state == LICKING && cat->anim_state.looping_for > 20) {
		set_anim(&cat->anim_state, &anim_curled_sleep);
		cat->state = CURLED;
	}
	update_anim(&cat->anim_state);
}

void draw_cat(Cat *cat, int scale) {
	u32 idx = cat->anim_state.anim->frames[cat->anim_state.frame];
	pml_draw_sprite_ca(cat->spritesheet, idx, cat->posx, cat->posy, scale);
}

