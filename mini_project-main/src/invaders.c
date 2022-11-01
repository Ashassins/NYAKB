#include "invaders.h"
#include "lcd.h"
#include "sprites.h"
#include "util.h"

int score = 0;
InvaderArmy invader_army;

Sprite invader_explosion;
// This is the number of units in the army
int _n_units = sizeof(invader_army.units) / sizeof(invader_army.units[0]);

/* Initialize the invader state
 * Sets the starting x, y coordinates for the invaders, as well as the drop and step for each of their ticks
*/
void init_invaders(uint16_t start_x, uint16_t start_y, uint16_t drop, uint16_t step) {
  // Set the inital movement and bounding box location
  invader_army.bbox.x1 = start_x;
  invader_army.bbox.y1 = start_y;
  invader_army.drop = drop;
  invader_army.step = step;
  // The number of pixels we need to draw the invaders, accounting for the 2 px
  // between rows
  invader_army.bbox.x2 =
      invader_army.bbox.x1 + INVADERS_WIDTH * invader1_a_width;
  invader_army.bbox.y2 = invader_army.bbox.y1 +
                         INVADERS_HEIGHT * invader1_a_height +
                         (INVADERS_HEIGHT * 2) - 1;

  // Initialize each invader's sprite
  uint16_t *sprite_a;
  uint16_t *sprite_b;
  for (int y = 0; y < INVADERS_HEIGHT; y++) {
    for (int x = 0; x < INVADERS_WIDTH; x++) {
      if (y <= 1) {
        sprite_a = (uint16_t *)invader1_a;
        sprite_b = (uint16_t *)invader1_b;
      } else if (y <= 3) {
        sprite_a = (uint16_t *)invader2_a;
        sprite_b = (uint16_t *)invader2_b;
      } else {
        sprite_a = (uint16_t *)invader3_a;
        sprite_b = (uint16_t *)invader3_b;
      }
      init_sprite(
          x * invader1_a_width + invader_army.bbox.x1,
          y * (invader1_a_height + 2) + invader_army.bbox.y1, // Add two to acount for the spacing
          invader1_a_width, invader1_a_height, sprite_a, sprite_b,
          &invader_army.units[x + y * INVADERS_WIDTH]);
    }
  }
  init_sprite(1000, 1000, invader_explode_width, invader_explode_height, (uint16_t *)invader_explode, NULL, &invader_explosion);
}

// Draw the invaders in place
void draw_invaders() {
  // Save the movement for the invaders
  uint16_t drop = invader_army.drop;
  uint16_t step = invader_army.step;
  invader_army.drop = 0;
  invader_army.step = 0;
  // Call move with no change in position
  move_invaders();
  // Put things back
  invader_army.drop = drop;
  invader_army.step = step;

}

// Update the invader visual and move them
void update_invaders() {
  for (int i = 0; i < _n_units; i++) {
    // Swap each sprite pointer
    invader_army.units[i].sprite_data =
        (uint16_t *)(((uint32_t)invader_army.units[i].sprite_data) ^
                     invader_army.units[i].sprite_swap_key);
  }
  // Move the invaders
  move_invaders();
}

// Move the invaders along their zig-zag pattern
void move_invaders(){

  Rect hull, old, new;
  old = invader_army.bbox;
  new = invader_army.bbox;
  uint16_t new_left = old.x2 + invader_army.step;
  // Move the army
  if(new_left >= LCD_W || old.x1 + invader_army.step < 0) {
    invader_army.step = -invader_army.step;
    new.y1 = old.y1 - invader_army.drop;
    new.y2 = old.y2 - invader_army.drop;
    for(int i = 0; i < _n_units; i++) {
      invader_army.units[i].bbox.y1 -= invader_army.drop;
      invader_army.units[i].bbox.y2 -= invader_army.drop;
    }
  } else {
    new.x1 = old.x1 + invader_army.step;
    new.x2 = old.x2 + invader_army.step;
    new.y1 = old.y1;
    new.y2 = old.y2;
    for(int i = 0 ;i < _n_units; i++) {
      invader_army.units[i].bbox.x1 += invader_army.step;
      invader_army.units[i].bbox.x2 += invader_army.step;
    }
  }
  // Compute the full area to draw
  compute_hull(old, new, &hull);
  invader_army.bbox = new;

  // Start the draw call
  lcddev.select(1);
  LCD_SetWindow(hull.x1, hull.y1, hull.x2, hull.y2);
  LCD_WriteData16_Prepare();

  // For tracking the drawing
  uint16_t sprite_x = 0, sprite_y = 0;
  uint16_t army_x = 0, army_y = 0;

  //// Draw the army
  for (uint16_t glob_y = hull.y1; glob_y <= hull.y2; glob_y++) {
    // Do we need to draw between the rows
    if (sprite_y >= invader1_a_height) {
      // Draw some spacing between the rows
      glob_y += 2;
      for (int i = 0; i < (hull.x2 - hull.x1 + 1) * 2; i++) {
        LCD_WriteData16(0x0);
      }
      // Reset the sprite veritcal
      sprite_y = 0;
      army_y++;
    }
    // Go through a row
    for (uint16_t glob_x = hull.x1; glob_x <= hull.x2; glob_x++) {
      // If the pixel we're at is in side of our bounding box, do we want to draw it?
      if (contains(glob_x, glob_y, new)) {
        // Compute which invader we're drawing currently
        uint16_t army_idx = army_x + army_y * INVADERS_WIDTH;
        // Make sure we don't try to draw something that doesn't exist
        if (army_idx >= INVADERS_WIDTH * INVADERS_HEIGHT) {
          army_idx = INVADERS_WIDTH * INVADERS_HEIGHT - 1;
        }
        // Get the sprite of our current invader
        uint16_t *cur_data = invader_army.units[army_idx].sprite_data;
        if (cur_data) {
          // Draw the invader if it's alive
          uint16_t data = cur_data[sprite_x + sprite_y * invader1_a_width];
          LCD_WriteData16(data);
        } else {
          // Fill with black otherwise
          LCD_WriteData16(0x0);
        }
        // Move along
        sprite_x++;
        if (sprite_x >= invader1_a_width) {
          sprite_x = 0;
          army_x++;
        }
      } else { // Draw over where we were
        LCD_WriteData16(0x0);
      }
    }
    // Move along
    army_x = 0;
    sprite_y++;
    sprite_x = 0;
  }
  // Close down the communication with the lcd
  LCD_WriteData16_End();
  lcddev.select(0);

}

uint8_t invader_coll(Sprite *shot) {
    for (int i = 0; i < INVADERS_COUNT; i++) {
        if (sprite_coll(shot, &(invader_army.units[i])) && invader_army.units[i].sprite_data != NULL) {
            if ((invader_army.units[i].sprite_data == &invader1_a) || (invader_army.units[i].sprite_data == &invader1_b)) {
                score += 10;
            } else if ((invader_army.units[i].sprite_data == &invader2_a) || (invader_army.units[i].sprite_data == &invader2_b)) {
                score += 20;
            } else if ((invader_army.units[i].sprite_data == &invader3_a) || (invader_army.units[i].sprite_data == &invader3_b)) {
                score += 30;
            }
            invader_army.units[i].sprite_data = NULL;
            invader_army.units[i].sprite_swap_key = 0;
            invader_explosion.bbox = invader_army.units[i].bbox;
            draw_sprite(&invader_explosion);
            //clear_sprite(&invader_army.units[i]);
            return 1;
        }
    }
    return 0;
}

void clear_invaders() {
  lcddev.select(1);
  LCD_SetWindow(invader_army.bbox.x1, invader_army.bbox.y1, invader_army.bbox.x2, invader_army.bbox.y2);
  LCD_WriteData16_Prepare();
  for(uint16_t i = invader_army.bbox.y1; i < invader_army.bbox.y2; i++) {
    for(uint16_t j = invader_army.bbox.x1; j < invader_army.bbox.x2; j++) {
      LCD_WriteData16(0x0);
    }
  }
  LCD_WriteData16_End();
  lcddev.select(0);
}


