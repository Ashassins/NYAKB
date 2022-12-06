#include "keymap.h"
uint8_t key_map[KEYBOARD_NCOLS][KEYBOARD_NROWS] = {
  {KC_ESCAPE, KC_TAB, KC_CAPS_LOCK, KC_LEFT_SHIFT, KC_LEFT_CTRL},
  {KC_1, KC_Q, KC_A, KC_Z, KC_NO} ,
  {KC_2, KC_W, KC_S, KC_X, KC_LEFT_ALT},
  {KC_3, KC_E, KC_D, KC_C, KC_NO},
  {KC_4, KC_R, KC_F, KC_V, KC_SPACE},
  {KC_5, KC_T, KC_G, KC_B, KC_FN},
  {KC_6, KC_Y, KC_H, KC_N, KC_NO},
  {KC_7, KC_U, KC_J, KC_H, KC_SPACE},
  {KC_8, KC_I, KC_K, KC_COMMA, KC_NO},
  {KC_9, KC_O, KC_L, KC_DOT, KC_RIGHT_ALT},
  {KC_0, KC_P, KC_SEMICOLON, KC_SLASH, KC_NO},
  {KC_MINUS, KC_LEFT_BRACKET, KC_QUOTE, KC_RIGHT_SHIFT},
  {KC_EQUAL, KC_RIGHT_BRACKET, KC_ENTER, KC_LEFT},
  {KC_GRAVE, KC_BACKSPACE, KC_NO, KC_UP, KC_DOWN},
  {KC_BACKSLASH, KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3, KC_RIGHT},
};
