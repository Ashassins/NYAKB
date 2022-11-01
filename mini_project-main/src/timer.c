#include "stm32f0xx.h"
#include "nunchuk.h"

// Global counter, modified in timer interrupt
int glbcnt = 0;

// Timer initializer and interrupt
void init_tim6() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 800 - 1;
    TIM6->ARR = 1000 - 1; // (48000000 / 800) / 1000 = 60
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;
}

void TIM6_DAC_IRQHandler(void) {
    TIM6->SR &= ~TIM_SR_UIF;
    glbcnt++;
    if(glbcnt > 59) glbcnt = 0;
    update_flags();
}
