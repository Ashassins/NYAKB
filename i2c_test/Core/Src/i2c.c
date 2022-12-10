#include "i2c.h"

void init_i2c(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  GPIOB->MODER |= 1 << (2 * 6) | 1 << (2 * 7);   // Setting PB6,7 to Output
  GPIOB->OTYPER |= 1 << 6 | 1 << 7;              // Open Drain
  GPIOB->OSPEEDR |= 3 << (2 * 6) | 3 << (2 * 7); // Very High Speed
  SCL_OFF;
}

void dly(int delay) {
  for (int i = 0; i < delay; i++) {
    asm("NOP");
  }
}

void i2c_start(void) {
  SDA_ON;
  dly(I2C_DELAY);
  SCL_ON;
  dly(I2C_DELAY);
  SDA_OFF;
  dly(I2C_DELAY);
  SCL_OFF;
  dly(I2C_DELAY);
}

void i2c_stop() {
  SDA_OFF;
  dly(I2C_DELAY);
  SCL_ON;
  dly(I2C_DELAY);
  SDA_ON;
  dly(I2C_DELAY);
}

#define I2C_MAX_DELAY 10000

uint32_t i2c_write(uint8_t dat) {

  for (uint8_t i = 8; i; i--) {
    (dat & 0x80) ? SDA_ON : SDA_OFF; // Mask for the eigth bit
    dat <<= 1;                       // Move
    dly(I2C_DELAY);
    SCL_ON;
    dly(I2C_DELAY);
    SCL_OFF;
    dly(I2C_DELAY);
  }
  SDA_ON;
  SCL_ON;
  dly(I2C_DELAY);
  // TODO Check properly, for now just punt to someone else
  uint32_t count = 0;
  while (SDA_READ) {
    count += 1;
    if (count > I2C_MAX_DELAY) {
      goto cleanup;
    }
  }
  cleanup:
  SCL_OFF;
  return count > I2C_MAX_DELAY;
}

uint8_t i2c_write_bytes(uint8_t addr, uint8_t *data, uint32_t size) {
  i2c_start();
  uint8_t ack = 0;

  ack |= i2c_write(addr << 1);
  if (ack) {
    return ack;
  }
  for (uint32_t i = 0; i < size; i++) {
    ack |= i2c_write(data[i]);
    if (ack) {
      goto cleanup;
    }
  }
  cleanup:
  i2c_stop();
  return ack;
}
