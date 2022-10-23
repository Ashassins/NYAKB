#ifndef NOTES_H
#define NOTES_H

#define SAMPLES 100

#define BPM 150

#define WHOLE_NOTE 240 * 1000 / BPM
#define HALF_NOTE 120 * 1000 / BPM
#define QUARTER_NOTE 60 * 1000 / BPM
#define EIGHTH_NOTE 30 * 1000 / BPM
#define SIXTEENTH_NOTE 15 * 1000 / BPM
#define DOT_QUART_NOTE 90 * 1000 / BPM
#define DOT_EIGHTH_NOTE 45 * 1000 / BPM
#define DOT_SIXTEENTH_NOTE 22.5 * 1000 / BPM
#define TRIP_QUARTER_NOTE 40 * 1000 / BPM
#define TRIP_EIGHTH_NOTE 20 * 1000 / BPM
#define TRIP_SIXTEENTH_NOTE 10 * 1000 / BPM
#define TOUNGED 5 * 1000 / BPM

#define CLOCK_FREQ 48000000
#define REST 0
#define NOTE_B0 (CLOCK_FREQ / (31 * SAMPLES)) - 1
#define NOTE_C1 (CLOCK_FREQ / (33 * SAMPLES)) - 1
#define NOTE_CS1 (CLOCK_FREQ / (35 * SAMPLES)) - 1
#define NOTE_D1 (CLOCK_FREQ / (37 * SAMPLES)) - 1
#define NOTE_DS1 (CLOCK_FREQ / (39 * SAMPLES)) - 1
#define NOTE_E1 (CLOCK_FREQ / (41 * SAMPLES)) - 1
#define NOTE_F1 (CLOCK_FREQ / (44 * SAMPLES)) - 1
#define NOTE_FS1 (CLOCK_FREQ / (46 * SAMPLES)) - 1
#define NOTE_G1 (CLOCK_FREQ / (49 * SAMPLES)) - 1
#define NOTE_GS1 (CLOCK_FREQ / (52 * SAMPLES)) - 1
#define NOTE_A1 (CLOCK_FREQ / (55 * SAMPLES)) - 1
#define NOTE_AS1 (CLOCK_FREQ / (58 * SAMPLES)) - 1
#define NOTE_B1 (CLOCK_FREQ / (62 * SAMPLES)) - 1
#define NOTE_C2 (CLOCK_FREQ / (65 * SAMPLES)) - 1
#define NOTE_CS2 (CLOCK_FREQ / (69 * SAMPLES)) - 1
#define NOTE_D2 (CLOCK_FREQ / (73 * SAMPLES)) - 1
#define NOTE_DS2 (CLOCK_FREQ / (78 * SAMPLES)) - 1
#define NOTE_E2 (CLOCK_FREQ / (82 * SAMPLES)) - 1
#define NOTE_F2 (CLOCK_FREQ / (87 * SAMPLES)) - 1
#define NOTE_FS2 (CLOCK_FREQ / (93 * SAMPLES)) - 1
#define NOTE_G2 (CLOCK_FREQ / (98 * SAMPLES)) - 1
#define NOTE_GS2 (CLOCK_FREQ / (104 * SAMPLES)) - 1
#define NOTE_A2 (CLOCK_FREQ / (110 * SAMPLES)) - 1
#define NOTE_AS2 (CLOCK_FREQ / (117 * SAMPLES)) - 1
#define NOTE_B2 (CLOCK_FREQ / (123 * SAMPLES)) - 1
#define NOTE_C3 (CLOCK_FREQ / (131 * SAMPLES)) - 1
#define NOTE_CS3 (CLOCK_FREQ / (139 * SAMPLES)) - 1
#define NOTE_D3 (CLOCK_FREQ / (147 * SAMPLES)) - 1
#define NOTE_DS3 (CLOCK_FREQ / (156 * SAMPLES)) - 1
#define NOTE_E3 (CLOCK_FREQ / (165 * SAMPLES)) - 1
#define NOTE_F3 (CLOCK_FREQ / (175 * SAMPLES)) - 1
#define NOTE_FS3 (CLOCK_FREQ / (185 * SAMPLES)) - 1
#define NOTE_G3 (CLOCK_FREQ / (196 * SAMPLES)) - 1
#define NOTE_GS3 (CLOCK_FREQ / (208 * SAMPLES)) - 1
#define NOTE_A3 (CLOCK_FREQ / (220 * SAMPLES)) - 1
#define NOTE_AS3 (CLOCK_FREQ / (233 * SAMPLES)) - 1
#define NOTE_B3 (CLOCK_FREQ / (247 * SAMPLES)) - 1
#define NOTE_C4 (CLOCK_FREQ / (262 * SAMPLES)) - 1
#define NOTE_CS4 (CLOCK_FREQ / (277 * SAMPLES)) - 1
#define NOTE_D4 (CLOCK_FREQ / (294 * SAMPLES)) - 1
#define NOTE_DS4 (CLOCK_FREQ / (311 * SAMPLES)) - 1
#define NOTE_E4 (CLOCK_FREQ / (330 * SAMPLES)) - 1
#define NOTE_F4 (CLOCK_FREQ / (349 * SAMPLES)) - 1
#define NOTE_FS4 (CLOCK_FREQ / (370 * SAMPLES)) - 1
#define NOTE_G4 (CLOCK_FREQ / (392 * SAMPLES)) - 1
#define NOTE_GS4 (CLOCK_FREQ / (415 * SAMPLES)) - 1
#define NOTE_A4 (CLOCK_FREQ / (440 * SAMPLES)) - 1
#define NOTE_AS4 (CLOCK_FREQ / (466 * SAMPLES)) - 1
#define NOTE_B4 (CLOCK_FREQ / (494 * SAMPLES)) - 1
#define NOTE_C5 (CLOCK_FREQ / (523 * SAMPLES)) - 1
#define NOTE_CS5 (CLOCK_FREQ / (554 * SAMPLES)) - 1
#define NOTE_D5 (CLOCK_FREQ / (587 * SAMPLES)) - 1
#define NOTE_DS5 (CLOCK_FREQ / (622 * SAMPLES)) - 1
#define NOTE_E5 (CLOCK_FREQ / (659 * SAMPLES)) - 1
#define NOTE_F5 (CLOCK_FREQ / (698 * SAMPLES)) - 1
#define NOTE_FS5 (CLOCK_FREQ / (740 * SAMPLES)) - 1
#define NOTE_G5 (CLOCK_FREQ / (784 * SAMPLES)) - 1
#define NOTE_GS5 (CLOCK_FREQ / (831 * SAMPLES)) - 1
#define NOTE_A5 (CLOCK_FREQ / (880 * SAMPLES)) - 1
#define NOTE_AS5 (CLOCK_FREQ / (932 * SAMPLES)) - 1
#define NOTE_B5 (CLOCK_FREQ / (988 * SAMPLES)) - 1
#define NOTE_C6 (CLOCK_FREQ / (1047 * SAMPLES)) - 1
#define NOTE_CS6 (CLOCK_FREQ / (1109 * SAMPLES)) - 1
#define NOTE_D6 (CLOCK_FREQ / (1175 * SAMPLES)) - 1
#define NOTE_DS6 (CLOCK_FREQ / (1245 * SAMPLES)) - 1
#define NOTE_E6 (CLOCK_FREQ / (1319 * SAMPLES)) - 1
#define NOTE_F6 (CLOCK_FREQ / (1397 * SAMPLES)) - 1
#define NOTE_FS6 (CLOCK_FREQ / (1480 * SAMPLES)) - 1
#define NOTE_G6 (CLOCK_FREQ / (1568 * SAMPLES)) - 1
#define NOTE_GS6 (CLOCK_FREQ / (1661 * SAMPLES)) - 1
#define NOTE_A6 (CLOCK_FREQ / (1760 * SAMPLES)) - 1
#define NOTE_AS6 (CLOCK_FREQ / (1865 * SAMPLES)) - 1
#define NOTE_B6 (CLOCK_FREQ / (1976 * SAMPLES)) - 1
#define NOTE_C7 (CLOCK_FREQ / (2093 * SAMPLES)) - 1
#define NOTE_CS7 (CLOCK_FREQ / (2217 * SAMPLES)) - 1
#define NOTE_D7 (CLOCK_FREQ / (2349 * SAMPLES)) - 1
#define NOTE_DS7 (CLOCK_FREQ / (2489 * SAMPLES)) - 1
#define NOTE_E7 (CLOCK_FREQ / (2637 * SAMPLES)) - 1
#define NOTE_F7 (CLOCK_FREQ / (2794 * SAMPLES)) - 1
#define NOTE_FS7 (CLOCK_FREQ / (2960 * SAMPLES)) - 1
#define NOTE_G7 (CLOCK_FREQ / (3136 * SAMPLES)) - 1
#define NOTE_GS7 (CLOCK_FREQ / (3322 * SAMPLES)) - 1
#define NOTE_A7 (CLOCK_FREQ / (3520 * SAMPLES)) - 1
#define NOTE_AS7 (CLOCK_FREQ / (3729 * SAMPLES)) - 1
#define NOTE_B7 (CLOCK_FREQ / (3951 * SAMPLES)) - 1
#define NOTE_C8 (CLOCK_FREQ / (4186 * SAMPLES)) - 1
#define NOTE_CS8 (CLOCK_FREQ / (4435 * SAMPLES)) - 1
#define NOTE_D8 (CLOCK_FREQ / (4699 * SAMPLES)) - 1
#define NOTE_DS8 (CLOCK_FREQ / (4978 * SAMPLES)) - 1


#endif