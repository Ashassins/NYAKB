/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usb_device.c
 * @version        : v1.0_Cube
 * @brief          : This file implements the USB Device
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

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

/* USER CODE BEGIN Includes */
#define RIGHT_GUI 1 << 7
#define RIGHT_ALT 1 << 6
#define RIGHT_SHIFT 1 << 5
#define RIGHT_CTRL 1 << 4
#define LEFT_GUI 1 << 3
#define LEFT_ALT 1 << 2
#define LEFT_SHIFT 1 << 1
#define LEFT_CTRL 1 << 0
#define KEYCODE_BASE 0x4
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

typedef struct {
  uint8_t modifier;
  uint8_t reserved;
  uint8_t keycodes[6];
} __packed KeyboardReport;

KeyboardReport keyboardReport = {0};

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

void PayRespects(void) {
  keyboardReport.modifier |= LEFT_SHIFT;
  keyboardReport.keycodes[0] = KEYCODE_BASE + 'f' - 'a';
  USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardReport,
                               sizeof(KeyboardReport));
  HAL_Delay(50);
  keyboardReport.modifier = 0;
  keyboardReport.keycodes[0] = 0;
  USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardReport,
                      sizeof(KeyboardReport));
  HAL_Delay(100);
}
/* USER CODE END 1 */

/**
 * Init USB device Library, add supported class and start the library
 * @retval None
 */
void MX_USB_DEVICE_Init(void) {
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */

  /* USER CODE END USB_DEVICE_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK) {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID) != USBD_OK) {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK) {
    Error_Handler();
  }

  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */

  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}

/**
 * @}
 */

/**
 * @}
 */
