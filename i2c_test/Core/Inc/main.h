/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "usb_device.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define BUTTON0_PIN			GPIO_PIN_0
#define BUTTON0_PORT		GPIOA
#define I2C_ADDRESS         0x5A
#define MASTER_REQ_READ    0x12
#define MASTER_REQ_WRITE   0x34

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

#define NUM_ROWS 5
#define NUM_COLS 15
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void LED5_Off();
void LED5_On();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
