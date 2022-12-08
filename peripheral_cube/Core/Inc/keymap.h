
#ifndef KEYMAP_H
#define KEYMAP_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define KEYBOARD_NCOLS 15
#define KEYBOARD_NROWS 5

enum hid_keyboard_keypad_usage {
    KC_NO = 0x00,
    KC_ROLL_OVER,
    KC_POST_FAIL,
    KC_UNDEFINED,
    KC_A,
    KC_B,
    KC_C,
    KC_D,
    KC_E,
    KC_F,
    KC_G,
    KC_H,
    KC_I,
    KC_J,
    KC_K,
    KC_L,
    KC_M, // 0x10
    KC_N,
    KC_O,
    KC_P,
    KC_Q,
    KC_R,
    KC_S,
    KC_T,
    KC_U,
    KC_V,
    KC_W,
    KC_X,
    KC_Y,
    KC_Z,
    KC_1,
    KC_2,
    KC_3, // 0x20
    KC_4,
    KC_5,
    KC_6,
    KC_7,
    KC_8,
    KC_9,
    KC_0,
    KC_ENTER,
    KC_ESCAPE,
    KC_BACKSPACE,
    KC_TAB,
    KC_SPACE,
    KC_MINUS,
    KC_EQUAL,
    KC_LEFT_BRACKET,
    KC_RIGHT_BRACKET, // 0x30
    KC_BACKSLASH,
    KC_NONUS_HASH,
    KC_SEMICOLON,
    KC_QUOTE,
    KC_GRAVE,
    KC_COMMA,
    KC_DOT,
    KC_SLASH,
    KC_CAPS_LOCK,
    KC_F1,
    KC_F2,
    KC_F3,
    KC_F4,
    KC_F5,
    KC_F6,
    KC_F7, // 0x40
    KC_F8,
    KC_F9,
    KC_F10,
    KC_F11,
    KC_F12,
    KC_PRINT_SCREEN,
    KC_SCROLL_LOCK,
    KC_PAUSE,
    KC_INSERT,
    KC_HOME,
    KC_PAGE_UP,
    KC_DELETE,
    KC_END,
    KC_PAGE_DOWN,
    KC_RIGHT,
    KC_LEFT, // 0x50
    KC_DOWN,
    KC_UP,
    KC_NUM_LOCK,
    KC_KP_SLASH,
    KC_KP_ASTERISK,
    KC_KP_MINUS,
    KC_KP_PLUS,
    KC_KP_ENTER,
    KC_KP_1,
    KC_KP_2,
    KC_KP_3,
    KC_KP_4,
    KC_KP_5,
    KC_KP_6,
    KC_KP_7,
    KC_KP_8, // 0x60
    KC_KP_9,
    KC_KP_0,
    KC_KP_DOT,
    KC_NONUS_BACKSLASH,
    KC_APPLICATION,
    KC_KB_POWER,
    KC_KP_EQUAL,
    KC_F13,
    KC_F14,
    KC_F15,
    KC_F16,
    KC_F17,
    KC_F18,
    KC_F19,
    KC_F20,
    KC_F21, // 0x70
    KC_F22,
    KC_F23,
    KC_F24,
    KC_EXECUTE,
    KC_HELP,
    KC_MENU,
    KC_SELECT,
    KC_STOP,
    KC_AGAIN,
    KC_UNDO,
    KC_CUT,
    KC_COPY,
    KC_PASTE,
    KC_FIND,
    KC_KB_MUTE,
    KC_KB_VOLUME_UP, // 0x80
    KC_KB_VOLUME_DOWN,
    KC_LOCKING_CAPS_LOCK,
    KC_LOCKING_NUM_LOCK,
    KC_LOCKING_SCROLL_LOCK,
    KC_KP_COMMA,
    KC_KP_EQUAL_AS400,
    KC_INTERNATIONAL_1,
    KC_INTERNATIONAL_2,
    KC_INTERNATIONAL_3,
    KC_INTERNATIONAL_4,
    KC_INTERNATIONAL_5,
    KC_INTERNATIONAL_6,
    KC_INTERNATIONAL_7,
    KC_INTERNATIONAL_8,
    KC_INTERNATIONAL_9,
    KC_LANGUAGE_1, // 0x90
    KC_LANGUAGE_2,
    KC_LANGUAGE_3,
    KC_LANGUAGE_4,
    KC_LANGUAGE_5,
    KC_LANGUAGE_6,
    KC_LANGUAGE_7,
    KC_LANGUAGE_8,
    KC_LANGUAGE_9,
    KC_ALTERNATE_ERASE,
    KC_SYSTEM_REQUEST,
    KC_CANCEL,
    KC_CLEAR,
    KC_PRIOR,
    KC_RETURN,
    KC_SEPARATOR,
    KC_OUT, // 0xA0
    KC_OPER,
    KC_CLEAR_AGAIN,
    KC_CRSEL,
    KC_EXSEL,

#if 0
  // ***************************************************************
  // These keycodes are present in the HID spec, but are           *
  // nonfunctional on modern OSes. QMK uses this range (0xA5-0xDF) *
  // for the media and function keys instead - see below.          *
  // ***************************************************************

  KC_KP_00                = 0xB0,
  KC_KP_000,
  KC_THOUSANDS_SEPARATOR,
  KC_DECIMAL_SEPARATOR,
  KC_CURRENCY_UNIT,
  KC_CURRENCY_SUB_UNIT,
  KC_KP_LEFT_PARENTHESIS,
  KC_KP_RIGHT_PARENTHESIS,
  KC_KP_LEFT_BRACE,
  KC_KP_RIGHT_BRACE,
  KC_KP_TAB,
  KC_KP_BACKSPACE,
  KC_KP_A,
  KC_KP_B,
  KC_KP_C,
  KC_KP_D,
  KC_KP_E,                //0xC0
  KC_KP_F,
  KC_KP_XOR,
  KC_KP_HAT,
  KC_KP_PERCENT,
  KC_KP_LESS_THAN,
  KC_KP_GREATER_THAN,
  KC_KP_AND,
  KC_KP_LAZY_AND,
  KC_KP_OR,
  KC_KP_LAZY_OR,
  KC_KP_COLON,
  KC_KP_HASH,
  KC_KP_SPACE,
  KC_KP_AT,
  KC_KP_EXCLAMATION,
  KC_KP_MEM_STORE,        //0xD0
  KC_KP_MEM_RECALL,
  KC_KP_MEM_CLEAR,
  KC_KP_MEM_ADD,
  KC_KP_MEM_SUB,
  KC_KP_MEM_MUL,
  KC_KP_MEM_DIV,
  KC_KP_PLUS_MINUS,
  KC_KP_CLEAR,
  KC_KP_CLEAR_ENTRY,
  KC_KP_BINARY,
  KC_KP_OCTAL,
  KC_KP_DECIMAL,
  KC_KP_HEXADECIMAL,
#endif

    /* Modifiers */
    KC_LEFT_CTRL = 0xE0,
    KC_LEFT_SHIFT,
    KC_LEFT_ALT,
    KC_LEFT_GUI,
    KC_RIGHT_CTRL,
    KC_RIGHT_SHIFT,
    KC_RIGHT_ALT,
    KC_RIGHT_GUI
};

enum internal_keyboard_vals  {
  KC_FN = KC_RIGHT_GUI + 1,
};

enum mouse_keys {
/* Mouse Buttons */
#ifdef VIA_ENABLE
    KC_MS_UP = 0xF0,
#else
    KC_MS_UP = 0xED,
#endif
    KC_MS_DOWN,
    KC_MS_LEFT,
    KC_MS_RIGHT, // 0xF0
    KC_MS_BTN1,
    KC_MS_BTN2,
    KC_MS_BTN3,
    KC_MS_BTN4,
    KC_MS_BTN5,
#ifdef VIA_ENABLE
    KC_MS_BTN6 = KC_MS_BTN5,
    KC_MS_BTN7 = KC_MS_BTN5,
    KC_MS_BTN8 = KC_MS_BTN5,
#else
    KC_MS_BTN6,
    KC_MS_BTN7,
    KC_MS_BTN8,
#endif

    /* Mouse Wheel */
    KC_MS_WH_UP,
    KC_MS_WH_DOWN,
    KC_MS_WH_LEFT,
    KC_MS_WH_RIGHT,

    /* Acceleration */
    KC_MS_ACCEL0,
    KC_MS_ACCEL1,
    KC_MS_ACCEL2 // 0xFF
};

uint8_t keymap_decode(enum hid_keyboard_keypad_usage kc);


#ifdef __cplusplus
}
#endif
#endif