#include "sound.h"

void init_sound_tables();
void init_dac();
void init_tim2();
void init_tim16();
void init_sound_dma();

uint16_t melody_idx;
uint16_t nxt_note, nxt_dur;
/*
 * Invokes setup procedures for music module
 */
void setup_music() {
  melody_idx = 0;
  //init_sound_tables();
  init_tim2();
  init_tim16();
  init_dac();
  init_sound_dma();
}

/*
 * Initialize all global values and start peripherals
 */
void start_music() {
  // Reset all of the global vars and enable the timers and dma
  // If we're starting from a state where the music stopped, restart it
  if(!(TIM2->CR1 & TIM_CR1_CEN)){
    melody_idx = 0;
  }
  DAC->CR |= DAC_CR_EN1;
  DMA1_Channel2->CCR |= DMA_CCR_EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  TIM16->CR1 |= TIM_CR1_CEN;
}

// Pause music peripherals
void pause_music() {
  // Pause all of the timers related to music
  TIM2->CR1 &= ~TIM_CR1_CEN;
  TIM16->CR1 &= ~TIM_CR1_CEN;
  DAC->CR &= ~DAC_CR_EN1;
  DMA1_Channel2->CCR &= ~DMA_CCR_EN;
}

// Restart music peripherals
void resume_music() {
  DAC->CR |= DAC_CR_EN1;
  DMA1_Channel2->CCR |= DMA_CCR_EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  TIM16->CR1 |= TIM_CR1_CEN;
}

/*
 * Initialize TIM2
 * this one will trigger the DMA and DAC for sound generation
 */
void init_tim2() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  // Disable timer
  TIM2->CR1 &= ~(TIM_CR1_CEN);
  // Clear MMS and enable Update event
  TIM2->CR2 &= ~(TIM_CR2_MMS);
  TIM2->CR2 |= TIM_CR2_MMS_1;
  // Set no prescale for max arr freq
  TIM2->PSC = 0;
  // Calculate arr value for desired freq
  TIM2->ARR = full_melody[0]; // -1 calculated in constant
  // Buffer ARR value
  TIM2->CR1 |= TIM_CR1_ARPE;
  // Enable dma
  TIM2->DIER |= TIM_DIER_UDE;
}

/*
 * Initialize TIM16
 * this timer is responsible for controlling note length,
 * it will change based on a value from one of the noteDurations arrays
 */
void init_tim16() {
  RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER &= ~GPIO_MODER_MODER9;
  GPIOC->MODER |= GPIO_MODER_MODER9_0;
  TIM16->CR1 &= ~TIM_CR1_CEN;

  TIM16->CR2 &= ~TIM_CR2_MMS;
  TIM16->CR2 |= TIM_CR2_MMS_1;

  TIM16->PSC = 48000 - 1;
  TIM16->ARR = (full_durations[0]) - 1; //- 1;
  TIM16->CR1 |= TIM_CR1_ARPE;
  TIM16->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] |= 1 << TIM16_IRQn;
}

void init_dac() {
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  // Configure which TRGO event to fire on and enable the dac
  DAC->CR &= ~(DAC_CR_TSEL1);
  DAC->CR |= DAC_CR_TSEL1_2;
}

void init_sound_dma() {
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;

  DMA_Channel_TypeDef *dma = DMA1_Channel2;
  // Disable DMA and clear msize and psize
  dma->CCR &= ~(DMA_CCR_EN | DMA_CCR_MSIZE | DMA_CCR_PSIZE);
  // We want to copy SAMPLES elements
  dma->CNDTR = SAMPLES;
  // from array
  dma->CMAR = (uint32_t) wavetable;
  // to the right aligned 12 bit dac
  dma->CPAR = (uint32_t) &DAC->DHR12R1;
  // Setup dma for circular, mem to periph, increment memory, set msize to 16
  // and psize to 16
  dma->CCR |= DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_MSIZE_0 |
              DMA_CCR_PSIZE_0;
}

/*
 * This interrupt handler changes notes
 * and changes the time till it's next trigger for the next note
 */
void TIM16_IRQHandler() {
  // Ack the interrupt
  TIM16->SR &= ~TIM_SR_UIF;
  // Status led for lazy debug
  GPIOC->BSRR = GPIO_BSRR_BR_9 | (GPIO_BSRR_BS_9 & ~(GPIOC->ODR));

  melody_idx += 1;
  if (melody_idx > full_melody_len) {
      melody_idx = 0;
  }
  if (melody_idx == 0) {
    nxt_note = full_melody[melody_idx];
  } else {
    nxt_note = full_melody[melody_idx-1];
  }
  nxt_dur = full_durations[melody_idx];

  // Needed to make sure we don't set the arr to zero on accident for a rest
  // note
  if (!nxt_note) {
    TIM2->CR1 &= ~(TIM_CR1_CEN);
  } else {
    TIM2->CR1 |= TIM_CR1_CEN;
  }

  // Set the pitch and duration values in the relavant timers
  TIM2->ARR = nxt_note;
  //printf("next_note : %f ", nxt_note);
  TIM16->ARR = nxt_dur - 1;
  //printf("next_dur : %f\n", nxt_dur);
}

const uint16_t wavetable[SAMPLES] = {
        96, 174, 252, 330, 408, 486, 564, 642, 720, 798,
        876, 954, 1032, 1111, 1189, 1267, 1345, 1423, 1501, 1579,
        1657, 1735, 1813, 1891, 1969, 2048, 2126, 2204, 2282, 2360,
        2438, 2516, 2594, 2672, 2750, 2828, 2906, 2984, 3063, 3141,
        3219, 3297, 3375, 3453, 3531, 3609, 3687, 3765, 3843, 3921,
        4000, 3921, 3843, 3765, 3687, 3609, 3531, 3453, 3375, 3297,
        3219, 3141, 3063, 2984, 2906, 2828, 2750, 2672, 2594, 2516,
        2438, 2360, 2282, 2204, 2126, 2048, 1969, 1891, 1813, 1735,
        1657, 1579, 1501, 1423, 1345, 1267, 1189, 1111, 1032, 954,
        876, 798, 720, 642, 564, 486, 408, 330, 252, 174,
};

const uint16_t full_melody[] = {// 19
    NOTE_E5,    NOTE_B4,    NOTE_C5,      NOTE_D5,
    NOTE_C5,    NOTE_B4,    NOTE_A4,      REST,
    NOTE_A4,
    NOTE_C5,    NOTE_E5,    NOTE_D5,      NOTE_C5,
    NOTE_B4,    NOTE_C5,    NOTE_D5,      NOTE_E5,
    NOTE_C5,    NOTE_A4,    REST,
    NOTE_A4, /**/ REST, // 19
    NOTE_D5,    NOTE_F5,    NOTE_A5,      NOTE_G5,
    NOTE_F5,    NOTE_E5,    NOTE_C5,      NOTE_E5,
    NOTE_D5,    NOTE_C5,    NOTE_B4,      REST,
    NOTE_B4,
    NOTE_C5,    NOTE_D5,    NOTE_E5,      NOTE_C5,
    NOTE_A4,    REST,
    NOTE_A4,/**/NOTE_E5,      NOTE_B4, // 21
    NOTE_C5,    NOTE_D5,    NOTE_E5,      NOTE_D5,
    NOTE_C5,    NOTE_B4,    NOTE_A4,      REST,
    NOTE_A4,
    NOTE_C5,    NOTE_E5,    NOTE_D5,      NOTE_C5,
    NOTE_B4,    NOTE_C5,    NOTE_D5,      NOTE_E5,
    NOTE_C5,    NOTE_A4,    REST,
    NOTE_A4, /**/ REST,   // 20
    NOTE_D5,    NOTE_F5,    NOTE_A5,      NOTE_G5,
    NOTE_F5,    NOTE_E5,    NOTE_C5,      NOTE_E5,
    NOTE_F5,    NOTE_E5,    NOTE_D5,      NOTE_C5,
    NOTE_B4,    NOTE_C5,    NOTE_D5,      NOTE_E5,
    NOTE_C5,    NOTE_A4,    REST,
    NOTE_A4, /**/ NOTE_E5, // 8
    NOTE_C5,    NOTE_D5,    NOTE_B4,      NOTE_C5,
    NOTE_A4,    NOTE_GS4,   NOTE_B4, /**/ NOTE_E5, //9
    NOTE_C5,    NOTE_D5,    NOTE_B4,      NOTE_C5,
    NOTE_E5,    NOTE_A5,    REST,
    NOTE_A5,    NOTE_GS5,   REST/**/};


const uint16_t full_durations[] = {
  QUARTER_NOTE,   EIGHTH_NOTE,    EIGHTH_NOTE,      QUARTER_NOTE,
  EIGHTH_NOTE,    EIGHTH_NOTE,    QUARTER_NOTE,     TOUNGED,
  EIGHTH_NOTE,
  EIGHTH_NOTE,    QUARTER_NOTE,   EIGHTH_NOTE,      EIGHTH_NOTE,
  DOT_QUART_NOTE, EIGHTH_NOTE,    QUARTER_NOTE,     QUARTER_NOTE,
  QUARTER_NOTE,   QUARTER_NOTE,   TOUNGED,
  HALF_NOTE, /**/   EIGHTH_NOTE,
  QUARTER_NOTE,   EIGHTH_NOTE,    QUARTER_NOTE,     EIGHTH_NOTE,
  EIGHTH_NOTE,    DOT_QUART_NOTE, EIGHTH_NOTE,      QUARTER_NOTE,
  EIGHTH_NOTE,    EIGHTH_NOTE,    QUARTER_NOTE,     TOUNGED,
  EIGHTH_NOTE,
  EIGHTH_NOTE,    QUARTER_NOTE,   QUARTER_NOTE,     QUARTER_NOTE,
  QUARTER_NOTE,   TOUNGED,
  HALF_NOTE, /**/ QUARTER_NOTE,     EIGHTH_NOTE,
  EIGHTH_NOTE,    EIGHTH_NOTE,    SIXTEENTH_NOTE,   SIXTEENTH_NOTE,
  EIGHTH_NOTE,    EIGHTH_NOTE,    QUARTER_NOTE,     TOUNGED,
  EIGHTH_NOTE,
  EIGHTH_NOTE,    QUARTER_NOTE,   EIGHTH_NOTE,      EIGHTH_NOTE,
  DOT_QUART_NOTE, EIGHTH_NOTE,    QUARTER_NOTE,     QUARTER_NOTE,
  QUARTER_NOTE,   QUARTER_NOTE,   TOUNGED,
  HALF_NOTE, /**/   EIGHTH_NOTE,
  QUARTER_NOTE,   EIGHTH_NOTE,    QUARTER_NOTE,     EIGHTH_NOTE,
  EIGHTH_NOTE,    DOT_QUART_NOTE, EIGHTH_NOTE,      EIGHTH_NOTE,
  SIXTEENTH_NOTE, SIXTEENTH_NOTE, EIGHTH_NOTE,      EIGHTH_NOTE,
  DOT_QUART_NOTE, EIGHTH_NOTE,    QUARTER_NOTE,     QUARTER_NOTE,
  QUARTER_NOTE,   QUARTER_NOTE,   TOUNGED,
  HALF_NOTE, /**/   HALF_NOTE,
  HALF_NOTE,      HALF_NOTE,      HALF_NOTE,        HALF_NOTE,
  HALF_NOTE,      HALF_NOTE,      HALF_NOTE, /**/   HALF_NOTE,
  HALF_NOTE,      HALF_NOTE,      HALF_NOTE,        QUARTER_NOTE,
  QUARTER_NOTE,   QUARTER_NOTE,   TOUNGED,
  QUARTER_NOTE,    WHOLE_NOTE,     SIXTEENTH_NOTE/**/};

const uint16_t full_melody_len = sizeof(full_melody) / sizeof(full_melody[0]);
