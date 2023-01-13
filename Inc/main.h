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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TOUCH_KEYPAD_SDO_Pin GPIO_PIN_0
#define TOUCH_KEYPAD_SDO_GPIO_Port GPIOC
#define TOUCH_KEYPAD_SCL_Pin GPIO_PIN_3
#define TOUCH_KEYPAD_SCL_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define MOTOR_SLEEP_Pin GPIO_PIN_5
#define MOTOR_SLEEP_GPIO_Port GPIOC
#define SPI_NSS_Pin GPIO_PIN_12
#define SPI_NSS_GPIO_Port GPIOB
#define MOTOR_BUTTON_Pin GPIO_PIN_6
#define MOTOR_BUTTON_GPIO_Port GPIOC
#define MOTOR_BUTTON_EXTI_IRQn EXTI9_5_IRQn
#define REGISTER_BUTTON_Pin GPIO_PIN_8
#define REGISTER_BUTTON_GPIO_Port GPIOC
#define REGISTER_BUTTON_EXTI_IRQn EXTI9_5_IRQn
#define MOTOR_1_Pin GPIO_PIN_11
#define MOTOR_1_GPIO_Port GPIOA
#define MOTOR_2_Pin GPIO_PIN_12
#define MOTOR_2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
