#include "cat.h"
#include "graphics/pomelo.h"
#include "sprites.h"

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

void draw_cat(Cat *cat, int scale) {
	u32 idx = cat->anim_state.anim->frames[cat->anim_state.frame];
	pml_draw_sprite_ca(&cat_sprites, idx, cat->posx, cat->posy, scale);
}

