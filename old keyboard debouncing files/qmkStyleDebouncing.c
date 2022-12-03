#include <stdlib.h>
#include <string.h>

#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif

#if DEBOUNCE > 127
#    undef DEBOUNCE
#    define DEBOUNCE 127
#endif

// #if DEBOUNCE > 0
// static debounce_counter_t *debounce_counters;
// static fast_timer_t        last_time;
// static bool                counters_need_update;
// static bool                matrix_need_update;
// static bool                cooked_changed;

#    define DEBOUNCE_ELAPSED 0

static void update_debounce_counters_and_transfer_if_expired(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, int8_t num_cols, uint8_t elapsed_time);
static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, int8_t num_cols);

typedef struct {
    bool    pressed : 1;
    uint8_t time : 7;
} debounce_counter_t;


void debounce_free(void) {
    free(debounce_counters);
    debounce_counters = NULL;
}

void debounce_init(uint8_t num_rows, int8_t num_cols) {
    debounce_counters = malloc(num_rows * MATRInum_colsX_COLS * sizeof(debounce_counter_t));
    int i = 0;
    for (uint8_t r = 0; r < num_rows; r++) {
        for (uint8_t c = 0; c < num_cols; c++) {
            debounce_counters[i++].time = DEBOUNCE_ELAPSED;
        }
    }
}

bool debounce(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, int8_t num_cols, bool changed) {
    bool updated_last = false;
    cooked_changed    = false;

    if (counters_need_update) {
        fast_timer_t now          = timer_read_fast();
        fast_timer_t elapsed_time = TIMER_DIFF_FAST(now, last_time);

        last_time    = now;
        updated_last = true;
        if (elapsed_time > UINT8_MAX) {
            elapsed_time = UINT8_MAX;
        }

        if (elapsed_time > 0) {
            update_debounce_counters_and_transfer_if_expired(raw, cooked, num_rows, num_cols, elapsed_time);
        }
    }

    if (changed || matrix_need_update) {
        if (!updated_last) {
            last_time = timer_read_fast();
        }

        transfer_matrix_values(raw, cooked, num_rows, num_cols);
    }

    return cooked_changed;
}

static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, int8_t num_cols) {
    debounce_counter_t *debounce_pointer = debounce_counters;

    for (uint8_t row = 0; row < num_rows; row++) {
        matrix_row_t delta = raw[row] ^ cooked[row];
        for (uint8_t col = 0; col < num_cols; col++) {
            matrix_row_t col_mask = (ROW_SHIFTER << col);

            if (delta & col_mask) {
                if (debounce_pointer->time == DEBOUNCE_ELAPSED) {
                    debounce_pointer->pressed = (raw[row] & col_mask);
                    debounce_pointer->time    = DEBOUNCE;
                    counters_need_update      = true;

                    if (debounce_pointer->pressed) {
                        // key-down: eager
                        cooked[row] ^= col_mask;
                        cooked_changed = true;
                    }
                }
            } else if (debounce_pointer->time != DEBOUNCE_ELAPSED) {
                if (!debounce_pointer->pressed) {
                    // key-up: defer
                    debounce_pointer->time = DEBOUNCE_ELAPSED;
                }
            }
            debounce_pointer++;
        }
    }
}

#else
#    include "none.c"
#endif