#include "sprites.h"
#include "lcd.h"
#include "util.h"


// Draw a sprite at whatever location it wants
void draw_sprite(Sprite *s) {
  move_sprite(s, 0, 0, 1);
}

/*
 * Initialize a sprite
 * uint16_t x: The rightmost pixel
 * uint16_t y: The bottom most pixel
 * uint16_t width: The distance from rightmost to leftmost pixel
 * uint16_t height: The distance from bottom to top pixels
 * uint16_t *sprite_a: A pointer to the first frame of data for the sprite, must be nonzero,
 *                    and have at least as much data to fill the bounding box of the sprite
 * uint16_t *sprite_b: A pointer to the second frame of data for the sprite,i
 *                    could also be null if it doesn't exist
 * Sprite *s: A pointer to the sprite to initialize
 */
void init_sprite(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *sprite_a, uint16_t *sprite_b, Sprite *s) {
  s->bbox.x1 = x;
  s->bbox.y1 = y;
  s->bbox.x2 = x + width - 1;
  s->bbox.y2 = y + height - 1;
  if(sprite_a == sprite_b || !sprite_b) {
    s->sprite_swap_key = 0;
  } else {
    s->sprite_swap_key = ((uint32_t)sprite_a) ^ ((uint32_t)sprite_b);
  }
  s->sprite_data = sprite_a;
}

/*
 * Move a sprite to a location relative to it
 * Will not draw the sprite if any portion is out of screen bounds
 * Sprite *s: The sprite to move and draw
 * int16_t mov_x: The amount to move the sprite on the x axis,
 *                + is to the left, - is to the right
 * int16_t mov_y: The amount to move the sprite on the y axis,
 *                + is up, - is down
 * uint8_t force: Nonzero value will force the redraw
 */
void move_sprite(Sprite *s, int16_t mov_x, int16_t mov_y, uint8_t force) {
  Rect old, new, hull;
  old = s->bbox;
  new = s->bbox;

  if(mov_x != 0 || mov_y != 0  || force) {
    new.x1 += mov_x;
    new.x2 += mov_x;
    new.y1 += mov_y;
    new.y2 += mov_y;
    compute_hull(old, new, &hull);
  }
  // Set the new bounding box
  s->bbox = new;

  if(hull.x1 < 0 || hull.y1 < 0 || hull.x2 > LCD_W || hull.y2 > LCD_H) {
    return;
  }

  // Magic screen incatation
  lcddev.select(1);
  LCD_SetWindow(hull.x1, hull.y1, hull.x2, hull.y2);
  LCD_WriteData16_Prepare();


  // Start drawing the sprite
  uint16_t paint_idx = 0;
  for(uint16_t y = hull.y1; y <= hull.y2; y++) {
    for(uint16_t x = hull.x1; x <= hull.x2; x++) {
      // Only get sprite data if we're drawing the sprite itself
      if(contains(x, y, new)) {
        LCD_WriteData16(s->sprite_data[paint_idx]);
        paint_idx++;
      } else { //Otherwise we just want to clear the section where it was
        LCD_WriteData16(0x0);
      }
    }
  }

  LCD_WriteData16_End();
  lcddev.select(0);

}

/*
 * Detect if two sprites are colliding with each other
 * Return 0 if they are not
 * Otherwise return their overlap
 */
int sprite_coll(Sprite *s1, Sprite *s2) {
    if ((s1->bbox.x1 < 0 || s1->bbox.y1 < 0 || s1->bbox.x2 > LCD_W || s1->bbox.y2 > LCD_H) || (s2->bbox.x1 < 0 || s2->bbox.y1 < 0 || s2->bbox.x2 > LCD_W || s2->bbox.y2 > LCD_H)) {
        return 0;
    }
    return overlap(s1->bbox, s2->bbox);
}

/*
 * Teleport a sprite a long distance
 * will first clear the sprite, then draw it at it's new location
 *
 * uint16_t x: The new x coordinate
 * uint16_t y: The new y coordinate
 * Sprite *s: The sprite to teleport
 */
void teleport_sprite(uint16_t x, uint16_t y, Sprite *s) {
    // x2 = x + width - 1;
    // x2 - x = width - 1;
    // x2 - x + 1 = width;
    clear_sprite(s);
    int width = s->bbox.x2 - s->bbox.x1 + 1;
    int height = s->bbox.y2 - s->bbox.y1 + 1;
    s->bbox.x1 = x;
    s->bbox.y1 = y;
    s->bbox.x2 = x + width - 1;
    s->bbox.y2 = y + height - 1;
    draw_sprite(s);
}

/*
 * Clears a sprite from the screen
 * Sprite *s: The sprite to blank out
 */
void clear_sprite(Sprite *s) {
  lcddev.select(1);
  LCD_SetWindow(s->bbox.x1, s->bbox.y1, s->bbox.x2, s->bbox.y2);
  LCD_WriteData16_Prepare();
  uint16_t h = s->bbox.y2 - s->bbox.y1;
  uint16_t w = s->bbox.x2 - s->bbox.x1;
  for(uint16_t i = 0; i <= h; i++) {
    for(uint16_t j  = 0; j <= w; j++) {
      LCD_WriteData16(0x0);
    }
  }
  LCD_WriteData16_End();
  lcddev.select(0);
}
