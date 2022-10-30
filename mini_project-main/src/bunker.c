#include "bunker.h"
#include "lcd.h"
#include "util.h"
#include "sprites.h"
#include "sprite_data.h"

BunkerArray bunkers;
Sprite clean_bunker;

int _n_bunkers = sizeof(bunkers.bunkerArray) / sizeof(bunkers.cleanBunker);

void init_4_bunkers() { 
    for (int i = 0; i < BUNKER_COUNT; i++) {
        bunkers.bunkerArray[i].id = i;
        bunkers.bunkerArray[i].hp = BUNKER_FULL_HEALTH;
        bunkers.bunkerArray[i].x = 30 + 50 * i;
        bunkers.bunkerArray[i].y = 65;
        init_sprite(bunkers.bunkerArray[i].x,bunkers.bunkerArray[i].y ,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &(bunkers.bunkerArray[i].cleanBunker));
        init_sprite(bunkers.bunkerArray[i].x,bunkers.bunkerArray[i].y ,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &(bunkers.bunkerArray[i].nextBunkerStage));
        draw_sprite(&(bunkers.bunkerArray[i].cleanBunker));
    }
}

void damage_bunker(Bunker *bunker) {
    if (bunker->hp != 0) {
        bunker->hp -= 1;
    }
    if (bunker->hp <= 16 && bunker->hp > 12){
        bunker->nextBunkerStage.sprite_data = &bunker_clean;
    } else if (bunker->hp <= 12 && bunker->hp > 8){
        bunker->nextBunkerStage.sprite_data = &bunker_lightly_damaged;
    } else if (bunker->hp <= 8 && bunker->hp > 4){
        bunker->nextBunkerStage.sprite_data = &bunker_moderately_damaged;
    } else if (bunker->hp <= 4 && bunker->hp > 0){
        bunker->nextBunkerStage.sprite_data = &bunker_heavily_damaged;
    } else if (bunker->hp  == 0){
        bunker->nextBunkerStage.sprite_data = &bunker_dead; 
    }
}

uint8_t bunker_coll(Sprite *shot) {
    for (int i = 0; i < BUNKER_COUNT; i++) {
        if (sprite_coll(shot, &(bunkers.bunkerArray[i].cleanBunker)) && (bunkers.bunkerArray[i].hp > 0) && (bunkers.bunkerArray[i].hp <= BUNKER_FULL_HEALTH)) {
            update_bunker(i);
            return 1;
        }
    }
    return 0;
}

void update_bunker(uint8_t bunkerId) {
    damage_bunker(&bunkers.bunkerArray[bunkerId]);
    redraw_bunker(&(bunkers.bunkerArray[bunkerId].nextBunkerStage));
}

void redraw_bunker(Sprite *nextBunker) {
    draw_sprite(nextBunker);
}


