/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gd25q16e.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern struct GD25Q16E_Dev flash;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Timer2_10ms_handle(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FL_CS_Pin LL_GPIO_PIN_4
#define FL_CS_GPIO_Port GPIOA
#define FL_SCK_Pin LL_GPIO_PIN_5
#define FL_SCK_GPIO_Port GPIOA
#define FL_MISO_Pin LL_GPIO_PIN_6
#define FL_MISO_GPIO_Port GPIOA
#define FL_MOSI_Pin LL_GPIO_PIN_7
#define FL_MOSI_GPIO_Port GPIOA
#define SW2_Pin LL_GPIO_PIN_12
#define SW2_GPIO_Port GPIOB
#define TTP_CLK_Pin LL_GPIO_PIN_13
#define TTP_CLK_GPIO_Port GPIOB
#define TTP_SDO_Pin LL_GPIO_PIN_14
#define TTP_SDO_GPIO_Port GPIOB
#define LED_B_Pin LL_GPIO_PIN_7
#define LED_B_GPIO_Port GPIOC
#define LED_G_Pin LL_GPIO_PIN_8
#define LED_G_GPIO_Port GPIOC
#define LED_R_Pin LL_GPIO_PIN_9
#define LED_R_GPIO_Port GPIOC
#define SW1_Pin LL_GPIO_PIN_15
#define SW1_GPIO_Port GPIOA
#define KQ0_Pin LL_GPIO_PIN_3
#define KQ0_GPIO_Port GPIOB
#define KQ1_Pin LL_GPIO_PIN_4
#define KQ1_GPIO_Port GPIOB
#define KQ2_Pin LL_GPIO_PIN_5
#define KQ2_GPIO_Port GPIOB
#define KQ3_Pin LL_GPIO_PIN_6
#define KQ3_GPIO_Port GPIOB
#define KQ4_Pin LL_GPIO_PIN_7
#define KQ4_GPIO_Port GPIOB
#define KQ5_Pin LL_GPIO_PIN_8
#define KQ5_GPIO_Port GPIOB
#define KQ6_Pin LL_GPIO_PIN_9
#define KQ6_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
