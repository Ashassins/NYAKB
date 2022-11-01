#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

typedef struct {
  // Bottom right corner
  uint16_t x1, y1;
  // Top left corner
  uint16_t x2, y2;
} Rect;

void compute_hull(Rect a, Rect b, Rect *rv);

// Returns 1 if there is an overlap
uint8_t overlap(Rect a, Rect b);

// Returns 1 if x,y is in Rect r
uint8_t contains(uint16_t x, uint16_t y, Rect r);

void milli_wait(uint32_t ms);

uint32_t umax(uint32_t a, uint32_t b);
uint32_t umin(uint32_t a, uint32_t b);

#endif
