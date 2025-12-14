#ifndef ANIM_H
#define ANIM_H

#include "kernel/util.h"

// Convention: Quadrants
#define FACING_RIGHT 0
#define FACING_LEFT 2
#define FACING_UP 1
#define FACING_DOWN 3

typedef struct Animation {
	const u32 *frames;
	const u32 *durations;
	u32 length;
	u8 loop;

	u32 loop_start_frame;
} Animation;

typedef struct AnimState {
	Animation *anim;
	u32 frame;
	u32 ticks_left;
	u8 looping;
	u32 looping_for;
	u8 paused;
} AnimState;

// Functions
void set_anim(AnimState *s, Animation *a);
void set_frame(AnimState *s, u32 frame);

void update_anim(AnimState *s);

#endif // ANIM_H
