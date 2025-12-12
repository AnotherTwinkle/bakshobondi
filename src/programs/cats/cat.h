#ifndef CAT_H
#define CAT_H

#include "kernel/util.h"
#include "kernel/pit.h"

#include "graphics/pomelo.h"
#include "sprites.h"
#include "anim.h"

#define CAT_IDLE 0
#define CAT_WALKING 1
#define CAT_LICKING 2
#define CAT_SLEEPING 3
#define CAT_CURLED 4
#define CAT_SITTING_DOWN 5

typedef struct Cat {
	float posx, posy;
	float dx, dy; // Amount moved last tick
	AnimState anim_state;
	SpriteSheet *spritesheet;
	u8  type;
	u8  state;
	u8  orientation;
} Cat;

// Defined animations
extern Animation anim_licking;
extern Animation anim_flat_sleep;
extern Animation anim_curled_sleep;
extern Animation anim_sitting;
extern Animation anim_walking_left;
extern Animation anim_walking_right;

void cat_update(Cat *cat);
void draw_cat(Cat *cat, int scale);

#endif