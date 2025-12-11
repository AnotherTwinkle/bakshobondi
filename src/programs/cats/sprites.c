#include "graphics/pomelo.h"
#include "drivers/disk.h"
#include "sprites.h"

#define CAT_SPRITES_MEM 0x200000
u8 *cat_sprites_data = (u8*)CAT_SPRITES_MEM;

#define TILESET_MEM 0x210000
u8 *tileset_data = (u8*)TILESET_MEM;

struct SpriteSheet tileset = {
    .width = 240,
    .height = 212,
    .unit_width = 16,
    .unit_height = 16,
    .data = 0x0
};

struct SpriteSheet cat_sprites = {
    .width = 128,
    .height = 256,
    .unit_width = 32,
    .unit_height = 32,
    .data = 0x0
}; 

void init_sprites() {
    load_bytes_to_buffer(cat_sprites_data, 256, 128*256);
    cat_sprites.data = cat_sprites_data;

    load_bytes_to_buffer(tileset_data, 384, 240*212);
    tileset.data = tileset_data;
}
