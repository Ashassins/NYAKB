#ifndef __MSG_H
#define __MSG_H
#include <stdint.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef enum {
  String,
  KeyPresses,
} MsgType;

typedef struct {
  u8 sz;
  u8 buf[255];
} StrBuf;

// There are 66 keys...
typedef struct {
  u8 bit_map[9];
} Keys;

typedef struct  {
  MsgType type;
  union {
    StrBuf str;
    Keys keys;
  } u;
} Msg;

#endif
