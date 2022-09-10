#include "stm32f0xx.h"
#include "string.h"

// Be sure to change this to your login...
const char login[] = "subbiah";

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}


//===========================================================================
// 2.1 FROM LAB 6
// ..........................................................................
// Configuring GPIO
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
// Debouncing a Keypad
//===========================================================================
const char keymap[] = "DCBA#9630852*741";
uint8_t hist[16];
uint8_t col;
char queue[2];
int qin;
int qout;

void drive_column(int c) {
    GPIOC->BSRR = 0xf00000 | (1 << (c + 4));
}

int read_rows() {
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

void update_history(int c, int rows) {
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
// This Lab 7
// ..........................................................................
// 2.2 Bit Bang SPI LED Array
//===========================================================================
int msg_index = 0;
uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };
extern const char font[];

void append_display(char val) {
    for (int i = 1; i < sizeof(msg); i++) {
        msg[i-1] &= ~0xff;
        msg[i-1] |= (i-1)<<8;
        msg[i-1] |= (msg[i] & 0xff);
    }
    msg[7] &= ~0xff;
    msg[7] |= 7<<8;
    msg[7] |= val;
}
void setup_bb(void) {
    // Enable RCC to GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // Clear PB12,13,15
    GPIOB->MODER &= ~0xCF000000; // Clear PB0-10
    // GPIOB PB12,13,15 Output
    GPIOB->MODER |= 0x45000000;
    // NSS high SCK low
    GPIOB->ODR |= 1<<12;
    GPIOB->ODR &= ~(1<<13);
}

void small_delay(void) {
    //nano_wait(5000000);
}

void bb_write_bit(int val) {
    if (val == 0) {
        // Set MOSI low
        GPIOB->ODR &= ~(1<<15);
    } else {
        // Set MOSI high
        GPIOB->ODR |= 1<<15;
    }
    small_delay();
    GPIOB->ODR |= 1<<13; // Set SCK High
    small_delay();
    GPIOB->ODR &= ~(1<<13); // Set SCK Low
}

void bb_write_halfword(int halfword) {
    GPIOB->ODR &= ~(1<<12); // Deassert NSS Pin;
    for (int i = 15; i >= 0; i--) {
        bb_write_bit(halfword & (1<<i)); // bit i
    }
    GPIOB->ODR |= 1<<12; // Assert NSS Pin;
}

void init_tim7(void) {
    RCC->APB1ENR |= 1<<5;
    TIM7->PSC = 4800-1;
    TIM7->ARR = 10-1;
    TIM7->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] = 1<<TIM7_IRQn;
    TIM7->CR1 |= TIM_CR1_CEN;
}

void TIM7_IRQHandler(void) {
    // Acknowledge Interrupt
    TIM7->SR &= ~TIM_SR_UIF;
    bb_write_halfword(msg[msg_index]);
    if (msg_index == 7) {
        msg_index = 0;
    } else {
        msg_index++;
    }
}


//===========================================================================
// 2.3 SPI DMA LED Array
//===========================================================================
void init_spi2(void) {
    // Enable RCC to SPI2
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    // Enable GPIO Alternate function for PB12,13,15
    // Enable RCC to GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // Clear PB12,13,15
    GPIOB->MODER &= ~0xCF000000; // Clear PB0-10
    // GPIOB PB12,13,15 Alternate Function
    GPIOB->MODER |= 0x8a000000;
    // PB12: AF0
    GPIOB->AFR[1] &= ~(0xf << (4*(12-8)));
    GPIOB->AFR[1] |=   0x0 << (4*(12-8));
    // PB13: AF0
    GPIOB->AFR[1] &= ~(0xf << (4*(13-8)));
    GPIOB->AFR[1] |=   0x0 << (4*(13-8));
    // PB15: AF0
    GPIOB->AFR[1] &= ~(0xf << (4*(15-8)));
    GPIOB->AFR[1] |=   0x0 << (4*(15-8));
    SPI2->CR1 &= ~SPI_CR1_SPE; // Clear CR1_SPE
    SPI2->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR; // Set baud rate 111
    // DS = 1111
    SPI2->CR2 = SPI_CR2_TXDMAEN | SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
//    SPI2->CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP;
//    SPI2->CR1 |= SPI_CR2_TXDMAEN;
    SPI2->CR1 |= SPI_CR1_SPE;
}

void setup_dma(void) {
    // Enable RCC to DMA
    RCC->AHBENR |= RCC_AHBENR_DMAEN;
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;  // Turn off DMA Channel before config
    DMA1_Channel5->CPAR = (uint32_t) &(SPI2->DR);
    DMA1_Channel5->CMAR = (uint32_t) msg;
    DMA1_Channel5->CNDTR = 8;
    DMA1_Channel5->CCR |= DMA_CCR_DIR;
    DMA1_Channel5->CCR |= DMA_CCR_MINC;
    DMA1_Channel5->CCR |= DMA_CCR_MSIZE_0;
    DMA1_Channel5->CCR |= DMA_CCR_PSIZE_0;
    DMA1_Channel5->CCR |= DMA_CCR_CIRC;
}

void enable_dma(void) {
    // Enable DMA
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}


//===========================================================================
// 2.4 SPI OLED Display
//===========================================================================
void setup_spi1() {
    // Enable RCC to SPI2
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // Enable GPIO Alternate function for PB12,13,15
    // Enable RCC to GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Clear PA5,6,7,15
    GPIOA->MODER &= ~0xc000fc00; // Clear  PA5,6,7,15
    // GPIOB PB12,13,15 Alternate Function
    GPIOA->MODER |= 0x8000a800;
//    // PB5: AF0
//    GPIOB->AFR[1] &= ~(0xf << (4*(5-8)));
//    GPIOB->AFR[1] |=   0x0 << (4*(5-8));
//    // PB6: AF0
//    GPIOB->AFR[1] &= ~(0xf << (4*(13-8)));
//    GPIOB->AFR[1] |=   0x0 << (4*(13-8));
//    // PB15: AF0
//    GPIOB->AFR[1] &= ~(0xf << (4*(15-8)));
//    GPIOB->AFR[1] |=   0x0 << (4*(15-8));
    SPI1->CR1 &= ~SPI_CR1_SPE; // Clear CR1_SPE
    SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR; // Set baud rate 111
    // DS = 1111
    SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
//    SPI2->CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP;
//    SPI2->CR1 |= SPI_CR2_TXDMAEN;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void spi_cmd(unsigned int data) {
    while((SPI1->SR & SPI_SR_TXE) == 0);
    SPI1->DR = data;
}

void spi_data(unsigned int data) {
    while((SPI1->SR & SPI_SR_TXE) == 0);
    SPI1->DR = (0x200 | data);
}

void spi_init_oled() {
    nano_wait(1000000);
    spi_cmd(0x38);
    spi_cmd(0x08);
    spi_cmd(0x01);
    nano_wait(2000000);
    spi_cmd(0x06);
    spi_cmd(0x02);
    spi_cmd(0x0c);
}

void spi_display1(const char *string) {
    spi_cmd(0x02);
    for (int i = 0; i < strlen(string); i++) {
//        if (string[i] != "\0") {
            spi_data(string[i]);
//        }
    }
}

void spi_display2(const char *string) {
    spi_cmd(0xc0);
    for (int i = 0; i < strlen(string); i++) {
//        if (string[i] != "\0") {
            spi_data(string[i]);
//        }
    }
}


//===========================================================================
// Main and supporting functions
//===========================================================================

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

    // GPIO enable
    enable_ports();

    // setup keypad
    init_tim6();

    // 2.2 LED array Bit Bang
//    setup_bb();
//    init_tim7();

    // 2.3 LED array SPI
     setup_dma();
     enable_dma();
     init_spi2();

    // 2.4 SPI OLED
     setup_spi1();
     spi_init_oled();
     spi_display1("Hello again,");
     spi_display2(login);

    int key = 0;
    for(;;) {
        char key = get_keypress();
        append_display(font[key]);
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
