#ifndef SPRITES_H
#define SPRITES_H
#include "stm32f0xx_dac.h"
#include "util.h"
#include <stdint.h>

typedef struct {
  // The bounding box for the sprite
  Rect bbox;
  // The pointer to the data to draw the sprite
  // The data is laid out in Row-major order
  uint16_t *sprite_data;
  // This is computed as sprite_data_a ^ sprite_data_b
  // it get's xored with sprite_data to swich the sprite
  // If there is no alternate sprite, this will be zero
  uint32_t sprite_swap_key;
} Sprite;

void draw_sprite(Sprite *s);

/*
 * Initialize a sprite
 * Sets the bottom right corner of the sprite as x/y
 * The top corner is (x,y) + (width, height) - 1
 * sprite_a is the A frame for the sprite
 * if there is no B frame, sprite_b can either be zero or equal to sprite_a
 */
void init_sprite(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *sprite_a, uint16_t *sprite_b, Sprite *s);

void move_sprite(Sprite *s, int16_t mov_x, int16_t mov_y, uint8_t force);

int sprite_coll(Sprite *s1, Sprite *s2);

void teleport_sprite(uint16_t x, uint16_t y, Sprite *s);



// Clears the space where the sprite is
void clear_sprite(Sprite *s);

#endif
