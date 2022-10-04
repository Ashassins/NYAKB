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
I2C_HandleTypeDef hi2c3;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

PCD_HandleTypeDef hpcd_USB_OTG_HS;

/* USER CODE BEGIN PV */
const char font[] = {
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x00, // 32: space
  0x86, // 33: exclamation
  0x22, // 34: double quote
  0x76, // 35: octothorpe
  0x00, // dollar
  0x00, // percent
  0x00, // ampersand
  0x20, // 39: single quote
  0x39, // 40: open paren
  0x0f, // 41: close paren
  0x49, // 42: asterisk
  0x00, // plus
  0x10, // 44: comma
  0x40, // 45: minus
  0x80, // 46: period
  0x00, // slash
  // digits
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67,
  // seven unknown
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  // Uppercase
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x6f, 0x76, 0x30, 0x1e, 0x00, 0x38, 0x00,
  0x37, 0x3f, 0x73, 0x7b, 0x31, 0x6d, 0x78, 0x3e, 0x00, 0x00, 0x00, 0x6e, 0x00,
  0x39, // 91: open square bracket
  0x00, // backslash
  0x0f, // 93: close square bracket
  0x00, // circumflex
  0x08, // 95: underscore
  0x20, // 96: backquote
  // Lowercase
  0x5f, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x6f, 0x74, 0x10, 0x0e, 0x00, 0x30, 0x00,
  0x54, 0x5c, 0x73, 0x7b, 0x50, 0x6d, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x6e, 0x00
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C3_Init(void);
static void MX_USB_OTG_HS_PCD_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM6_Init(void);
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
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY) {
    }

    /* When Acknowledge failure occurs (Slave don't acknowledge its address)
    Master restarts communication */
  } while (HAL_I2C_GetError(hi2c) == HAL_I2C_ERROR_AF);
}
static void I2C_send_test() {
  /* I2C_send_packet(&hi2c3, I2C_ADDRESS, aTxBuffer, txBuffSize); */
  uint8_t data = 0x42;
  HAL_StatusTypeDef rv = HAL_I2C_Master_Transmit(&hi2c3, SCREEN_PERIPH_ADDR,
                                                 &data, 1, HAL_MAX_DELAY);
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
  HAL_NVIC_SetPriority((IRQn_Type)(EXTI0_IRQn), 0x0F, 0);
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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  // KEYBOARD DEBOUNCING
  init_tim6();
  int key = 0;
  for(;;) {
	char key = get_keypress();
  }
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
  MX_I2C3_Init();
  MX_USB_OTG_HS_PCD_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  UserLedInit();
  LED4_Off();
  Button0_Init();
  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 167;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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
  * @brief USB_OTG_HS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_HS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_HS_Init 0 */

  /* USER CODE END USB_OTG_HS_Init 0 */

  /* USER CODE BEGIN USB_OTG_HS_Init 1 */

  /* USER CODE END USB_OTG_HS_Init 1 */
  hpcd_USB_OTG_HS.Instance = USB_OTG_HS;
  hpcd_USB_OTG_HS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_HS.Init.speed = PCD_SPEED_HIGH;
  hpcd_USB_OTG_HS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.phy_itface = USB_OTG_ULPI_PHY;
  hpcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.use_dedicated_ep1 = DISABLE;
  hpcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_HS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_HS_Init 2 */

  /* USER CODE END USB_OTG_HS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == BUTTON0_PIN) {
    /* LED4_Toggle(); */
    I2C_send_test();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim6) {
	int rows = read_rows();
	update_history(col, rows);
	col = (col + 1) & 3;
	drive_column(col);
	get_keypress();
  }
  if (htim == &htim7) {
    uint8_t rcv_data;
    HAL_I2C_Master_Receive(&hi2c3, SCREEN_PERIPH_ADDR, &rcv_data, 1,
                           HAL_MAX_DELAY);
    if (rcv_data == 1) {
      LED4_Toggle();
    }
  }
}

void enable_ports(void) {
  // Enable RCC to GPIOB and GPIOC
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;//RCC_AHBENR_GPIOEEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;//RCC_AHBENR_GPIOCEN;

  // GPIOE PE0-10 Outputs
  GPIOE->MODER &= ~0x3FFFFF; // Clear PB0-10
  GPIOE->MODER |= 0x155555;

  // GPIOC PC4-7 Outputs
  GPIOC->MODER &= ~0xFF00;
  GPIOC->MODER |= 0x5500;

  // GPIOC PC0-3 Inputs
  GPIOC->MODER &= ~0xFF;
  // GPIOC PC0-3 Pull Down
  GPIOC->PUPDR &= ~0xFF;
  GPIOC->PUPDR |= 0xAA;
}

//===========================================================================
// Debouncing a Keypad
//===========================================================================
const char keymap[] = "DCBA#9630852*741";
uint8_t hist[16];
uint8_t col;
char queue[2];
int qin;
int qout;

void drive_column(int c)
{
  GPIOC->BSRR = 0xf00000 | (1 << (c + 4));
}

int read_rows()
{
  return GPIOC->IDR & 0xf;
}

void push_queue(int n) {
  n = (n & 0xff) | 0x80;
  queue[qin] = n;
  qin ^= 1;
}

uint8_t pop_queue() {
  uint8_t tmp = queue[qout] & 0x7f;
  queue[qout] = 0;
  qout ^= 1;
  return tmp;
}

void update_history(int c, int rows)
{
  for(int i = 0; i < 4; i++) {
    hist[4*c+i] = (hist[4*c+i]<<1) + ((rows>>i)&1);
    if (hist[4*c+i] == 1)
      push_queue(4*c+i);
  }
}

//void TIM6_DAC_IRQHandler(void) {
//  TIM6->SR &= ~TIM_SR_UIF;
//  int rows = read_rows();
//  update_history(col, rows);
//  col = (col + 1) & 3;
//  drive_column(col);
//}

char get_keypress() {
  for(;;) {
    asm volatile ("wfi" : :);   // wait for an interrupt
    if (queue[qout] == 0)
      continue;
    return keymap[pop_queue()];
  }
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
