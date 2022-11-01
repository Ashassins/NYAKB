#include "lcd.h"
#include "util.h"
#include "sprites.h"
#include "sprite_data.h"


extern Sprite ufo_explosion;
typedef struct {
    uint16_t points;
    Sprite s;
    uint8_t dir;
    uint8_t active;
} UFO;

extern UFO ufo_object;

void init_ufo();

void update_ufo();
