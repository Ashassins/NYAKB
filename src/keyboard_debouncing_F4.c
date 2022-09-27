#include "stm32f0xx.h"
#include <string.h> // for memset() declaration
#include <math.h>   // for MA_PI

void enable_ports(void) {
  // Enable RCC to GPIOB and GPIOC
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  // GPIOB PB0-10 Outputs
  GPIOB->MODER &= ~0x3FFFFF; // Clear PB0-10
  GPIOB->MODER |= 0x155555;

  // GPIOC PC4-7 Outputs
  GPIOC->MODER &= ~0xFF00;
  GPIOC->MODER |= 0x5500;

  // GPIOC PC0-3 Inputs
  GPIOC->MODER &= ~0xFF;
  // GPIOC PC0-3 Pull Down
  GPIOC->PUPDR &= ~0xFF;
  GPIOC->PUPDR |= 0xAA;
}

//===========================================================================
// 2.2 Configuring DMA transfers
//===========================================================================
uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };
extern const char font[];

void setup_dma(void) {
	// Enable RCC to DMA
	// need to use a stream number 0-7 for DMA_SxCR
	// need to change these constants
	RCC->AHB1ENR |= RCC_AHB1ENR_DMAEN; // DMA 1 EN on 21
	DMA1_Channel2->S2CR &= ~DMA_CCR_EN;	// Turn off DMA Channel before config
	DMA1_Channel2->S2PAR = (uint32_t) &(GPIOB->ODR); //hmmmm
	DMA1_Channel2->S2M0AR = (uint32_t) msg; // memeory 0 addr?
//	DMA1_Channel2->S2M1AR = (uint32_t) msg; // memmory 1 addr?
	DMA1_Channel2->S2NDTR = 8;
	DMA1_Channel2->S2CR |= DMA_CCR_DIR;
	DMA1_Channel2->S2CR |= DMA_CCR_MINC;
	DMA1_Channel2->S2CR |= DMA_CCR_MSIZE_0;
	DMA1_Channel2->S2CR |= DMA_CCR_PSIZE_0;
	DMA1_Channel2->S2CR |= DMA_CCR_CIRC;
}

void enable_dma(void) {
	// Enable DMA
	DMA1_Channel2->S2CR |= DMA_CCR_EN;
}

void init_tim2(void) {
	// Enable RCC to tim2
	RCC->APB1ENR |= 0x1; //TIM2EN on 0
	// Set PSC to 12000-1 and ARR to 4-1
	TIM2->PSC = 12000-1;
	TIM2->ARR = 4-1;
	// Enable UDE in DIER
	TIM2->DIER |= TIM_DIER_UDE;
	// Enable CEN bit in Tim2
	TIM2->CR1 |= TIM_CR1_CEN;
}

void append_display(char val) {
	for (int i = 1; i < sizeof(msg); i++) {
		msg[i-1] &= ~0xff;
		msg[i-1] |= (msg[i] & 0xff);
	}
	msg[7] &= ~0xff;
	msg[7] |= val;
}

//===========================================================================
// 2.3 Debouncing a Keypad
//===========================================================================
const char keymap[] = "DCBA#9630852*741";
uint8_t hist[16];
uint8_t col;
char queue[2];
int qin;
int qout;

void drive_column(int c)
{
  GPIOC->BSRR = 0xf00000 | (1 << (c + 4));
}

int read_rows()
{
  return GPIOC->IDR & 0xf;
}

void push_queue(int n) {
  n = (n & 0xff) | 0x80;
  queue[qin] = n;
  qin ^= 1;
}

uint8_t pop_queue() {
  uint8_t tmp = queue[qout] & 0x7f;
  queue[qout] = 0;
  qout ^= 1;
  return tmp;
}

void update_history(int c, int rows)
{
  for(int i = 0; i < 4; i++) {
    hist[4*c+i] = (hist[4*c+i]<<1) + ((rows>>i)&1);
    if (hist[4*c+i] == 1)
      push_queue(4*c+i);
  }
}

void init_tim6() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // on 4
  TIM6->PSC = 48 - 1;
  TIM6->ARR = 1000 - 1;
  TIM6->DIER |= TIM_DIER_UIE;
  TIM6->CR1 |= TIM_CR1_CEN;
  NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;
}

void TIM6_DAC_IRQHandler(void) {
  TIM6->SR &= ~TIM_SR_UIF;
  int rows = read_rows();
  update_history(col, rows);
  col = (col + 1) & 3;
  drive_column(col);
}

char get_keypress() {
  for(;;) {
    asm volatile ("wfi" : :);   // wait for an interrupt
    if (queue[qout] == 0)
      continue;
    return keymap[pop_queue()];
  }
}

int main(void) {
  init_tim6();
  int key = 0;
  for(;;) {
    char key = get_keypress();
  }
}

const char font[] = {
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x00, // 32: space
  0x86, // 33: exclamation
  0x22, // 34: double quote
  0x76, // 35: octothorpe
  0x00, // dollar
  0x00, // percent
  0x00, // ampersand
  0x20, // 39: single quote
  0x39, // 40: open paren
  0x0f, // 41: close paren
  0x49, // 42: asterisk
  0x00, // plus
  0x10, // 44: comma
  0x40, // 45: minus
  0x80, // 46: period
  0x00, // slash
  // digits
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67,
  // seven unknown
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  // Uppercase
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x6f, 0x76, 0x30, 0x1e, 0x00, 0x38, 0x00,
  0x37, 0x3f, 0x73, 0x7b, 0x31, 0x6d, 0x78, 0x3e, 0x00, 0x00, 0x00, 0x6e, 0x00,
  0x39, // 91: open square bracket
  0x00, // backslash
  0x0f, // 93: close square bracket
  0x00, // circumflex
  0x08, // 95: underscore
  0x20, // 96: backquote
  // Lowercase
  0x5f, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x6f, 0x74, 0x10, 0x0e, 0x00, 0x30, 0x00,
  0x54, 0x5c, 0x73, 0x7b, 0x50, 0x6d, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x6e, 0x00
};
