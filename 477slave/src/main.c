
#include "stm32f0xx.h"
			
void slave_mode(void){
    /* (1) Timing register value is computed with the AN4235 xls file,
     fast Mode @400kHz with I2CCLK = 48MHz, rise time = 140ns,
     fall time = 40ns */
    /* (2) Periph enable, address match interrupt enable */
    /* (3) 7-bit address = 0x5A */
    /* (4) Enable own address 1 */
    uint32_t I2C1_OWN_ADDRESS = 0x5a;
    I2C1->TIMINGR = (uint32_t)0x00B00000; /* (1) */
    I2C1->CR1 = I2C_CR1_PE | I2C_CR1_ADDRIE; /* (2) */
    I2C1->OAR1 |= (uint32_t)(I2C1_OWN_ADDRESS << 1); /* (3) */
    I2C1->OAR1 |= I2C_OAR1_OA1EN; /* (4) */

}

void transmitter(I2C_BYTE_TO_SEND) {
    uint32_t I2C_InterruptStatus = I2C1->ISR; // gets interrupt status
    // check if address matches
    if ((I2C_InterruptStatus & I2C_ISR_ADDR) == I2C_ISR_ADDR)
    {
        I2C1->ICR |= I2C_ICR_ADDRCF; // clear address match flag
        if ((I2C1->ISR & I2C_ISR_DIR) == I2C_ISR_DIR)
        {
            I2C1->CR1 |= I2C_CR1_TXIE; // set transmit IT
        }
    }
    else if ((I2C_InterruptStatus & I2C_ISR_TXIS) == I2C_ISR_TXIS)
    {
        I2C1->CR1 &= ~I2C_CR1_TXIE; // Disable transmit IT
        I2C1->TXDR = I2C_BYTE_TO_SEND; // Byte to send
    }
}

void reciever(I2C_BYTE_TO_SEND){
    uint32_t I2C_InterruptStatus = I2C1->ISR; /* Get interrupt status */
    if ((I2C_InterruptStatus & I2C_ISR_ADDR) == I2C_ISR_ADDR)
    {
        I2C1->ICR |= I2C_ICR_ADDRCF; /* Address match event */
    }
    else if ((I2C_InterruptStatus & I2C_ISR_RXNE) == I2C_ISR_RXNE)
    {
        /* Read receive register, will clear RXNE flag */
        if (I2C1->RXDR == I2C_BYTE_TO_SEND)
             {
             /* Process */
             }
    }
}

int main(void)
{

	for(;;);
}
