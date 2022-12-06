#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "stm32f4xx.h"
#include "stdint.h"
#include "keymap.h"


// Should be a power of 2 to make bit math nice
#define KEYBOARD_COL_STATE_LEN 4
#define KEYBOARD_DEBOUNCE_CYCLE 2

extern uint8_t col_state[KEYBOARD_NCOLS][KEYBOARD_COL_STATE_LEN];

void init_keypad();

// Returns 0 if no keys remain, return 1 if keys remain
uint8_t read_keypad(uint16_t max_press_keys, uint8_t *key_press_storage);

#endif // KEYBOARD_H
