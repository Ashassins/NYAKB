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
			

void i2c_init(void){
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
    I2C1->OAR1 = I2C_OAR1_OA1EN | 0x2; // Set 7-bit own address 1 address is 0x5A
    I2C1->OAR2 &= ~I2C_OAR2_OA2EN; // Disable own address 2
    I2C1->CR2 &= ~I2C_CR2_ADD10; // 0 = 7 bit mode 1 = 10 bit mode
    I2C1->CR2 |= I2C_CR2_AUTOEND; // Enable the auto end
    I2C1->CR2 |= I2C_CR2_NACK; // for slave mode: set NACK

    I2C1->CR1 |= I2C_CR1_PE; // Enables peripheral
}



//===========================================================================
// I2C helpers
//===========================================================================

void i2c_waitidle(void) {
    while((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);
}

void i2c_start(uint32_t devaddr, uint8_t size, uint8_t dir) {
    // dir: 0 = master requests a write transfer
    // dir: 1 = master requests a read transfer
    uint32_t tmpreg = I2C1->CR2;
    tmpreg &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD |
            I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP);
    if (dir == 1)
        tmpreg |= I2C_CR2_RD_WRN; // Read from slave
    else
        tmpreg &= ~I2C_CR2_RD_WRN; // Write to slave
    tmpreg |= ((devaddr<<1) & I2C_CR2_SADD) | ((size<<16) & I2C_CR2_NBYTES);
    tmpreg |= I2C_CR2_START;
    I2C1->CR2 = tmpreg;
}

void i2c_stop(void) {
    if (I2C1->ISR & I2C_ISR_STOPF)
        return;
    // Master: Generate STOP bit after current byte has been transferred.
    I2C1->CR2 |= I2C_CR2_STOP;
    // Wait until STOPF flag is reset
    while((I2C1->ISR & I2C_ISR_STOPF) == 0);
    I2C1->ICR |= I2C_ICR_STOPCF; // Write to clear STOPF flag
}

int i2c_senddata(uint8_t devaddr, const void *data, uint8_t size) {
    int i;
    if (size <= 0 || data == 0) return -1;
    uint8_t *udata = (uint8_t*)data;
    i2c_waitidle();
    // Last arguement is dir: 0 = sending data to slave device
    i2c_start(devaddr, size, 0);

    for(i=0; i<size; i++){
        // TXIS bit is set by hardware when TXDR register is empty and the
        // data to be transmitted must be written in the TXDR register. It is
        // cleared when the next data to be sent is written in the TXDR reg.
        // The TXIS flag is not set when NACK is recieved.
        int count = 0;
        while((I2C1->ISR & I2C_ISR_TXIS) == 0){
            count += 1;
            if (count > 1000000) return -1;
            if (i2c_checknack()) {i2c_clearnack(); i2c_stop(); return -1;}
        }
        // TXIS is cleared by writing to the TXDR register
        I2C1->TXDR = udata[i] & I2C_TXDR_TXDATA;
    }
    // Wait until TC flag is ser or the NACK flag is set
    while((I2C1->ISR & I2C_ISR_TC) == 0 && (I2C1->ISR & I2C_ISR_NACKF) == 0);
    if((I2C1->ISR & I2C_ISR_NACKF) != 0)
        return -1;
    i2c_stop();
    return 0;
}

int i2c_recvdata(uint8_t devaddr, void *data, uint8_t size) {
    int i;
    if (size <= 0 || data == 0) return -1;
    uint8_t *udata = (uint8_t*)data;
    i2c_waitidle();
    // Last argument is dir: 1 = receiving data from the slave device
    i2c_start(devaddr, size, 1);
    for(i=0; i<size; i++){
        int count = 0;
        while((I2C1->ISR & I2C_ISR_RXNE) == 0){
            count += 1;
            if (count > 1000000) return -1;
            if (i2c_checknack()) {i2c_clearnack(); i2c_stop(); return -1;}
        }
        udata[i] = I2C1->RXDR;
    }
    // Wait until TC flag is set or the NACK flag is set
    while((I2C1->ISR & I2C_ISR_TC) == 0 && (I2C1->ISR & I2C_ISR_NACKF) == 0);
    if((I2C1->ISR & I2C_ISR_NACKF) != 0)
        return -1;
    i2c_stop();
    return 0;
}

int main(void)
{
    i2c_init();
    while(1) {
        i2c_waitidle();
        i2c_start(0x5A, 0, 0);
        int x = 0;
        while((I2C1->ISR & I2C_ISR_TC) == 0 &&
                (I2C1->ISR & I2C_ISR_STOPF) == 0 &&
                (I2C1->ISR & I2C_ISR_NACKF) == 0)
            x++;
        if (I2C1->ISR & I2C_ISR_NACKF)
            I2C1->ICR |= I2C_ISR_NACKF;
        if (I2C1->ISR & I2C_ISR_STOPF)
                I2C1->ICR |= I2C_ISR_STOPF;
        else
            i2c_stop();
    }
}
