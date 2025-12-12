#ifndef MAIN_H
#define MAIN_H

#include "drivers/screen.h"
#include "camera.h"

#define SCALE 2
#define TILE_SIZE 16

#define VIEWPORT_WIDTH_TILES SCREEN_WIDTH/(SCALE*TILE_SIZE)
#define VIEWPORT_HEIGHT_TILES SCREEN_HEIGHT/(SCALE*TILE_SIZE)

extern Camera camera;

void PROGRAM_CAT_MAIN();

void world_draw_sprite(SpriteSheet *sheet, int idx, float x, float y, int scale);
void world_draw_sprite_ca(SpriteSheet *sheet, int idx, float x, float y, int scale);

#endif