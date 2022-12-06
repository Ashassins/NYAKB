/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define I2C_DELAY 420
#define PI2C_SCL 6
#define PI2C_SDA 7
#define OUT_REG GPIOB->ODR
#define IN_REG GPIOB->IDR
#define SDA_ON (OUT_REG |= (1<< PI2C_SDA))
#define SDA_OFF (OUT_REG &= ~(1<< PI2C_SDA))
#define SCL_ON (OUT_REG |= (1<< PI2C_SCL))
#define SCL_OFF (OUT_REG &= ~(1<< PI2C_SCL))
#define SDA_READ (IN_REG & (1<< PI2C_SDA))


void init_i2c(void);
void i2c_start(void);
void i2c_stop(void);
uint32_t i2c_write(uint8_t dat);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
