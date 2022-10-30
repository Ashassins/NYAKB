#include <stdio.h>
#include <stdint.h>
#include <math.h>

int SAMPLES = 100;
uint16_t triangle_wave(int i) {
  return (uint16_t)(2048 + 1952 * (1 - fabs((float)i / SAMPLES - 0.5) * 4));
}

uint16_t sin_wave(int i) {
  return (uint16_t)(2048 + 1952 * sin(2 * M_PI * i / SAMPLES));
}

uint16_t idk_wave(int i) {
  return (uint16_t)(2048 + 1952 * (sin(2 * M_PI * i / SAMPLES) - .1 * sin(.25 * M_PI * i / SAMPLES + (M_PI / 2))));
}

int main() {
  printf("const uint16_t wavetable[SAMPLES] = {\n\t");
  for(int i = 0; i < SAMPLES; i++) {
    printf("%hu, ", idk_wave(i));
    if (i % 10 == 9) {
      printf("\n\t");
    }
  }
  printf("};\n");
}
