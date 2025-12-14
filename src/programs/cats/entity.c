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

void set_intent_from_orientation(Entity *e, u8 dir) {
    int x, y;
    switch (dir) {
        case DIR_UP:
            x = 0;
            y = -1;
            break;
        case DIR_DOWN:
            x = 0;
            y = 1;
            break;
        case DIR_RIGHT:
            x = 1;
            y = 0;
            break;
        case DIR_LEFT:
            x = -1;
            y = 0;
            break;
        default:
            x = 0;
            y = 0;
    }

    e->move.ix = x;
    e->move.iy = y;
}
u8 check_entity_collision(Entity* a, Entity* b,
                          float ax, float ay,
                          float bx, float by)
{
    int afi = a->anim_state.anim->frames[a->anim_state.frame];
    int bfi = b->anim_state.anim->frames[b->anim_state.frame];

    
    u32 abb = a->spritesheet->sprite_bounding_box[afi];
    u32 bbb = b->spritesheet->sprite_bounding_box[bfi];

    float a_lx = BB_FIRST_X(abb);
    float a_rx = BB_LAST_X(abb);
    float a_ty = BB_FIRST_Y(abb);
    float a_by = BB_LAST_Y(abb);

    float b_lx = BB_FIRST_X(bbb);
    float b_rx = BB_LAST_X(bbb);
    float b_ty = BB_FIRST_Y(bbb);
    float b_by = BB_LAST_Y(bbb);

    float a_w = (float)a->spritesheet->unit_width;
    float a_h = (float)a->spritesheet->unit_height;
    float b_w = (float)b->spritesheet->unit_width;
    float b_h = (float)b->spritesheet->unit_height;

    float a_left   = ax + (a_lx - a_w*0.5f)/TILE_SIZE;
    float a_right  = ax + (a_rx - a_w*0.5f)/TILE_SIZE;
    float a_top    = ay + (a_h*0.5f - a_ty)/TILE_SIZE;
    float a_bottom = ay + (a_h*0.5f - a_by)/TILE_SIZE;

    float b_left   = bx + (b_lx - b_w*0.5f)/TILE_SIZE;
    float b_right  = bx + (b_rx - b_w*0.5f)/TILE_SIZE;
    float b_top    = by + (b_h*0.5f - b_ty)/TILE_SIZE;
    float b_bottom = by + (b_h*0.5f - b_by)/TILE_SIZE;

    const float tol = 0.25f; 
    a_left   += tol; a_right  -= tol;
    a_top    -= tol; a_bottom += tol;
    b_left   += tol; b_right  -= tol;
    b_top    -= tol; b_bottom += tol;

    if (a_right  < b_left)   return 0;
    if (a_left   > b_right)  return 0;
    if (a_bottom > b_top)    return 0;
    if (a_top    < b_bottom) return 0;

    return 1;
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

    u8 entity_blocked = 0;
    for (int i = 0; i < active_entity_count; i++) {
        Entity* o = active_entities[i];
        if (o == e) continue;

        if (check_entity_collision(o, e, o->x, o->y, nx, ny)) {
            entity_blocked = 1;
        }
    }

    if (is_pos_invalid(cur_level_ptr, nx, ny) || entity_blocked) {
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
