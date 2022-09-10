//=============================================================================
// ECE 362 lab experiment 6 -- Analog Input/Output and DMA
//=============================================================================

#include "stm32f0xx.h"
#include <string.h> // for memset() declaration
#include <math.h>   // for MA_PI

// Be sure to change this to your login...
const char login[] = "subbiah";


//===========================================================================
// 2.1 Configuring GPIO
//===========================================================================
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
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;	// Turn off DMA Channel before config
	DMA1_Channel2->CPAR = (uint32_t) &(GPIOB->ODR);
	DMA1_Channel2->CMAR = (uint32_t) msg;
	DMA1_Channel2->CNDTR = 8;
	DMA1_Channel2->CCR |= DMA_CCR_DIR;
	DMA1_Channel2->CCR |= DMA_CCR_MINC;
	DMA1_Channel2->CCR |= DMA_CCR_MSIZE_0;
	DMA1_Channel2->CCR |= DMA_CCR_PSIZE_0;
	DMA1_Channel2->CCR |= DMA_CCR_CIRC;
}

void enable_dma(void) {
	// Enable DMA
	DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void init_tim2(void) {
	/*
	// Enable RCC to tim7
	ldr  r0,=RCC
	ldr  r1,[r0,#APB1ENR]
	ldr  r2,=TIM7EN
	orrs r1,r2
	str  r1,[r0,#APB1ENR]

	// Set PSC to 4799 and ARR to 9
	ldr  r0,=TIM7
	ldr  r1,=4800-1
	str  r1,[r0,#TIM_PSC]

	ldr  r1,=10-1
	str  r1,[r0,#TIM_ARR]

	// Enable UIE in DIER
	ldr  r1,[r0,#TIM_DIER]
	ldr  r2,=TIM_DIER_UIE
	orrs r1,r2
	str  r1,[r0,#TIM_DIER]

	// Unmask interrupt
	ldr  r0,=NVIC
	ldr  r1,=ISER
	ldr  r2,=(1<<TIM7_IRQn)
	str  r2,[r0,r1]

	// Enable CEN bit in Tim7
	ldr  r0,=TIM7
	ldr  r1,[r0,#TIM_CR1]
	ldr  r2,=TIM_CR1_CEN
	orrs r1,r2
	str  r1,[r0,#TIM_CR1]

	bx   lr
	*/
	// Enable RCC to tim2
	RCC->APB1ENR |= 0x1;
	// Set PSC to 12000-1 and ARR to 4-1
	TIM2->PSC = 12000-1;
	TIM2->ARR = 4-1;
	// Enable UDE in DIER
	TIM2->DIER |= TIM_DIER_UDE;
//	// Unmask Interrupt
//	//NVIC->ISER = 1<<TIM2_IRQn;
//	NVIC->ISER[0] = 1<<TIM2_IRQn;
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
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
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

//===========================================================================
// 2.4 Reading an Analog Voltage
//===========================================================================
int volume = 2048;

void setup_adc_dma(void) {
	// Enable RCC to DMA
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	DMA1_Channel1->CCR &= ~DMA_CCR_EN;	// Turn off DMA Channel before config
	DMA1_Channel1->CPAR = (uint32_t) &(ADC1->DR);
	DMA1_Channel1->CMAR = (uint32_t) &(volume);
	DMA1_Channel1->CNDTR = 1;
	DMA1_Channel1->CCR &= ~DMA_CCR_DIR;
	DMA1_Channel1->CCR &= ~DMA_CCR_MINC;
	DMA1_Channel1->CCR &= ~DMA_CCR_PINC;
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_CIRC;
}

void enable_adc_dma(void) {
	DMA1_Channel1->CCR |= DMA_CCR_EN;
}

void setup_adc(void) {
	// Enable RCC to GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= 0xC; // Set pins PA1 to analog out
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR2 |= RCC_CR2_HSI14ON;
	while(!(RCC->CR2 & RCC_CR2_HSI14RDY));
	ADC1->CR |= ADC_CR_ADEN;
	while(!(ADC1->ISR & ADC_ISR_ADRDY));
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
	ADC1->CFGR1 |= ADC_CFGR1_DMACFG;
	ADC1->CHSELR = 0;
	ADC1->CHSELR |= 1<<1;
	while(!(ADC1->ISR & ADC_ISR_ADRDY));
	ADC1->CR |= ADC_CR_ADSTART;
}


//===========================================================================
// 2.5 Output Sine Wave
//===========================================================================
#define N 1000
#define RATE 20000
short int wavetable[N];
int step = 0;
int offset = 0;

void init_wavetable(void) {
	for(int i=0; i < N; i++)
		wavetable[i] = 32767 * sin(2 * M_PI * i / N);
}

void set_freq(float f) {
	step = f * N / RATE * (1<<16);
}

void setup_dac() {
	// Enable RCC to GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= 3<<(2*4); // Set pins PA4 to analog
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	DAC->CR |= DAC_CR_TSEL1;
	DAC->CR |= DAC_CR_TEN1;
	DAC->CR |= DAC_CR_EN1;
}

void init_tim7(void) {
//	// Enable RCC to tim2
//	RCC->APB1ENR |= 0x1;
//	// Set PSC to 12000-1 and ARR to 4-1
//	TIM2->PSC = 12000-1;
//	TIM2->ARR = 4-1;
//	// Enable UDE in DIER
//	TIM2->DIER |= TIM_DIER_UDE;
////	// Unmask Interrupt
////	//NVIC->ISER = 1<<TIM2_IRQn;
////	NVIC->ISER[0] = 1<<TIM2_IRQn;
//	// Enable CEN bit in Tim2
//	TIM2->CR1 |= TIM_CR1_CEN;
	RCC->APB1ENR |= 1<<5;
	TIM7->PSC = 1200-1;
	TIM7->ARR = 2-1;
	TIM7->DIER |= TIM_DIER_UIE;
	NVIC->ISER[0] = 1<<TIM7_IRQn;
	TIM7->CR1 |= TIM_CR1_CEN;
}

void TIM7_IRQHandler(void) {
//	ldr  r0,=TIM7			// Acknowledge interrupt
//	ldr  r1,[r0,#TIM_SR]
//	ldr  r2,=TIM_SR_UIF
//	bics r1,r2
//	str  r1,[r0,#TIM_SR]
	// Acknowledge Interrupt
	TIM7->SR &= ~TIM_SR_UIF;
	DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1; // Trigger DAC
	offset += step;
	if ((offset>>16) >= N) {
		offset -= N<<16;
	}
	int sample = wavetable[offset>>16];
	sample *= volume;
	sample = sample >> 16;
	sample += 2048;
	DAC->DHR12R1 = sample;
}


//===========================================================================
// Main and supporting functions
//===========================================================================
// Turn on the dot of the rightmost display element.
void dot()
{
    msg[7] |= 0x80;
}

// Read an entire floating-point number.
float getfloat()
{
    int num = 0;
    int digits = 0;
    int decimal = 0;
    int enter = 0;
    memset(msg,0,16);
    // clear_display();
    msg[7] = (7 << 8) | font['0'];
    while(!enter) {
        int key = get_keypress();
        if (digits == 8) {
            if (key != '#')
                continue;
        }
        switch(key) {
        case '0':
            if (digits == 0)
                continue;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            num = num*10 + key-'0';
            decimal <<= 1;
            digits += 1;
            if (digits == 1)
    		msg[7] = (7 << 8) | font[key];
                // display[7] = font[key];
            else
                append_display(font[key]);
                //shift(key);
            break;
        case '*':
            if (decimal == 0) {
                decimal = 1;
                dot();
            }
            break;
        case '#':
            enter = 1;
            break;
        default: continue; // ABCD
        }
    }
    float f = num;
    while (decimal) {
        decimal >>= 1;
        if (decimal)
            f = f/10.0;
    }
    return f;
}

int main(void)
{
    msg[0] |= font['E'];
    msg[1] |= font['C'];
    msg[2] |= font['E'];
    msg[3] |= font[' '];
    msg[4] |= font['3'];
    msg[5] |= font['6'];
    msg[6] |= font['2'];
    msg[7] |= font[' '];
    asm volatile ("bl autotest" : :);
    // 2.1
    // enable_ports();

    // 2.2
    // setup_dma();
    // enable_dma();
    // init_tim17();

    // 2.3
    // init_tim6();

    // 2.4
    // setup_adc_dma();
    // enable_adc_dma();
    // setup_adc();

    // 2.5
    // init_wavetable();
    // setup_dac();
    // init_tim7();

    int key = 0;
    for(;;) {
        char key = get_keypress();
        if (key == 'A')
            set_freq(getfloat());
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
