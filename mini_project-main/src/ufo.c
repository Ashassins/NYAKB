#include "lcd.h"
#include "util.h"
#include "sprites.h"
#include "sprite_data.h"
#include "ufo.h"
#include "invaders.h"
#include <stdlib.h>
#define UFO_SPAWN 290
#define UFO_SPEED 3
Sprite ufo_explosion;
UFO ufo_object;

void init_ufo(){
  init_sprite(1000, UFO_SPAWN, ufo_width, ufo_height, (uint16_t *) ufo, NULL, &ufo_object.s);
  // Going to the left
  ufo_object.dir = 1;
  ufo_object.active = 0;
  ufo_object.points = 100;
  init_sprite(150, 1000, invader_explode_width, invader_explode_height, (uint16_t *)invader_explode, NULL, &ufo_explosion);

}


void update_ufo() {
  if(invader_army.bbox.y2 < UFO_SPAWN) {
    if(!ufo_object.active) {
      if(rand() % 200 == 1) {
        ufo_object.active = 1;
        if (ufo_object.dir == 1) {
          teleport_sprite(0, UFO_SPAWN, &ufo_object.s);
        } else {
          teleport_sprite(LCD_W - (ufo_object.s.bbox.x2 - ufo_object.s.bbox.x1) - 3, UFO_SPAWN, &ufo_object.s);
        }
      }
    } else { // UFO is active
      // Check if its in bounds
      if ((ufo_object.s.bbox.x2 >= LCD_W - 5 && ufo_object.dir) || (ufo_object.s.bbox.x1 <= 5 && !ufo_object.dir)) {
        ufo_object.active = 0;
        teleport_sprite(1000, UFO_SPAWN, &ufo_object.s);
        if(ufo_object.dir) {
          ufo_object.dir = 0;
        } else {
          ufo_object.dir = 1;
        }
      } else {
        if(ufo_object.dir == 1) {
          move_sprite(&ufo_object.s, UFO_SPEED, 0, 1);
        } else {
          move_sprite(&ufo_object.s, -UFO_SPEED, 0, 1);
        }
      }
    }
  } else {
    ufo_object.active = 0;
  }
}
