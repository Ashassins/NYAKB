/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include <stdio.h>

#define FIFOSIZE 16
char serfifo[FIFOSIZE];
int seroffset = 0;

void init_usart5() {
    // Enable RCC to GPIOC and GPIOD
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->AHBENR |= RCC_AHBENR_GPIODEN;

    // PC12 to USART5_TX
    GPIOC->MODER &= ~0x3000000;
    GPIOC->MODER |= 0x2000000;
    GPIOC->AFR[1] &= ~(0xf << (4*(12-8)));
    GPIOC->AFR[1] |=   0x2 << (4*(12-8));

    // PD2 to USART5_RX
    GPIOD->MODER &= ~0x30;
    GPIOD->MODER |= 0x20;
    GPIOD->AFR[0] &= ~(0xf << (4*(2)));
    GPIOD->AFR[0] |=   0x2 << (4*(2));

    // Enable RCC to USART5
    RCC->APB1ENR |= RCC_APB1ENR_USART5EN;

    // Configure USART5
    USART5->CR1 &= ~USART_CR1_UE;
//    USART5->CR1 &= ~ (USART_CR1_M | USART_CR1_M);
    USART5->BRR = 0x1A1;
    USART5->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART5->CR1 |= USART_CR1_UE;
    while (((USART5->ISR & USART_ISR_REACK) == 0) || (USART5->ISR & USART_ISR_TEACK) == 0);
}



//#define STEP21
#if defined(STEP21)
int main(void)
{
    init_usart5();
    for(;;) {
        while (!(USART5->ISR & USART_ISR_RXNE)) { }
        char c = USART5->RDR;
        while(!(USART5->ISR & USART_ISR_TXE)) { }
        USART5->TDR = c;
    }
}
#endif

//#define STEP22
#if defined(STEP22)
#include <stdio.h>
#include <syscalls.h>
int __io_putchar(int c) {
    if (c == '\n') {
        while(!(USART5->ISR & USART_ISR_TXE)) { }
        USART5->TDR = '\r';
    }
    while(!(USART5->ISR & USART_ISR_TXE)) { }
    USART5->TDR = c;
    return c;
}

int __io_getchar(void) {
     while (!(USART5->ISR & USART_ISR_RXNE)) { }
     char c = USART5->RDR;
     if (c == '\r') {
         c = '\n';
     }
     putchar(c);
     return c;
}

int main() {
    init_usart5();
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    printf("Enter your name: ");
    char name[80];
    fgets(name, 80, stdin);
    printf("Your name is %s", name);
    printf("Type any characters.\n");
    for(;;) {
        char c = getchar();
        putchar(c);
    }
}
#endif


//#define STEP23
#if defined(STEP23)
#include <stdio.h>
//#include <syscalls.h>
#include "fifo.h"
#include "tty.h"
int __io_putchar(int c) {
    if (c == '\n') {
        while(!(USART5->ISR & USART_ISR_TXE)) { }
        USART5->TDR = '\r';
    }
    while(!(USART5->ISR & USART_ISR_TXE)) { }
    USART5->TDR = c;
    return c;
}

int __io_getchar(void) {
//     while (!(USART5->ISR & USART_ISR_RXNE)) { }
//     char c = USART5->RDR;
//     if (c == '\r') {
//         c = '\n';
//     }
//     putchar(c);
//     return c;
    return line_buffer_getchar();
}

int main() {
    init_usart5();
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    printf("Enter your name: ");
    char name[80];
    fgets(name, 80, stdin);
    printf("Your name is %s", name);
    printf("Type any characters.\n");
    for(;;) {
        char c = getchar();
        putchar(c);
    }
}
#endif

#define STEP24
#if defined(STEP24)
#include <stdio.h>
//#include <syscalls.h>
#include "fifo.h"
#include "tty.h"

void enable_tty_interrupt() {
    // Enable interrupt on RXNE flag
    USART5->CR1 |= USART_CR1_RXNEIE;
    USART5->CR3 |= USART_CR3_DMAR;
    // Unmask USART RXNE Interrupt
    NVIC->ISER[0] |= 1 << USART3_8_IRQn;

    // Config and init DMA2 Channel2
    RCC->AHBENR |= RCC_AHBENR_DMA2EN;
    DMA2->RMPCR |= DMA_RMPCR2_CH2_USART5_RX;
    DMA2_Channel2->CCR &= ~DMA_CCR_EN;  // First make sure DMA is turned off
    DMA2_Channel2->CPAR = &(USART5->RDR);
    DMA2_Channel2->CMAR = serfifo;
    DMA2_Channel2->CNDTR = FIFOSIZE;
    DMA2_Channel2->CCR &= ~DMA_CCR_DIR; // read from Perif
    DMA2_Channel2->CCR &= ~DMA_CCR_TCIE; // no transfer complete interrupt
    DMA2_Channel2->CCR &= ~DMA_CCR_HTIE; // no half complete interrupt
    DMA2_Channel2->CCR &= ~DMA_CCR_MSIZE_0; // 8-bit
    DMA2_Channel2->CCR &= ~DMA_CCR_PSIZE_0; // 8-bit
    DMA2_Channel2->CCR |= DMA_CCR_MINC; // Increment memory address
    DMA2_Channel2->CCR &= ~DMA_CCR_PINC; // Don't increment perif address
    DMA2_Channel2->CCR |= DMA_CCR_CIRC; // circular transfer
    DMA2_Channel2->CCR &= ~DMA_CCR_MEM2MEM; // Disable mem to mem
    DMA2_Channel2->CCR |= DMA_CCR_PL; // Enable High priority
    DMA2_Channel2->CCR |= DMA_CCR_EN; // enable
}

int interrupt_getchar() {
//    USART_TypeDef *u = USART5;
    // Wait for a newline to complete the buffer.
    while(fifo_newline(&input_fifo) == 0) {
        asm volatile ("wfi"); // wait for an interrupt
//        insert_echo_char(u->RDR);
    }
    // Return a character from the line buffer.
    char ch = fifo_remove(&input_fifo);
    return ch;
}

void USART3_4_5_6_7_8_IRQHandler(void) {
    while(DMA2_Channel2->CNDTR != sizeof serfifo - seroffset) {
        if (!fifo_full(&input_fifo))
            insert_echo_char(serfifo[seroffset]);
        seroffset = (seroffset + 1) % sizeof serfifo;
    }
}

int __io_putchar(int c) {
    if (c == '\n') {
        while(!(USART5->ISR & USART_ISR_TXE)) { }
        USART5->TDR = '\r';
    }
    while(!(USART5->ISR & USART_ISR_TXE)) { }
    USART5->TDR = c;
    return c;
}

int __io_getchar(void) {
//     while (!(USART5->ISR & USART_ISR_RXNE)) { }
//     char c = USART5->RDR;
//     if (c == '\r') {
//         c = '\n';
//     }
//     putchar(c);
//     return c;
//    return 'a';
    return interrupt_getchar();
}

int main() {
    init_usart5();
    enable_tty_interrupt();
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    printf("Enter your name: ");
    char name[80];
    fgets(name, 80, stdin);
    printf("Your name is %s", name);
    printf("Type any characters.\n");
    for(;;) {
        char c = getchar();
        putchar(c);
    }
}
#endif
