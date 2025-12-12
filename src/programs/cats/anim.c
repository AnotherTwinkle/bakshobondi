#include "anim.h"

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