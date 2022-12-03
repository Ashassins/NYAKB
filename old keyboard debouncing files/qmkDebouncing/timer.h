
#pragma once

#if __has_include_next("_timer.h")
#    include_next "_timer.h" /* Include the platform's _timer.h */
#endif

#include <stdint.h>

#define TIMER_DIFF(a, b, max) ((max == UINT8_MAX) ? ((uint8_t)((a) - (b))) : ((max == UINT16_MAX) ? ((uint16_t)((a) - (b))) : ((max == UINT32_MAX) ? ((uint32_t)((a) - (b))) : ((a) >= (b) ? (a) - (b) : (max) + 1 - (b) + (a)))))
#define TIMER_DIFF_8(a, b) TIMER_DIFF(a, b, UINT8_MAX)
#define TIMER_DIFF_16(a, b) TIMER_DIFF(a, b, UINT16_MAX)
#define TIMER_DIFF_32(a, b) TIMER_DIFF(a, b, UINT32_MAX)
#define TIMER_DIFF_RAW(a, b) TIMER_DIFF_8(a, b)

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t timer_count;

void     timer_init(void);
void     timer_clear(void);
uint16_t timer_read(void);
uint32_t timer_read32(void);
uint16_t timer_elapsed(uint16_t last);
uint32_t timer_elapsed32(uint32_t last);

// Utility functions to check if a future time has expired & autmatically handle time wrapping if checked / reset frequently (half of max value)
#define timer_expired(current, future) ((uint16_t)(current - future) < UINT16_MAX / 2)
#define timer_expired32(current, future) ((uint32_t)(current - future) < UINT32_MAX / 2)

// Use an appropriate timer integer size based on architecture (16-bit will overflow sooner)
#if FAST_TIMER_T_SIZE < 32
#    define TIMER_DIFF_FAST(a, b) TIMER_DIFF_16(a, b)
#    define timer_expired_fast(current, future) timer_expired(current, future)
typedef uint16_t fast_timer_t;
fast_timer_t inline timer_read_fast(void) {
    return timer_read();
}
fast_timer_t inline timer_elapsed_fast(fast_timer_t last) {
    return timer_elapsed(last);
}
#else
#    define TIMER_DIFF_FAST(a, b) TIMER_DIFF_32(a, b)
#    define timer_expired_fast(current, future) timer_expired32(current, future)
typedef uint32_t fast_timer_t;
fast_timer_t inline timer_read_fast(void) {
    return timer_read32();
}
fast_timer_t inline timer_elapsed_fast(fast_timer_t last) {
    return timer_elapsed32(last);
}
#endif

#ifdef __cplusplus
}
#endif
