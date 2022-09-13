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
			
void slave_mode(void){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= 2<<(2*6) | 2<<(2*7);
    GPIOB->AFR[0] |= 1<<(4*6) | 1<<(4*7);

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    I2C1->CR1 &= ~I2C_CR1_PE; // Disable to perform reset
    I2C1->CR1 &= ~I2C_CR1_ANFOFF; // 0: Analog noise filter on
    I2C1->CR1 &= ~I2C_CR1_ERRIE; // Error interrupt disable
    I2C1->CR1 &= ~I2C_CR1_NOSTRETCH; // Enable clock stretching

    I2C1->TIMINGR = 0;
    I2C1->TIMINGR &= ~I2C_TIMINGR_PRESC; // Clear prescaler
    I2C1->TIMINGR |= 0 << 28; // Set prescaler to 0
    I2C1->TIMINGR |= 3 << 20; // SCLDEL
    I2C1->TIMINGR |= 1 << 16; // SDADEL
    I2C1->TIMINGR |= 3 << 8; // SCLH
    I2C1->TIMINGR |= 9 << 0; // SCLL

    // I2c "Own Address" 1 register
    I2C1->OAR1 &= ~I2C_OAR1_OA1EN; // Disable own address 1
    I2C1->OAR1 = I2C_OAR1_OA1EN | 0x5A; // Set 7-bit own address 1 address is 0x5A
    I2C1->OAR2 &= ~I2C_OAR2_OA2EN; // Disable own address 2
    I2C1->CR2 &= ~I2C_CR2_ADD10; // 0 = 7 bit mode 1 = 10 bit mode
    I2C1->CR2 |= I2C_CR2_AUTOEND; // Enable the auto end
    I2C1->CR2 |= I2C_CR2_NACK; // for slave mode: set NACK

    I2C1->CR1 |= I2C_CR1_PE; // Enables peripheral
}

void transmitter(I2C_BYTE_TO_SEND) {
    uint32_t I2C_InterruptStatus = I2C1->ISR; // gets interrupt status
    // check if address matches
    if ((I2C_InterruptStatus & I2C_ISR_ADDR) == I2C_ISR_ADDR)
        I2C1->ICR |= I2C_ICR_ADDRCF; // clear address match flag
        if ((I2C1->ISR & I2C_ISR_DIR) == I2C_ISR_DIR)
            I2C1->CR1 |= I2C_CR1_TXIE; // set transmit IT
    else if ((I2C_InterruptStatus & I2C_ISR_TXIS) == I2C_ISR_TXIS)
        I2C1->CR1 &= ~I2C_CR1_TXIE; // Disable transmit IT
        I2C1->TXDR = I2C_BYTE_TO_SEND; // Byte to send
}

void reciever(I2C_BYTE_TO_SEND){
    uint32_t I2C_InterruptStatus = I2C1->ISR; // gets interrupt status
    if ((I2C_InterruptStatus & I2C_ISR_ADDR) == I2C_ISR_ADDR)
        I2C1->ICR |= I2C_ICR_ADDRCF; // address match event
    else if ((I2C_InterruptStatus & I2C_ISR_RXNE) == I2C_ISR_RXNE)
        // read recieve register, will clear RXNE flag
        if (I2C1->RXDR == I2C_BYTE_TO_SEND)
            // blinking
}

int main(void)
{

	for(;;);
}
