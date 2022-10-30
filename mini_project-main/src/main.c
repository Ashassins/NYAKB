#include "bunker.h"
#include "invaders.h"
#include "lcd.h"
#include "nunchuk.h"
#include "sound.h"
#include "sprite_data.h"
#include "sprites.h"
#include "stm32f0xx.h"
#include "string.h"
#include "timer.h"
#include "ufo.h"
#include <stdlib.h>
#include <time.h>

#define SHOT_SPEED 10
#define LIGHTNING_SPEED 5
#define LIGHTNING_CNT 5

int start = 0;
// int score = 0;
int lives = 3; // Cannot be more than 9 for now
int all_dead = 0;
int inv_comp = 0;

Sprite player;
Sprite shot;
Sprite bolts[LIGHTNING_CNT];

uint8_t ufo_pop = 0;

void init_game_data();
void init_bolts();
void start_screen();
void game_loop();
void end_screen();

void handle_player_input();
void handle_shot();
void handle_invaders();
void handle_bolt_animate();

uint32_t get_ent();

void print_glbcnt(int x, int y) {
  char output[5] = "G:  ";
  itoa(glbcnt, &output[2], 10);
  LCD_DrawString(x, y, 0xFFFF, 0x0000, output, 16, 0);
}

void draw_score(int x, int y) {
  char output[10] = "SCORE:   ";
  itoa(score, &output[6], 10);
  LCD_DrawString(x, y, 0xFFFF, 0x0000, output, 16, 0);
  char output_2[10] = "LIVES:   ";
  itoa(lives, &output_2[7], 10);
  LCD_DrawString(140, 0, 0xFFFF, 0x0000, output_2, 16, 0);
}

int main(void) {
  init_nunchuk();
  init_tim6();
  setup_music();
  start_music();
  LCD_Setup();
  LCD_Clear(0x0);

  // Setup randomness
  srand(get_ent());

  start_screen();
  for (;;) {
    init_game_data();
    game_loop();
    clear_invaders();
    clear_sprite(&ufo_object.s);
    end_screen();
  }
}

void init_game_data() {
  LCD_Clear(0x0);
  init_sprite(120, 25, tank_clean_width, tank_clean_height,
              (uint16_t *)tank_clean, (uint16_t *)tank_clean, &player);
  init_sprite(1000, 1000, tank_shot_width, tank_shot_height,
              (uint16_t *)tank_shot, (uint16_t *)tank_shot, &shot);
  init_4_bunkers();

  // setup five bolts
  init_bolts();

  // Initialize the invader army
  init_invaders(0, 230, 5, 2);

  // Init the UFO
  init_ufo();

  start = 0;
  lives = 3;
  all_dead = 0;
  inv_comp = 0;
  score = 0;
  start_music();
}

void start_screen() {
  LCD_DrawString(165, 250, 0x0F00, 0x0000, "SPACE INVADERS", 16, 0);
  LCD_DrawString(180, 200, 0x0F00, 0x0000, "PRESS (C) TO START!", 16, 0);
  while (!(start)) {
    if (flg_c) {
      start = 1;
    }
  }
  // Clear the text
  LCD_DrawString(165, 250, 0x0F00, 0x0000, "              ", 16, 0);
  LCD_DrawString(180, 200, 0x0F00, 0x0000, "                   ", 16, 0);
}

void game_loop() {
  while (!(all_dead) && lives > 0 && !(inv_comp)) {
    if ((glbcnt + 1) % 15 == 0) {
      // -----4FPS (BASICALLY FREE)-----
      draw_score(140, 300);
      handle_bolt_animate();
      // Animate the army
      update_invaders();
      if(ufo_pop) {
        ufo_pop = 0;
        clear_sprite(&ufo_explosion);
      }
    }

    // if ((glbcnt + 1) % 4 == 0) {
    //  // -----15FPS-----
    //  //      print_flags(175, 1);
    //  //        print_glbcnt(230,1);
    //}

    if ((glbcnt + 1) % 2 == 0) {
      // -----30FPS-----
      handle_player_input();
      handle_shot();
      handle_invaders();
      update_ufo();
    }

    // while ((glbcnt + 1) % 1 != 0)
    //  ;
    asm volatile("wfi" ::);
  }
}

void end_screen() {
  for (;;) {
    // For ending
    pause_music();
    draw_score(140, 300);
    if (lives <= 0 || inv_comp == 1) {
      LCD_DrawString(180, 250, 0xF000, 0x0000, "GAME OVER YOU LOSE!", 16, 0);
      LCD_DrawString(190, 200, 0xF000, 0x0000, "PRESS (C) TO RE-START", 16, 0);
    }
    if (all_dead) {
      LCD_DrawString(180, 250, 0x0F00, 0x0000, "GAME OVER YOU WON!", 16, 0);
      LCD_DrawString(190, 200, 0x0F00, 0x0000, "PRESS (C) TO RE-START", 16, 0);
    }
    if (flg_c) {
      break;
    }
  }
}

void handle_player_input() {
  if (flg_mv_right && player.bbox.x1 > 0) {
    move_sprite(&player, -2, 0, 0);
  } else if (flg_mv_left && player.bbox.x2 < (LCD_W - 1)) {
    move_sprite(&player, 2, 0, 0);
  } else {
    draw_sprite(&player);
  }
  if (flg_v && (shot.bbox.x1 == 1000)) {
    teleport_sprite((int)((player.bbox.x2 + player.bbox.x1) / 2),
                    player.bbox.y1 + 10, &shot);
    draw_sprite(&shot);
  }
}

void handle_shot() {
  if (shot.bbox.y2 >= (LCD_H - SHOT_SPEED)) {
    teleport_sprite(1000, 1000, &shot);
  } else if (shot.bbox.x1 != 1000) {
    move_sprite(&shot, 0, SHOT_SPEED, 0);
  }

  // Collision test
  if (invader_coll(&shot)) {
    teleport_sprite(1000, 1000, &shot);
  }

  if (bunker_coll(&shot)) {
    teleport_sprite(1000, 1000, &shot);
  }

  if(sprite_coll(&shot, &ufo_object.s)) {
    ufo_object.active = 0;
    ufo_explosion.bbox = ufo_object.s.bbox;
    draw_sprite(&ufo_explosion);
    ufo_pop = 1;
    teleport_sprite(1000, 290, &ufo_object.s);
    score += ufo_object.points;
    if(ufo_object.dir) {
      ufo_object.dir = 0;
    } else {
      ufo_object.dir = 1;
    }
  }
}

void handle_invaders() {
  Sprite shooter;
  for (int i = 0; i < LIGHTNING_CNT; i++) {
    shooter = invader_army.units[(rand() % (INVADERS_COUNT - 0)) + 0];
    if (shooter.sprite_data != NULL) {
      if (bolts[i].bbox.x1 == 1000) {
        teleport_sprite((int)((shooter.bbox.x2 + shooter.bbox.x1) / 2),
                        shooter.bbox.y1 - 10, &bolts[i]);
      }
    }
    if (bolts[i].bbox.x1 != 1000) {
      move_sprite(&bolts[i], 0, -LIGHTNING_SPEED, 0);
    }
    if (bolts[i].bbox.y2 <= 25) {
      teleport_sprite(1000, 1000, &bolts[i]);
    }
    if (bunker_coll(&bolts[i])) {
      teleport_sprite(1000, 1000, &bolts[i]);
    }
    // Collision test (bumker)
    if (sprite_coll(&bolts[i], &player)) {
      teleport_sprite(1000, 1000, &bolts[i]);
      lives--;
    }
  }
  all_dead = 1;
  for (int i = 0; i < INVADERS_COUNT; i++) {
    if (invader_army.units[i].sprite_data != NULL) {
      all_dead = 0;
    }
  }
  if (invader_army.bbox.y1 < 5) {
    inv_comp = 1;
  }
}

void init_bolts() {
  for (int i = 0; i < LIGHTNING_CNT; i++) {
    init_sprite(1000, 1000, lightning_a_width, lightning_a_height,
                (uint16_t *)lightning_a, (uint16_t *)lightning_b, &bolts[i]);
  }
}

void handle_bolt_animate() {
  for (int i = 0; i < LIGHTNING_CNT; i++) {
    bolts[i].sprite_data = (uint16_t *)(((uint32_t)bolts[i].sprite_data) ^
                                        bolts[i].sprite_swap_key);
  }
}

uint32_t get_ent() {
  uint8_t buf[8] = {0};
  read_nunchuk(buf);
  return buf[2] << 24 | buf[3] << 16 | buf[4] << 8 | buf[6];
}
