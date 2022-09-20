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
#include <stdint.h>

void initI2CPeripheral(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->MODER &= ~((3 << (2 * 6)) | (3 << (2 * 7)));
  GPIOB->MODER |= (2 << (2 * 6)) | (2 << (2 * 7));
  GPIOB->AFR[0] &= ~((0xf << (4 * 6)) | (0xf << (4 * 7)));
  GPIOB->AFR[0] |= 1 << (4 * 6) | 1 << (4 * 7);

  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

  // Disable to perform reset, 0: Analog noise filter on, Error interrupt
  // disable,Enable clock stretching
  /* (4) Enable own address 1 */
  I2C1->CR1 = 0;
  I2C1->TIMINGR = (uint32_t)0x00B00000;    /* (1) */
  I2C1->CR1 = I2C_CR1_PE | I2C_CR1_ADDRIE; /* (2) */
  I2C1->OAR1 |= (uint32_t)(0x5a << 1);     /* (3) */
  I2C1->OAR1 |= I2C_OAR1_OA1EN;            /* (4) */

  NVIC_EnableIRQ((IRQn_Type)I2C1_IRQn);
  NVIC_SetPriority((IRQn_Type)I2C1_IRQn, 2);
  /* NVIC->ISER[0] = 1 << I2C1_IRQn; */
}

void initLED(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER |= (1 << (2 * 6));
}

void toggleLED(void) {
  if (GPIOC->ODR & (1 << 6)) {
    GPIOC->ODR &= ~(1 << 6);
  } else {
    GPIOC->ODR |= (1 << 6);
  }
}

void I2C1_IRQHandler(void) {
  uint32_t isr_stat = I2C1->ISR;
  if (isr_stat & I2C_ISR_ADDR) {
    I2C1->ICR |= I2C_ICR_ADDRCF;
  }
  if ((isr_stat & I2C_ISR_RXNE) == I2C_ISR_RXNE) {
    I2C1->ICR |= I2C_ICR_ADDRCF;
    uint32_t data = I2C1->RXDR;
    if (data == 0x42) {
      toggleLED();
    }
  }
}

int main(void) {
  initI2CPeripheral();
  initLED();
  toggleLED();

  for (;;) {
    asm("nop");
  }
}
