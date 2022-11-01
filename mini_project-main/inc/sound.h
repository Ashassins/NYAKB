#ifndef SOUND_H
#define SOUND_H
#include "stm32f0xx.h"
#include <math.h>
#include <stdint.h>
#include "notes.h"

extern const uint16_t wavetable[SAMPLES];

extern uint16_t melody_idx;

extern const uint16_t full_melody[];
// How long to hold each note for in ms
extern const uint16_t full_durations[];
extern const uint16_t full_melody_len;
void setup_music();
void start_music();
void pause_music();
void resume_music();


#endif
