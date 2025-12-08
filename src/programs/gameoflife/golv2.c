#include "golv2.h"
#include "kernel/util.h"
#include "kernel/pit.h"
#include "drivers/screen.h"
#include "graphics/pomelo.h"

#define SEED 0xBADFACE
#define ON  0xff
#define OFF 0x20

static u8 ibuf[SCREEN_SIZE];
static u8 next[SCREEN_SIZE];
