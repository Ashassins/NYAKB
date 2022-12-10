#include "keyboard.h"

uint32_t debounce_buf_idx = 0;
uint8_t debounce_buffer[KEYBOARD_NCOLS][KEYBOARD_COL_STATE_LEN];

GPIO_TypeDef *ROW_BASE = GPIOA;
GPIO_TypeDef *COL_BASE = GPIOE;

// TODO Need to init a interrupt to handle reading the keymatrix
void init_keypad(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  ROW_BASE->MODER &= ~0x3ff;
  COL_BASE->MODER |= 0x15555555;
  for (int i = 0; i < KEYBOARD_NCOLS; i++) {
    uint8_t *cur_col = debounce_buffer[i];
    for (int j = 0; j < KEYBOARD_COL_STATE_LEN; j++) {
      cur_col[j] = 0;
    }
  }
}

void scan_keymatrix(void) {
  for (uint8_t col = 0; col < KEYBOARD_NCOLS; col++) {
    // Set takes precedence over reset, so reset all columns off and turn on the
    // one we want
    COL_BASE->BSRR = (((1 << KEYBOARD_NCOLS) - 1) << 16) | (1 << col);
    // Read a column into the state buffer
    debounce_buffer[col][debounce_buf_idx] = ROW_BASE->IDR & 0x1f;
  }
  // Increment idx into the state history, make sure to wrap around
  debounce_buf_idx += 1;
  debounce_buf_idx &= (KEYBOARD_COL_STATE_LEN - 1);
}

// Returns the number of keys that we've added to the storage
uint8_t decode_column(uint8_t col, uint8_t col_mask, uint8_t rem_space,
                      uint8_t *storage) {
  // Count the number of keys pressed in reality
  uint8_t count = 0;
  for (int row = 0; row < KEYBOARD_NROWS; row++) {
    if (rem_space == 0) {
      break;
    }
    // If we've a key pressed,
    // look up the keycode
    if ((col_mask & 1) == 1) {
      *storage = key_map[col][row];
      storage += 1;
      rem_space--;
      count++;
    }
    col_mask >>= 1;
  }
  return count;
}

uint8_t read_keypad(uint16_t max_press_keys, uint8_t *key_press_storage) {
  uint16_t pressed_keys = 0;
  for (uint32_t col_idx = 0; col_idx < KEYBOARD_NCOLS; col_idx++) {
    // Grab the row we currently care about
    uint8_t *col_history = debounce_buffer[col_idx];
    // Mask of rows that have been set for all entries in the debounce buffer
    uint8_t debounced_mask = 0xff;
    uint32_t mask_start_pos = (debounce_buf_idx + KEYBOARD_COL_STATE_LEN + 1) &
                              (KEYBOARD_COL_STATE_LEN - 1);
    for (int j = mask_start_pos, i = 0; i < KEYBOARD_DEBOUNCE_CYCLE;
         j = (j + 1) & (KEYBOARD_COL_STATE_LEN - 1), i++) {
      // If we care about that point in history,
      // and a key's bit wasn't set, then we clear in the mask
      debounced_mask &= col_history[j];
    }
    // Decode the debounced column and copy to storage
    pressed_keys +=
        decode_column(col_idx, debounced_mask, max_press_keys - pressed_keys,
                      key_press_storage + pressed_keys);
    if (pressed_keys >= max_press_keys) {
      pressed_keys++;
      break;
    }
  }
  return pressed_keys;
}
