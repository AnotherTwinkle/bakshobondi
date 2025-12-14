#include "entity.h"

#include "kernel/util.h"
#include "graphics/pomelo.h"
#include "drivers/screen.h"

#include "main.h"
#include "anim.h"

u8 entity_pool[MAX_ENTITIES][ENTITY_SLOT_SIZE];
u8 entity_used[MAX_ENTITIES];
Entity *active_entities[MAX_ENTITIES];
int active_entity_count = 0;

Entity *entity_alloc(int size) {
	if (size > ENTITY_SLOT_SIZE) {
		while(1); // Remember to replace with panic function
	}

	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!entity_used[i]) {
			entity_used[i] = 1;
			memset(entity_pool[i], 0, ENTITY_SLOT_SIZE);
			Entity *e =  (Entity*)entity_pool[i];
			e->pool_index = i;

			return e;
		}
	}

	while(1); // Panic...
}

void entity_free(Entity *e) {
	entity_used[e->pool_index] = 0;
}

void entity_add(Entity *e) {
	active_entities[active_entity_count++] = e;
}

void entity_remove(int index) {
	active_entities[index] = active_entities[--active_entity_count];
}

void entity_render(Entity *entity, int scale) {
	u32 idx = entity->anim_state.anim->frames[entity->anim_state.frame];
	level_draw_sprite_ca(entity->spritesheet, idx, entity->x, entity->y, scale);
}

void entity_set_state(Entity *entity, update_func update, think_func think) {
    entity->update = update;
    entity->think = think;
}

void movement_update(Entity *e) {
    if (!(e->flags & ENTITY_MOVABLE))
        return;

    EntityMovement *m = &e->move;
    u8 was_moving = m->moving;
    m->moving = 0;

    float ix = m->ix;
    float iy = m->iy;

    if (ix == 0 && iy == 0) {
        if (was_moving && m->wants_to_move && (e->flags & ENTITY_ANIM_DRIVEN_BY_MOVE)) {
            set_frame(&e->anim_state, 0);
            e->anim_state.paused = 1;
        }
        return;
    }

    if (ix != 0 && iy != 0) {
        ix *= 0.7071f;
        iy *= 0.7071f;
    }

    float nx = e->x + ix * m->speed;
    float ny = e->y + iy * m->speed;

    m->nx = nx;
    m->ny = ny;

    if (is_pos_invalid(cur_level_ptr, nx, ny)) {
        if (was_moving && (e->flags & ENTITY_ANIM_DRIVEN_BY_MOVE)) {
            set_frame(&e->anim_state, 0);
            e->anim_state.paused = 1;   
        }
        return;
    }

    e->x = nx;
    e->y = ny;
    m->moving = 1;

    if (ix != 0 || iy != 0) {
        u8 dir;

        if (ix != 0) {
            dir = (ix > 0) ? DIR_RIGHT : DIR_LEFT;
        } else if (iy != 0) {
            dir = (iy > 0) ? DIR_DOWN : DIR_UP;
        } else {
            dir = e->orientation;
        }

        e->orientation = dir;

        if ((e->flags & ENTITY_ANIM_DRIVEN_BY_MOVE)) {
            Animation *a = e->move_anims[dir];
            if (a && e->anim_state.anim != a)
                set_anim(&e->anim_state, a);

            e->anim_state.paused = 0; // always play while moving
        }
    }
}
