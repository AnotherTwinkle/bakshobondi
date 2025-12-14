#ifndef ENTITY_H
#define ENTITY_H

#include "kernel/util.h"

#include "sprites.h"
#include "anim.h"

#define MAX_ENTITIES 	 256
#define ENTITY_SLOT_SIZE 128

#define DIR_RIGHT 0
#define DIR_UP 1
#define DIR_LEFT 2
#define DIR_DOWN 3

#define ENTITY_MOVABLE (1 << 0)
#define ENTITY_ANIM_DRIVEN_BY_MOVE (1 << 1)

extern int active_entity_count;

typedef struct Entity Entity;
typedef void (*update_func)(Entity *);
typedef void (*think_func)(Entity *);

typedef struct EntityMovement {
	float ix;
	float iy;
	float speed;

	float nx;
	float ny; // Upcoming position if current movement continues

	u8 moving;
	u8 wants_to_move;
} EntityMovement;

typedef struct Entity {
	float x, y; 
	EntityMovement move;
	u8 orientation;

	update_func update;
	think_func think;
	int next_think;

	AnimState anim_state;
	Animation **move_anims;

	SpriteSheet *spritesheet;

	u32 flags;
	int pool_index;

} Entity;


extern u8 entity_pool[MAX_ENTITIES][ENTITY_SLOT_SIZE];
extern u8 entity_used[MAX_ENTITIES];
extern Entity *active_entities[MAX_ENTITIES];
extern int active_entity_count;


Entity* entity_alloc(int size);
void entity_free(Entity *e); 

void entity_add(Entity *e);
void entity_remove(int index);
void entity_render(Entity *entity, int scale);
void entity_set_state(Entity *entity, update_func update, think_func think);

#endif // ENTITY_H
