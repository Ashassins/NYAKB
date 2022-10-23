#ifndef BUNKER_H
#define BUNKER_H

#include "lcd.h"
#include "sprite_data.h"
#include "sprites.h" 

#define BUNKER_COUNT 4
#define BUNKER_WIDTH 76
#define BUNKER_HEIGHT 5
#define BUNKER_FULL_HEALTH 16

typedef struct {
    uint8_t id; // each bunker has its own ID
    uint8_t hp; // total num hits bunker can take
    uint16_t x, y;
    Sprite nextBunkerStage; // stores the next sprite the bunker would turn into
    Sprite cleanBunker; // stores the clean slate sprite
} Bunker;

typedef struct {
    Bunker bunkerArray[BUNKER_COUNT];
    uint16_t cleanBunker; // stores the clean slate sprite
} BunkerArray;

void init_4_bunkers();
Bunker damage_bunkers(Bunker bunker);
void update_bunker(uint8_t bunkerId);
void redraw_bunker(Sprite *nextBunker);
uint8_t bunker_coll(Sprite *shot);
#endif