#include "keyboard.h"

uint32_t col_state_buf_idx = 0;
uint8_t col_state[KEYBOARD_NCOLS][KEYBOARD_COL_STATE_LEN];

GPIO_TypeDef *ROW_BASE;
GPIO_TypeDef *COL_BASE;

// TODO Need to init a interrupt to handle reading the keymatrix
void init_keypad(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER &= ~0x3ff;
  GPIOE->MODER |= 0x15555555;
}

void scan_keymatrix(void) {
  for (uint8_t col = 0; col < KEYBOARD_NCOLS; col++) {
    // Set takes precedence over reset, so reset all columns off and turn on the
    // one we want
    COL_BASE->BSRR = ((1 << KEYBOARD_NCOLS) - 1) | (1 << col);
    // Magic number is mask to only get IDR we care about
    col_state[col][col_state_buf_idx] = ROW_BASE->IDR & 0x1f;
    col_state_buf_idx += 1;
    col_state_buf_idx &= (KEYBOARD_COL_STATE_LEN - 1);
  }
}

// Returns the number of keys that we've added to the storage
uint8_t decode_column(uint8_t col_idx, uint8_t col_mask, uint8_t rem_space,
                      uint8_t *storage) {
  uint8_t count = 0;
  for(int i = 0; i < KEYBOARD_NROWS; i++) {
    if (rem_space == 0) {
      break;
    }
    if ((col_mask & 1) == 1)  {
      *storage = key_map[col_idx][i];
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
    uint8_t *col_history = col_state[col_idx];
    uint8_t debounce_iter_cap = (KEYBOARD_COL_STATE_LEN + col_state_buf_idx) &
                                (KEYBOARD_COL_STATE_LEN - 1);
    uint8_t col_mask = 0xff;
    for (int j = col_state_buf_idx; j < debounce_iter_cap;
         j = (j + 1) & (KEYBOARD_COL_STATE_LEN - 1)) {
      col_mask &= col_history[j];
    }
    pressed_keys +=
        decode_column(col_idx, col_mask, max_press_keys - pressed_keys,
                      key_press_storage + pressed_keys);
    if (pressed_keys >= max_press_keys) {
      pressed_keys++;
      break;
    }
  }
  return (pressed_keys > max_press_keys) ? 1 : 0;
}
