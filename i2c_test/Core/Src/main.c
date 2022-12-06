/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SCREEN_PERIPH_ADDR (0x5a << 1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void I2C_send_packet(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                            uint8_t *pData, uint16_t Size) {
  do {
    if (HAL_I2C_Master_Transmit_IT(hi2c, DevAddress, pData, Size) != HAL_OK) {
      /* Error_Handler() function is called in case of error. */
      Error_Handler();
    }

    /* Before starting a new communication transfer, you need to check the
    current state of the peripheral; if it�s busy you need to wait for the end
    of current transfer before starting a new one. For simplicity reasons, this
    example is just waiting till the end of the transfer, but application may
    perform other tasks while transfer operation is ongoing. */
//    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY) {
//    }

    /* When Acknowledge failure occurs (Slave don't acknowledge its address)
    Master restarts communication */
  } while (HAL_I2C_GetError(hi2c) == HAL_I2C_ERROR_AF);
}
static void I2C_send_test() {
	uint8_t tx_val = 0x42;
	I2C_send_packet(&hi2c1, I2C_ADDRESS, &tx_val, 1);
}
static void Button0_Init() {
  GPIO_InitTypeDef GPIO_InitStruct;

  // Button 0 is connected to GPIO PA0
  // Enable RCC Clock to GPIOA
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure Button pin as input with External interrupt */
  GPIO_InitStruct.Pin = BUTTON0_PIN;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  HAL_GPIO_Init(BUTTON0_PORT, &GPIO_InitStruct);

  /* Enable and set Button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(EXTI0_IRQn), 0x0F, 2);
  HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI0_IRQn));
}

void UserLedInit(void) {
  GPIO_InitTypeDef initStruct;
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  initStruct.Pin = GPIO_PIN_12; // Pin for LED 4
  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_PULLUP;
  initStruct.Speed = GPIO_SPEED_FAST;
  // LED 4 is Pin 12 on GPIOD
  HAL_GPIO_Init(GPIOD, &initStruct);
  // LED 5 is Pin 14 on GPIOD
  initStruct.Pin = GPIO_PIN_14; // Pin for LED 5
  HAL_GPIO_Init(GPIOD, &initStruct);

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
}

void LED4_Off() { HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET); }

void LED4_Toggle() { HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); }

void LED5_Off() { HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); }
void LED5_On() { HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); }

void LED5_Toggle() { HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14); }
int i2c_write(uint8_t dat);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM7_Init();
//  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
//  UserLedInit();
//  LED4_Off();
//  Button0_Init();
  init_i2c();

  //  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */
	  HAL_Delay(5000);
	  i2c_start();
	    i2c_write((uint8_t)(I2C_ADDRESS<<1)|0x00);
	    i2c_write(0x42);
	    i2c_stop();
//	  i2c_addr(I2C_ADDRESS);
//	  i2c_write(0x42);
//	  i2c_stop();
//	  I2C_send_test();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 41999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 199;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
//void nano_wait(unsigned int);

#define GPIOEX_ADDR 0x00 // ENTER GPIO EXPANDER I2C ADDRESS HERE
#define EEPROM_ADDR 0x00 // ENTER EEPROM I2C ADDRESS HERE

uint8_t NUNCHUK_INIT = 0;

//void init_i2c(void) {
//  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
//
//  //    GPIOB->MODER &= ~0xF000;
//  //    GPIOB->MODER |= 0xA000; // PB 6-7 Alternate Function
//  //    GPIOB->AFR[0] &= ~(0xf << (4*(6)));
//  //    GPIOB->AFR[0] |=   0x4 << (4*(6));
//  //    GPIOB->AFR[0] &= ~(0xf << (4*(7)));
//  //    GPIOB->AFR[0] |=   0x4 << (4*(7));
//  GPIOB->MODER |= 2 << (2 * 6) | 2 << (2 * 7);
//  GPIOB->OTYPER |= 1 << 6 | 1 << 7;
//  GPIOB->OSPEEDR |= 3 << (2 * 6) | 3 << (2 * 7);
//  GPIOB->AFR[0] |= 4 << (4 * 6) | 4 << (4 * 7);
//
//  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
//
//  // Reseting I2C
//  I2C1->CR1 |= I2C_CR1_SWRST;
//  I2C1->CR1 &= ~I2C_CR1_SWRST;
//
//  I2C1->CR2 |= 28 << 0;
//  I2C1->CCR = 0x8c;
//  I2C1->TRISE = 0x1d;
//
//  // Enable I2C
//  I2C1->CR1 |= I2C_CR1_PE;
//}



//===========================================================================
// Main and supporting functions
//===========================================================================
//void nano_wait(unsigned int n) {
//  asm("        mov r0,%0\n"
//      "repeat: sub r0,#83\n"
//      "        bgt repeat\n"
//      :
//      : "r"(n)
//      : "r0", "cc");
//}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == BUTTON0_PIN) {
    LED4_Toggle();
//    PayRespects();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  //if (htim == &htim7) {
  //  uint8_t rcv_data;
  //  HAL_I2C_Master_Receive(&hi2c1, SCREEN_PERIPH_ADDR, &rcv_data, 1,
  //                         HAL_MAX_DELAY);
  //  if (rcv_data == 1) {
  //    LED4_Toggle();
  //  }
  //}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add own implementation to report the file name and line number */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
