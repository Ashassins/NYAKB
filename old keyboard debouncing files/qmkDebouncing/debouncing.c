#include "matrix.h"
#include "timer.h"
#include "quantum.h"
#include <string.h>
#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif

#if DEBOUNCE > 0
static bool         debouncing = false;
static fast_timer_t debouncing_time;

void debounce_init(uint8_t num_rows) {}

bool debounce(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, bool changed) {
    bool cooked_changed = false;

    if (changed) {
        debouncing      = true;
        debouncing_time = timer_read_fast();
    }

    if (debouncing && timer_elapsed_fast(debouncing_time) >= DEBOUNCE) {
        if (memcmp(cooked, raw, sizeof(matrix_row_t) * num_rows) != 0) {
            memcpy(cooked, raw, sizeof(matrix_row_t) * num_rows);
            cooked_changed = true;
        }
        debouncing = false;
    }

    return cooked_changed;
}

void debounce_free(void) {}
#else // no debouncing.
#    include "none.c"
#endif
