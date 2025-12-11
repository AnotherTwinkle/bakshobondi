#ifndef CAT_H
#define CAT_H

#include "kernel/util.h"
#include "kernel/pit.h"

#include "graphics/pomelo.h"

#define IDLE 0
#define WALKING 1
#define LICKING 2
#define SLEEPING 3
#define CURLED 4
#define SITTING_DOWN 5

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
} AnimState;

typedef struct Cat {
	int posx, posy;
	AnimState anim_state;
	u8 type;
	u8 state;
	u8 orientation;
} Cat;

// Defined animations
extern Animation anim_licking;
extern Animation anim_flat_sleep;
extern Animation anim_curled_sleep;
extern Animation anim_sitting;
extern Animation anim_walking_left;

// Functions
void set_anim(AnimState *s, Animation *a);
void update_anim(AnimState *s);
void draw_cat(Cat *cat, int scale);
#endif